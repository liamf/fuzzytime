/*
 * Irish Fuzzy Time
 * jim.lawton@gmail.com
 *
 * Inspired by Fuzzy Time +, Fuzzy Time.
 * 
 * Configuration:
 *  - Define HOURLY_VIBE to enable the hourly vibration.
 */


#include <pebble.h>
#include <time.h>
#include "fuzzy_time.h"

/* ========== Configuration ========== */


/* Enable hourly vibration. */
#define HOURLY_VIBE
#define HOURLY_VIBE_START 8
#define HOURLY_VIBE_END 23


/* Fonts. */
#define LINE1_FONT FONT_KEY_BITHAM_42_LIGHT
#define LINE2_NORMAL_FONT FONT_KEY_BITHAM_42_LIGHT
#define LINE2_BOLD_FONT FONT_KEY_BITHAM_42_BOLD
#define LINE3_FONT FONT_KEY_BITHAM_42_BOLD

/* ========= End configuration. ========== */

#define LINE_BUFFER_SIZE 50

static Window *window;

typedef struct {
    TextLayer *layer[2];
} TextLine;

typedef struct {
    char line1[LINE_BUFFER_SIZE];
    char line2[LINE_BUFFER_SIZE];
    char line3[LINE_BUFFER_SIZE];
} TimeLines;


static TextLayer *topbarLayer;
static char str_topbar[LINE_BUFFER_SIZE];
static TextLayer *bottombarLayer;
static char str_bottombar[LINE_BUFFER_SIZE];


static TextLayer *line3_bg;
static TextLine line1;
static TextLine line2;
static TextLine line3;

static TimeLines cur_time;
static TimeLines new_time;

const int line1_y = 18;
const int line2_y = 60;
const int line3_y = 102;

static void set_pm_style(void) {
    text_layer_set_text_color(line3.layer[0], GColorWhite);
    text_layer_set_background_color(line3.layer[0], GColorClear);
    text_layer_set_text_color(line3.layer[1], GColorWhite);
    text_layer_set_background_color(line3.layer[1], GColorClear);
    text_layer_set_background_color(line3_bg, GColorClear);
}

static void set_line2_pm(void) {
    Layer *layer = text_layer_get_layer(line2.layer[0]);
    GRect rect = layer_get_frame(layer);
    if (rect.origin.x == 0) {
        text_layer_set_font(line2.layer[1], fonts_get_system_font(LINE2_BOLD_FONT));
    } else {
        text_layer_set_font(line2.layer[0], fonts_get_system_font(LINE2_BOLD_FONT));
    }
}

static void reset_line2(void) {
    Layer *layer = text_layer_get_layer(line2.layer[0]);
    GRect rect = layer_get_frame(layer);
    if (rect.origin.x == 0) {
        text_layer_set_text_color(line2.layer[1], GColorWhite);
        text_layer_set_background_color(line2.layer[1], GColorBlack);
        text_layer_set_font(line2.layer[1], fonts_get_system_font(LINE2_NORMAL_FONT));
    } else {
        text_layer_set_text_color(line2.layer[0], GColorWhite);
        text_layer_set_background_color(line2.layer[0], GColorBlack);
        text_layer_set_font(line2.layer[0], fonts_get_system_font(LINE2_NORMAL_FONT));
    }
}

void updateLayer(TextLine *animating_line, int line) {
    TextLayer *inside, *outside;
    Layer *layer = text_layer_get_layer(animating_line->layer[0]);
    GRect rect = layer_get_frame(layer);

    inside = (rect.origin.x == 0) ? animating_line->layer[0] : animating_line->layer[1];
    Layer *in_layer = text_layer_get_layer(inside);
    outside = (inside == animating_line->layer[0]) ? animating_line->layer[1] : animating_line->layer[0];
    Layer *out_layer = text_layer_get_layer(outside);

    /* TODO: is this the wrong way round? */
    GRect in_rect = layer_get_frame(in_layer);
    GRect out_rect = layer_get_frame(out_layer);

    in_rect.origin.x -= 144;
    out_rect.origin.x -= 144;

    if (line == 1) {
        text_layer_set_text(outside, new_time.line1);
        text_layer_set_text(inside, cur_time.line1);
    }

    if (line == 2) {
        text_layer_set_text(outside, new_time.line2);
        text_layer_set_text(inside, cur_time.line2);
    }

    if (line == 3) {
        text_layer_set_text(outside, new_time.line3);
        text_layer_set_text(inside, cur_time.line3);
    }

}

static void update_watch(struct tm *t) {
    fuzzy_time(t->tm_hour, t->tm_min, new_time.line1, new_time.line2, new_time.line3);
    

    strftime(str_topbar, sizeof(str_topbar), "%H:%M | %A | %e %b", t);
    text_layer_set_text(topbarLayer, str_topbar);

    strftime(str_bottombar, sizeof(str_bottombar), " %H%M | Week %W", t);
    text_layer_set_text(bottombarLayer, str_bottombar);

    if (t->tm_min == 0) {
#ifdef HOURLY_VIBE
        /* No vibe during the night. */
        if (t->tm_hour > HOURLY_VIBE_START && t->tm_hour <= HOURLY_VIBE_END) {
            vibes_short_pulse();
        }
#endif

        set_line2_pm();
    }

    if (t->tm_min > 1) {
        reset_line2();
    }


    set_pm_style();

    if (strcmp(new_time.line1, cur_time.line1) != 0) {
        updateLayer(&line1, 1);
    }
    if (strcmp(new_time.line2, cur_time.line2) != 0) {
        updateLayer(&line2, 2);
    }
    if (strcmp(new_time.line3, cur_time.line3) != 0) {
        updateLayer(&line3, 3);
    }
}

static void init_watch(struct tm *t) {
    fuzzy_time(t->tm_hour, t->tm_min, new_time.line1, new_time.line2, new_time.line3);

    strftime(str_topbar, sizeof(str_topbar), "%H:%M | %A | %e %b", t);
    text_layer_set_text(topbarLayer, str_topbar);

    strftime(str_bottombar, sizeof(str_bottombar), " %H%M | Week %W", t);
    text_layer_set_text(bottombarLayer, str_bottombar);

    strcpy(cur_time.line1, new_time.line1);
    strcpy(cur_time.line2, new_time.line2);
    strcpy(cur_time.line3, new_time.line3);


    set_line2_pm();
    set_pm_style();

    text_layer_set_text(line1.layer[0], cur_time.line1);
    text_layer_set_text(line2.layer[0], cur_time.line2);
    text_layer_set_text(line3.layer[0], cur_time.line3);
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick");
    update_watch(tick_time);
}

static void window_load(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Window load");
    Layer *windowLayer = window_get_root_layer(window);
    GRect frame = layer_get_frame(windowLayer);

    window_set_background_color(window, GColorBlack);

    line1.layer[0] = text_layer_create(GRect(0, line1_y, frame.size.w, 50));
    text_layer_set_text_alignment(line1.layer[0], GTextAlignmentLeft);
    text_layer_set_background_color(line1.layer[0], GColorClear);
    text_layer_set_text_color(line1.layer[0], GColorWhite);
    layer_add_child(windowLayer, text_layer_get_layer(line1.layer[0]));

    line1.layer[1] = text_layer_create(GRect(frame.size.w, line1_y, frame.size.w, 50));
    text_layer_set_background_color(line1.layer[1], GColorClear);
    text_layer_set_text_color(line1.layer[1], GColorWhite);
    text_layer_set_font(line1.layer[1], fonts_get_system_font(LINE1_FONT));
    layer_add_child(windowLayer, text_layer_get_layer(line1.layer[1]));

    line2.layer[0] = text_layer_create(GRect(0, line2_y, frame.size.w, 50));
    text_layer_set_text_alignment(line2.layer[0], GTextAlignmentLeft);
    text_layer_set_background_color(line2.layer[0], GColorBlack);
    text_layer_set_text_color(line2.layer[0], GColorWhite);
    text_layer_set_font(line2.layer[0], fonts_get_system_font(LINE2_NORMAL_FONT));
    layer_add_child(windowLayer, text_layer_get_layer(line2.layer[0]));

    line2.layer[1] = text_layer_create(GRect(frame.size.w, line2_y, frame.size.w, 50));
    text_layer_set_text_alignment(line2.layer[1], GTextAlignmentLeft);
    text_layer_set_background_color(line2.layer[1], GColorBlack);
    text_layer_set_text_color(line2.layer[1], GColorWhite);
    text_layer_set_font(line2.layer[1], fonts_get_system_font(LINE2_NORMAL_FONT));

    line3.layer[0] = text_layer_create(GRect(0, line3_y, frame.size.w, 50));
    text_layer_set_text_alignment(line3.layer[0], GTextAlignmentLeft);
    text_layer_set_font(line3.layer[0], fonts_get_system_font(LINE3_FONT));

    line3.layer[1] = text_layer_create(GRect(frame.size.w, line3_y, frame.size.w, 50));
    text_layer_set_text_alignment(line3.layer[1], GTextAlignmentLeft);

    text_layer_set_font(line3.layer[1], fonts_get_system_font(LINE3_FONT));

    topbarLayer = text_layer_create(GRect(0, 0, 144, 18));
    text_layer_set_text_color(topbarLayer, GColorWhite);
    text_layer_set_background_color(topbarLayer, GColorBlack);
    text_layer_set_font(topbarLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text_alignment(topbarLayer, GTextAlignmentCenter);

    bottombarLayer = text_layer_create(GRect(0, 150, 144, 18));
    text_layer_set_text_color(bottombarLayer, GColorWhite);
    text_layer_set_background_color(bottombarLayer, GColorBlack);
    text_layer_set_font(bottombarLayer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text_alignment(bottombarLayer, GTextAlignmentCenter);


    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    init_watch(t);
}

static void window_unload(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Window unload");
    text_layer_destroy(line1.layer[0]);
    text_layer_destroy(line1.layer[1]);
    text_layer_destroy(line2.layer[0]);
    text_layer_destroy(line2.layer[1]);
    text_layer_destroy(line3.layer[0]);
    text_layer_destroy(line3.layer[1]);
    text_layer_destroy(topbarLayer);
    text_layer_destroy(bottombarLayer);

}

static void init(void) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Init");
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    const bool animated = true;
    window_stack_push(window, animated);
    tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

static void deinit(void) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Deinit");
    tick_timer_service_unsubscribe();
    window_destroy(window);
}

int main(void) {
    init();
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
    app_event_loop();
    deinit();
}