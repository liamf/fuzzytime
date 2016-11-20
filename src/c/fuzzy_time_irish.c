/*
 * Irish Fuzzy Time
 *
 * Simple watchface to tell time in the Hiberno-English style
 *
 *  jim.lawton@gmail.com
 *  modifications by
 *  liam.friel@gmail.com
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
#define HOURLY_VIBE_START 7
#define HOURLY_VIBE_END 23

// Define DEBUG to turn on verbose logging
// #define DEBUG 

/* Fonts. */
#define TOPBAR_FONT       FONT_KEY_GOTHIC_14_BOLD
#define BOTTOMBAR_FONT    FONT_KEY_GOTHIC_14_BOLD

#define LINE1_FONT        FONT_KEY_BITHAM_42_LIGHT
#define LINE2_NORMAL_FONT FONT_KEY_BITHAM_42_LIGHT
#define LINE2_BOLD_FONT   FONT_KEY_BITHAM_42_BOLD
#define LINE3_FONT        FONT_KEY_BITHAM_42_BOLD

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

#define TIMELINE_SPACING  48

const int line1_y = 14;
const int line2_y = 62;
const int line3_y = 107;

static void update_watch(struct tm *t) {
    fuzzy_time(t->tm_hour, t->tm_min, new_time.line1, new_time.line2, new_time.line3);
    
    strftime(str_topbar, sizeof(str_topbar), "%A %H:%M", t);
    text_layer_set_text(topbarLayer, str_topbar);

    strftime(str_bottombar, sizeof(str_bottombar), "%d %b", t);
    text_layer_set_text(bottombarLayer, str_bottombar);

    if (t->tm_min == 0) {
      
#ifdef HOURLY_VIBE
        /* No vibe during the night. */
        if (t->tm_hour > HOURLY_VIBE_START && t->tm_hour <= HOURLY_VIBE_END) {
            vibes_short_pulse();
        }
#endif

    }

    text_layer_set_text(line1.layer[0], new_time.line1);
    text_layer_set_text(line2.layer[0], new_time.line2);
    text_layer_set_text(line3.layer[0], new_time.line3);

}

static void init_watch(struct tm *t) {
    fuzzy_time(t->tm_hour, t->tm_min, new_time.line1, new_time.line2, new_time.line3);

    strftime(str_topbar, sizeof(str_topbar), "%A %H:%M", t);
    text_layer_set_text(topbarLayer, str_topbar);

    strftime(str_bottombar, sizeof(str_bottombar), "%d %b", t);
    text_layer_set_text(bottombarLayer, str_bottombar);

    strcpy(cur_time.line1, new_time.line1);
    strcpy(cur_time.line2, new_time.line2);
    strcpy(cur_time.line3, new_time.line3);
    
    text_layer_set_text(line1.layer[0], cur_time.line1);
    text_layer_set_text(line2.layer[0], cur_time.line2);
    text_layer_set_text(line3.layer[0], cur_time.line3);
  
    #ifdef DEBUG   
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Watch Init done");
    #endif
    
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
    #ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Tick");
    #endif
    update_watch(tick_time);
}

static void window_load(Window *window) {
    #ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Window load entry");
    #endif
    
    Layer *windowLayer = window_get_root_layer(window);   
    GRect frame = layer_get_bounds(windowLayer);

    #ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "W1");
    #endif
    
    window_set_background_color(window, GColorBlack);

    // Our display has 3 lines
    // top info bar
    // Three lines of time text
    // bottom info bar
    
    line1.layer[0] = text_layer_create(GRect(0, line1_y, frame.size.w, TIMELINE_SPACING));
    text_layer_set_text_alignment(line1.layer[0], GTextAlignmentCenter);
    text_layer_set_text_color(line1.layer[0],GColorRajah);
    text_layer_set_background_color(line1.layer[0],GColorBlack);    
    text_layer_set_font(line1.layer[0], fonts_get_system_font(LINE1_FONT));
    layer_add_child(windowLayer, text_layer_get_layer(line1.layer[0]));

    line2.layer[0] = text_layer_create(GRect(0, line2_y, frame.size.w, TIMELINE_SPACING));
    text_layer_set_text_alignment(line2.layer[0], GTextAlignmentCenter);
    text_layer_set_text_color(line2.layer[0],GColorWhite);
    text_layer_set_background_color(line2.layer[0],GColorBlack);
    text_layer_set_font(line2.layer[0], fonts_get_system_font(LINE2_NORMAL_FONT));
    layer_add_child(windowLayer, text_layer_get_layer(line2.layer[0]));

    line3.layer[0] = text_layer_create(GRect(0, line3_y, frame.size.w, TIMELINE_SPACING));
    text_layer_set_text_alignment(line3.layer[0], GTextAlignmentCenter);
    text_layer_set_text_color(line3.layer[0],GColorRajah);
    text_layer_set_background_color(line3.layer[0],GColorBlack);
    text_layer_set_font(line3.layer[0], fonts_get_system_font(LINE3_FONT));
    layer_add_child(windowLayer, text_layer_get_layer(line3.layer[0]));

    topbarLayer = text_layer_create(GRect(0, 6, frame.size.w, 18));
    text_layer_set_text_color(topbarLayer, GColorWhite);
    text_layer_set_background_color(topbarLayer, GColorBlack);
    text_layer_set_font(topbarLayer, fonts_get_system_font(TOPBAR_FONT));
    text_layer_set_text_alignment(topbarLayer, GTextAlignmentCenter);
    layer_add_child(windowLayer, text_layer_get_layer(topbarLayer));

    bottombarLayer = text_layer_create(GRect(0, 150, frame.size.w, 18));
    text_layer_set_text_color(bottombarLayer, GColorWhite);
    text_layer_set_background_color(bottombarLayer, GColorBlack);
    text_layer_set_font(bottombarLayer, fonts_get_system_font(BOTTOMBAR_FONT));
    text_layer_set_text_alignment(bottombarLayer, GTextAlignmentCenter);
    layer_add_child(windowLayer, text_layer_get_layer(bottombarLayer));


    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    
    #ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "About to init watch");
    #endif
    
    init_watch(t);
}

static void window_unload(Window *window) {
    #ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Window unload");
    #endif    
       
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
    #ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Init");
    #endif
    
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
    #ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Deinit");
    #endif
    tick_timer_service_unsubscribe();
    window_destroy(window);
}

int main(void) {
    init();
    #ifdef DEBUG
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
    #endif
    app_event_loop();
    deinit();
}