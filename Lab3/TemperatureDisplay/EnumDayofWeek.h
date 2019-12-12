//
// Created by Alexander Powers on /1212/19.
//

#ifndef TEMPERATUREDISPLAY_ENUMDAYOFWEEK_H
#define TEMPERATUREDISPLAY_ENUMDAYOFWEEK_H

namespace DaysOfWeek {
    enum Days {
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        SUNDAY
    };

    static const DaysOfWeek::Days Week[] = {MONDAY, TUESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY};
    static const int NUM_DAYS_IN_WEEK = 7;

    const char *ToString(DaysOfWeek d) {
        switch (d) {
            case DaysOfWeek::Days::MONDAY:
                return "MONDAY";
            case DaysOfWeek::Days::TUESDAY:
                return "TUESDAY";
            case DaysOfWeek::Days::WEDNESDAY:
                return "WEDNESDAY";
            case DaysOfWeek::Days::THURSDAY:
                return "THURSDAY";
            case DaysOfWeek::Days::FRIDAY:
                return "FRIDAY";
            case DaysOfWeek::Days::SATURDAY:
                return "SATURDAY";
            case DaysOfWeek::Days::SUNDAY:
                return "SUNDAY";
        }
    }
}


#endif //TEMPERATUREDISPLAY_ENUMDAYOFWEEK_H
