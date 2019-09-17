#!/usr/bin/env python
# Alexander Powers (alexander-powers@uiowa.edu)

import sys
import firebase_admin
from firebase_admin import credentials
from firebase_admin import firestore

try:
    assert len(sys.argv) == 2
except AssertionError as e:
    raise ValueError(
        "This script takes one parameter, the collection to delete data from"
    )
collection_name = sys.argv[1]
print("Deleting: {0}".format(collection_name))

cred = credentials.Certificate("./lab1-firebase-admin-sdk-key.json")
firebase_admin.initialize_app(cred)

db = firestore.client()


def delete_collection(coll_ref, batch_size):
    docs = coll_ref.limit(batch_size).get()
    deleted = 0

    for doc in docs:
        print(u"Deleting doc {} => {}".format(doc.id, doc.to_dict()))
        doc.reference.delete()
        deleted = deleted + 1

    if deleted >= batch_size:
        return delete_collection(coll_ref, batch_size)


BATCH_SIZE = 1000
delete_collection(db.collection(collection_name), BATCH_SIZE)

