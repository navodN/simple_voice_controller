import speech_recognition as sr
import requests

# Adafruit IO details
AIO_USERNAME = "YOUR_AIO_USERNAME"
AIO_KEY = "YOUR_AIO_KEY"
FEED_NAME = "YOUR_FEED_NAME"

FEED_URL = f"https://io.adafruit.com/api/v2/{AIO_USERNAME}/feeds/{FEED_NAME}/data"

def send_command(value):
    data = {"value": value}
    response = requests.post(FEED_URL, json=data, headers={"X-AIO-Key": AIO_KEY})
    print("Sent:", value, "| Status:", response.status_code)

recognizer = sr.Recognizer()
mic = sr.Microphone()

print("🎤 Say 'turn on light' or 'turn off light'")

while True:
    try:
        with mic as source:
            audio = recognizer.listen(source, phrase_time_limit=5)

        command = recognizer.recognize_google(audio, show_all=False).lower()
        print("Heard:", command)

        if "turn on light" in command:
            send_command("ON")
        elif "turn off light" in command:
            send_command("OFF")

    except Exception as e:
        print("Error:", e)
