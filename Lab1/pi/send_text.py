import sys
import os
from dotenv import load_dotenv
from twilio.rest import Client

load_dotenv()

def send_message(message_body, to_phone_number):
    account_sid = os.getenv('TWILIO_ACCOUNT_SID')
    auth_token = os.getenv('TWILIO_AUTH_TOKEN')
    client = Client(account_sid, auth_token)

    message = client.messages.create(
        body=message_body,
        from_='+12054984327',
        to=to_phone_number
        )
    
    print(message.sid)

if __name__ == '__main__':
    send_message(sys.argv[1], sys.argv[2])
