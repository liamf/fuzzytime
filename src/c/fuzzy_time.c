#include <pebble.h>
#include "fuzzy_time.h"

#include "string.h"

static const char* const HOURS[] = {
    "twelve",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
    "ten",
    "eleven",
    "twelve",
};

// Time      line1     line2     line3
// ----      --------  --------  --------
// 5:00      it's      five      o'clock
// 5:01-03   gone      five      o'clock
// 5:04      nearly    five past five
// 5:05                five past five
// 5:06-08   after     five past five
// 5:09      nearly    ten past  five
// 5:10                ten past  five
// ..
// 5:44      quarter   to        six
// 5:45      quarter   to        six
// 5:46-48   quarter   to        six
// 5:49      nearly    ten to    six
// 5:50                ten to    six

void fuzzy_time(int hours, int minutes, char* line1, char* line2, char* line3) {

    if (minutes % 5 >= 1 && minutes % 5 < 3) {
        strcpy(line1, "gone");
    }

    if (minutes % 5 == 3) {
        strcpy(line1, "nearly");
    }
  
    if (minutes % 5 == 4) {
        strcpy(line1, "almost");
    }

    if (minutes >= 34) {
        hours += 1;
    }

    if (hours > 12)
        hours -= 12;

    switch (minutes) {
    case 0:
        strcpy(line1, "it's");
        strcpy(line2, HOURS[hours]);
        strcpy(line3, "o'clock");
        break;
    case 1:
    case 2:
    case 3:
        strcpy(line2, HOURS[hours]);
        strcpy(line3, "o'clock");
        break;
    case 4:
        strcpy(line2, "5 past");
        strcpy(line3, HOURS[hours]);
        break;
    case 5:
        strcpy(line1, "five");
        strcpy(line2, "past");
        strcpy(line3, HOURS[hours]);
        break;
    case 6:
    case 7:
    case 8:
        strcpy(line2, "5 past");
        strcpy(line3, HOURS[hours]);
        break;
    case 9:
        strcpy(line2, "10 past");
        strcpy(line3, HOURS[hours]);
        break;
    case 10:
        strcpy(line1, "ten");
        strcpy(line2, "past");
        strcpy(line3, HOURS[hours]);
        break;
    case 11:
    case 12:
    case 13:
        strcpy(line2, "10 past");
        strcpy(line3, HOURS[hours]);
        break;
    case 14:
    case 15:
    case 16:
        strcpy(line1, "quarter");
        strcpy(line2, "past");
        strcpy(line3, HOURS[hours]);
        break;
    case 17:
    case 18:
        strcpy(line1, "gone");
        strcpy(line2, "1/4 past");
        strcpy(line3, HOURS[hours]);
        break;      
    case 19:
        strcpy(line2, "20 past");
        strcpy(line3, HOURS[hours]);
        break;
    case 20:
        strcpy(line1, "twenty");
        strcpy(line2, "past");
        strcpy(line3, HOURS[hours]);
        break;
    case 21:
    case 22:
    case 23:
        strcpy(line2, "20 past");
        strcpy(line3, HOURS[hours]);
        break;
    case 24:
        strcpy(line2, "25 past");
        strcpy(line3, HOURS[hours]);
        break;
    case 25:
        strcpy(line1, "25");
        strcpy(line2, "past");
        strcpy(line3, HOURS[hours]);
        break;
    case 26:
    case 27:
    case 28:
        strcpy(line2, "25 past");
        strcpy(line3, HOURS[hours]);
        break;
    case 29:
    case 30:
    case 31:
        strcpy(line1, "half");
        strcpy(line2, "past");
        strcpy(line3, HOURS[hours]);
        break;
    case 32:
    case 33:
        strcpy(line1, "gone");
        strcpy(line2, "half past");
        strcpy(line3, HOURS[hours]);      
    case 34:
        strcpy(line2, "25 to");
        strcpy(line3, HOURS[hours]);
        break;
    case 35:
        strcpy(line1, "25");
        strcpy(line2, "to");
        strcpy(line3, HOURS[hours]);
        break;
    case 36:
    case 37:
    case 38:
        strcpy(line1, "gone");
        strcpy(line2, "25 to");
        strcpy(line3, HOURS[hours]);
        break;
    case 39:
        strcpy(line2, "20 to");
        strcpy(line3, HOURS[hours]);
        break;
    case 40:
        strcpy(line1, "twenty");
        strcpy(line2, "to");
        strcpy(line3, HOURS[hours]);
        break;
    case 41:
    case 42:
    case 43:
        strcpy(line2, "20 to");
        strcpy(line3, HOURS[hours]);
        break;
    case 44:
    case 45:
    case 46:
        strcpy(line1, "quarter");
        strcpy(line2, "to");
        strcpy(line3, HOURS[hours]);
        break;
    case 47:
    case 48:
        strcpy(line1, "gone");
        strcpy(line2, "1/4 to");
        strcpy(line3, HOURS[hours]);      
    case 49:
        strcpy(line2, "10 to");
        strcpy(line3, HOURS[hours]);
        break;
    case 50:
        strcpy(line1, "ten");
        strcpy(line2, "to");
        strcpy(line3, HOURS[hours]);
        break;
    case 51:
    case 52:
    case 53:
        strcpy(line2, "10 to");
        strcpy(line3, HOURS[hours]);
        break;
    case 54:
        strcpy(line2, "5 to");
        strcpy(line3, HOURS[hours]);
        break;
    case 55:
        strcpy(line1, "five");
        strcpy(line2, "to");
        strcpy(line3, HOURS[hours]);
        break;
    case 56:
    case 57:
    case 58:
        strcpy(line2, "5 to");
        strcpy(line3, HOURS[hours]);
        break;
    case 59:
        strcpy(line2, HOURS[hours]);
        strcpy(line3, "o'clock");
        break;
    }
}
