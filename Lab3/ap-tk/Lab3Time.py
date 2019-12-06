class Lab3Time(object):
    def __init__(self, hour, minute, is_am, is_weekend, *args, **kwargs):
        # set up variables
        self._is_am = None
        self._is_weekday = None
        self._active = None
        self._hour = None
        self._minute = None
        # initialize state
        self.set_weekend() if is_weekend else self.set_weekday()
        self.set_am() if is_am else self.set_pm()
        self.set_minute(minute)
        self.set_hour(hour)

    def __str__(self):
        am_pm = 'am' if self.is_am() else 'pm'
        return f"{self._hour}:{self._minute} {am_pm}"

    def __repr__(self):
        return self.__str__()

    # Hours
    def set_hour(self, hour):
        if hour < 1 or 12 < hour:
            raise ValueError('Hour must be between 1-12')
        self._hour = hour

    def get_hour(self):
        return self._hour

    # Minutes
    def set_minute(self, minute):
        if minute < 0 or 60 < minute:
            raise ValueError('Minute must be between 0-60')
        self._minute = minute

    def get_minute(self):
        return self._minute

    # AM PM
    def set_am(self):
        self._is_am = True

    def set_pm(self):
        self._is_am = False

    def is_am(self):
        return self._is_am

    def is_pm(self):
        return not self._is_am

    # Weekend Weekday
    def set_weekend(self):
        self._is_weekday = False

    def set_weekday(self):
        self._is_weekday = True

    def is_weekday(self):
        return self._is_weekday

    def is_weekend(self):
        return not self._is_weekday
