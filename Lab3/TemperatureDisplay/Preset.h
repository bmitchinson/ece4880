//
// Created by Alexander Powers on /1212/19.
//

#ifndef TEMPERATUREDISPLAY_PRESET_H
#define TEMPERATUREDISPLAY_PRESET_H

class Preset {
public:
    // constructor
    Preset(int hour, int minute, bool isAm, bool isWeekday, int temp) {
        this->setHour(hour);
        this->setMinute(minute);
        this->m_isAm = isAm;
        this->m_isWeekday = isWeekday;
        this->m_temp = temp;
    }

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

    // am/pm manip
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
    bool isActive() {
        return this->m_isActive;
    }

    void setActive() {
        this->m_isActive = true;
    }

    void setInactive() {
        this->m_isActive = false;
    }


private:
    // time data
    int m_hour;
    int m_minute;
    bool m_isAm;
    bool m_isWeekday;
    bool m_isActive;

    // temp data
    int m_temp;
};

#endif //TEMPERATUREDISPLAY_PRESET_H
