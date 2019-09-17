#!/usr/bin/env python
# Alexander Powers (alexander-powers@uiowa.edu)

from random import random
import firebase_admin
from firebase_admin import credentials
from firebase_admin import firestore
from time import sleep
from Temperature import Temperature

cred = credentials.Certificate("./lab1-firebase-admin-sdk-key.json")
firebase_admin.initialize_app(cred)

db = firestore.client()
temps = db.collection(u"temps")

offset = 15
temp_range = 30

print("connection test... this may take a moment")
count = 0
for doc in temps.stream():
    count += 1
print("{0} documents already exist.".format(count))
print("Writing a new data point every second.")

while True:
    new_doc = temps.document()
    new_temp = Temperature(random() * temp_range + offset)
    new_doc.set(new_temp.to_dict(firestore_timestamp=True))
    print("Saved: {0}".format(str(new_temp)))
    sleep(1)

