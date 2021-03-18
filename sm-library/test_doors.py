import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

frontdoorPin = 17 # Se conecta a GPIO 18
backdoorPin = 27 # Se conecta a GPIO 23
bathroomPin = 22 # Se conecta a GPIO 24
room1doorPin = 5 # Se conecta a GPIO 25
room2doorPin = 6 # Se conecta a GPIO 16

GPIO.setup(frontdoorPin, GPIO.OUT)
GPIO.setup(backdoorPin, GPIO.OUT)
GPIO.setup(bathroomPin, GPIO.OUT)
GPIO.setup(room1doorPin, GPIO.OUT)
GPIO.setup(room2doorPin, GPIO.OUT)

frontdoorStatus = False;
backdoorStatus = False;
bathroomStatus = False;
room1doorStatus = False;
room2doorStatus = False;

GPIO.output(frontdoorPin, False)
GPIO.output(backdoorPin, False)
GPIO.output(bathroomPin, False)
GPIO.output(room1doorPin, False)
GPIO.output(room2doorPin, False)

while True:
    num = input("Ingrese una tecla: ")
    if(num == 1):
        frontdoorStatus = not frontdoorStatus
        GPIO.output(frontdoorPin, frontdoorStatus)
        print("Puerta delantera: " + str(frontdoorStatus) + "\n")
    if(num == 2):
        backdoorStatus = not backdoorStatus
        GPIO.output(backdoorPin, backdoorStatus)
        print("Puerta trasera: " + str(backdoorStatus) + "\n")
    if(num == 3):
        bathroomStatus = not bathroomStatus
        GPIO.output(bathroomPin, bathroomStatus)
        print("Puerta bano: " + str(bathroomStatus) + "\n")
    if(num == 4):
        room1doorStatus = not room1doorStatus
        GPIO.output(room1doorPin, room1doorStatus)
        print("Puerta cuarto1: " + str(room1doorStatus) + "\n")
    if(num == 5):
        room2doorStatus = not room2doorStatus
        GPIO.output(room2doorPin, room2doorStatus)
        print("Puerta cuarto2: " + str(room2doorStatus) + "\n")
    
