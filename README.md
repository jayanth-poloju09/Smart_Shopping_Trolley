A DIY, offline self-checkout trolley that uses RFID and Arduino to automate billing—ideal for rapid prototyping, smart retail concepts, and embedded innovation.

What It Does:
1. Scans RFID-tagged items in a shopping trolley
2. Calculates and displays item prices and total on LCD
3. Uses two buttons:
Confirm → Finalizes and resets bill
Remove → Deletes last scanned item
4. No cloud, no internet—fully offline system
5. 3-second delay to avoid repeated scans
6. Powered by USB (power bank or battery)

Hardware:
1. Arduino Nano
2. MFRC-522 RFID Reader
3. 16x2 LCD with I2C module
4. Piezo Buzzer
5. 2 Push Buttons
6. Power Bank or 9V Battery

Pin Configuration
  Component	Arduino Nano Pin
1. RFID:       (SDA)	D10
2. RFID:       (SCK)	D13
3. RFID:       (MOSI)	D11
4. RFID:       (MISO)	D12
5. RFID:       (RST)	D9
6. LCD:        (SDA)	A4
7. LCD:        (SCL)	A5
8. Buzzer:	    D3
9. C_Button :	  D4
10. R_Button:   D5

How It Works
1. RFID reader fetches 16-bit UID from card
2. UID is converted to integer → used as item price
3. LCD shows item price + running total
4. Buzzer beeps after each scan/action
5. Confirm resets total; Remove subtracts last item

Setup Guide:
1. Download or clone this repo
2. Open .ino file in Arduino IDE
3. Install required libraries: MFRC522, LiquidCrystal_I2C
4. Upload code to Arduino Nano
5. Power the system and test with RFID cards

Use Cases:

Smart shopping carts,
DIY billing systems,
RFID learning modules,
Arduino project exhibitions
