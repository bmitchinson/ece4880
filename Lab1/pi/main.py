#!/usr/bin/env python
# Alexander Powers (alexander-powers@uiowa.edu)

from time import sleep

# thermometer library
from w1thermsensor import W1ThermSensor

# database library
import firebase_admin
from firebase_admin import credentials
from firebase_admin import firestore

# custom Temperature class
from Temperature import Temperature

# LETS MULTI-PROCESS THIS BITCH
from multiprocessing import Manager, Process

cred = credentials.Certificate("./lab1-firebase-admin-sdk-key.json")
firebase_admin.initialize_app(cred)
SLEEP_DURATION = 1


def button_is_pressed():
    # replace this with some logic that determines if the button is pressed
    return True


def switch_is_on():
    # replace this with some logic that determines if the switch is on
    return True


def read_and_send_temp_process(state):
    db = firestore.client()
    temps = db.collection(u"temps")
    sensor = W1ThermSensor()
    while True:
        sleep(SLEEP_DURATION)
        send_temps = (
            switch_is_on()
        )  # && some logic from firestore get this variable from the current state
        if send_temps:
            new_doc = temps.document()
            current_temp = senesor.get_temperature
            state["current_temp"] = current_temp
            new_temp = Temperature(current_temp)
            new_doc.set(new_temp.to_dict(firestore_timestamp=True))
            print("Saved: {0}, to firestore".format(str(new_temp)))


def update_state_process(state):
    db = firestore.client()
    prefs = db.collection(u"preferences")
    while True:
        sleep(SLEEP_DURATION)
        # read state from firestore
        state["b"] = not state["b"]


def update_display_process(state):
    while True:
        sleep(SLEEP_DURATION)
        print("idk what to do here yet")
        current_temp = state["current_temp"]
        # display the current temperature


def main():
    with Manager() as manager:
        state_dict = manager.dict()
        # Read initial state from firestore
        db = firestore.client()
        prefs = db.collection(u"preferences")
        state_dict["state"] = {}  # put state here
        state_dict["b"] = True
        state_dict["current_temp"] = 5

        try:
            processes = [
                Process(target=read_and_send_temp_process, args=(state_dict,)),
                Process(target=update_state_process, args=(state_dict,)),
                Process(target=update_display_process, args=(state_dict,)),
            ]
            [p.start() for p in processes]
            [p.join() for p in processes]
        finally:
            print("BYE BYE")
            [p.join() for p in processes]


if __name__ == "__main__":
    main()

