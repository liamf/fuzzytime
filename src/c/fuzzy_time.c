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
// 5:01-02   gone      five      o'clock
// 5:03      nearly    5 past    five
// 5:04      almost    5 past    five
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

    // after half past the hour we are "to" the next hour
    if (minutes >= 33) {
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
        strcpy(line2, HOURS[hours]);
        strcpy(line3, "o'clock");
        break;
    case 3:
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
        strcpy(line2, "5 past");
        strcpy(line3, HOURS[hours]);
        break;
    case 8:
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
        strcpy(line2, "10 past");
        strcpy(line3, HOURS[hours]);
        break;
    case 13:
    case 14:
        strcpy(line2, "1/4 past");
        strcpy(line3, HOURS[hours]);
        break;     
    case 15:
        strcpy(line1, "quarter");
        strcpy(line2, "past");
        strcpy(line3, HOURS[hours]);
        break;
    case 16:
    case 17:
        strcpy(line1, "gone");
        strcpy(line2, "1/4 past");
        strcpy(line3, HOURS[hours]);
        break;      
    case 18:
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
        strcpy(line2, "20 past");
        strcpy(line3, HOURS[hours]);
        break;
    case 23:
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
        strcpy(line2, "25 past");
        strcpy(line3, HOURS[hours]);
        break;
    case 28:
    case 29:
        strcpy(line2, "1/2 past");
        strcpy(line3, HOURS[hours]);
        break;
    case 30:  
        strcpy(line1, "half");
        strcpy(line2, "past");
        strcpy(line3, HOURS[hours]);
        break;
    case 31:
    case 32:
        strcpy(line1, "gone");
        strcpy(line2, "half past");
        strcpy(line3, HOURS[hours]);      
        break;
    case 33:
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
        strcpy(line1, "gone");
        strcpy(line2, "25 to");
        strcpy(line3, HOURS[hours]);
        break;
    case 38:
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
        strcpy(line2, "20 to");
        strcpy(line3, HOURS[hours]);
        break;
    case 43:
    case 44:
        strcpy(line2, "1/4 to");
        strcpy(line3, HOURS[hours]);
        break;      
    case 45:
        strcpy(line1, "quarter");
        strcpy(line2, "to");
        strcpy(line3, HOURS[hours]);
        break;
    case 46:
    case 47:
        strcpy(line1, "gone");
        strcpy(line2, "1/4 to");
        strcpy(line3, HOURS[hours]);   
        break;
    case 48:
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
        strcpy(line2, "10 to");
        strcpy(line3, HOURS[hours]);
        break;
    case 53:
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
        strcpy(line2, "5 to");
        strcpy(line3, HOURS[hours]);
        break;
    case 58:
    case 59:
        strcpy(line2, HOURS[hours]);
        strcpy(line3, "o'clock");
        break;
    }
}
