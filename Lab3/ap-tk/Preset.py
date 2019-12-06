from Lab3Time import Lab3Time


class Preset(Lab3Time):
    def __init__(self,
                 hour,
                 minute,
                 is_am,
                 is_weekend,
                 temperature=72,
                 *args,
                 **kwargs):
        Lab3Time.__init__(self, hour, minute, is_am, is_weekend, *args,
                          **kwargs)

        # declare global constants
        self.MIN_TEMP = 60
        self.MAX_TEMP = 90
        # set up variables
        self._temp = None
        # initialize state
        self.set_temp(temperature)
        self.set_inactive()

    def __str__(self):
        am_pm = 'am' if self.is_am() else 'pm'
        return f"{self._temp} degrees @ {Lab3Time.__str__(self)}"

    def __repr__(self):
        return self.__str__(self)

    # Temp
    def set_temp(self, temp):
        self._temp = temp

    def get_temp(self):
        return self._temp

    # Active
    def set_active(self):
        self._active = True

    def set_inactive(self):
        self._active = False

    def is_active(self):
        return self._active
