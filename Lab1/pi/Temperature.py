from firebase_admin import firestore
from datetime import datetime


class Temperature(object):
    def __init__(self, temp):
        self._temperature = temp

    @property
    def temp(self):
        return self._temperature

    @temp.setter
    def temp(self, new_temp):
        self._temperature = new_temp

    def to_dict(self, firestore_timestamp=False):
        data = {"temp": self._temperature}
        if firestore_timestamp:
            data["time"] = firestore.SERVER_TIMESTAMP
        else:
            data["time"] = str(datetime.now())
        return data

    def __repr__(self):
        return u"Temperature(temp={0})".format(self._temperature)
