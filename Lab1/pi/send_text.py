import sys
import os
from twilio.rest import Client
from TwilioCreds import TwilioCreds

def send_message(message_body, to_phone_number):

    print(TwilioCreds().sid, TwilioCreds().auth_token)
    
    client = Client(TwilioCreds().sid, TwilioCreds().auth_token)

    message = client.messages.create(
        body=message_body,
        from_='+12054984327',
        to=to_phone_number
        )
    
    print(message.sid)

if __name__ == '__main__':
    send_message('test', '+16307404172')
