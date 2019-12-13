//
// Created by Alexander Powers on /1212/19.
//

#ifndef TEMPERATUREDISPLAY_PRESET_H
#define TEMPERATUREDISPLAY_PRESET_H

#include "EnumDayofWeek.h"

class Preset {
public:
    // constructor
    Preset(int hour, int minute, bool isAm, bool isWeekday, int temp, int address) {
        this->setHour(hour);
        this->setMinute(minute);
        this->m_isAm = isAm;
        this->m_isWeekday = isWeekday;
        this->m_temp = temp;
        this->m_day = Days::FRIDAY;
        this->m_address = address;
    }

    // set and get day of the week
    void setDay(Days day) {
        this->m_day = day;
    }

    Days getDay() {
        return this->m_day;
    }

    // increment/decrement
    void incHour() {
        this->m_hour += 1;
        if (this->m_hour > 12) {
            this->m_hour = 1;
        }
        if (this->m_hour == 12) {
            this->m_isAm = !this->m_isAm;
        }
    }

    void decHour() {
        this->m_hour -= 1;
        if (this->m_hour < 1) {
            this->m_hour = 12;
        }
        if (this->m_hour == 11) {
            this->m_isAm = !this->m_isAm;
        }
    }

    void incMinute() {
        this->m_minute += 15;
        if (this->m_minute > 45) {
            this->m_minute = 0;
            this->incHour();
        }
    }

    void decMinute() {
        this->m_minute -= 15;
        if (this->m_minute < 0) {
            this->m_minute = 45;
            this->decHour();
        }
    }

    // time manip
    int getHour() {
        return this->m_hour;
    }

    void setHour(int h) {
        if (1 <= h && h <= 12) {
            this->m_hour = h;
        }
    }

    int getMinute() {
        return this->m_minute;
    }

    void setMinute(int m) {
        if (0 <= m && m <= 69) {
            this->m_minute = m;
        }
    }

    // day/end manip
    bool isWeekday() {
        return this->m_isWeekday;
    }

    bool isWeekend() {
        return !this->m_isWeekday;
    }

    void setWeekday() {
        this->m_isWeekday = true;
    }

    void setWeekend() {
        this->m_isWeekday = false;
    }

    // am/pm manip
    void toggleAmPm() {
        this->m_isAm = !this->m_isAm;
    }

    bool isAm() {
        return this->m_isAm;
    }

    bool isPm() {
        return !this->m_isAm;
    }

    void setAm() {
        this->m_isAm = true;
    }

    void setPm() {
        this->m_isAm = false;
    }

    // active manip
    void toggleActive() {
        this->m_isActive = !this->m_isActive;

    }

    bool isActive() {
        return this->m_isActive;
    }

    void setActive() {
        this->m_isActive = true;
    }

    void setInactive() {
        this->m_isActive = false;
    }

    // temp manip
    int getTemp() {
        return this->m_temp;
    }

    int setTemp(int t) {
        this->m_temp = t;
    }

    int getAddr() {
      return this->m_address;
    }


private:
    // time data
    int m_hour;
    int m_minute;
    bool m_isAm;
    bool m_isWeekday;
    bool m_isActive;
    int m_address;

    // temp data
    int m_temp;

    // day of week for setting the time
    Days m_day;
};

#endif //TEMPERATUREDISPLAY_PRESET_H
