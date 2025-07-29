#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10   // SDA Pin for RFID
#define RST_PIN 9   // Reset Pin for RFID
#define BUZZER_PIN 3  // Buzzer control pin
#define I2C_ADDR 0x27 // Default I2C address for LCD
#define CONFIRM_BUTTON 4  // Confirm button pin
#define CANCEL_BUTTON 5   // Cancel button pin

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(I2C_ADDR, 16, 2); // 16x2 LCD

int totalSum = 0;  
bool removeNextItem = false;  
bool wasItemAdded = false;  

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Scan Product:");
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(CONFIRM_BUTTON, INPUT_PULLUP);
    pinMode(CANCEL_BUTTON, INPUT_PULLUP);

    Serial.println("Place a Product:");
}

void activateBuzzer() {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(300);
}

void loop() {
    // Confirm Button: Reset Total Amount
    if (digitalRead(CONFIRM_BUTTON) == LOW) {
        confirmTransaction();
    }

    // Cancel Button: Activate Remove Mode
    if (digitalRead(CANCEL_BUTTON) == LOW) {
        removeNextItem = true;
        wasItemAdded = false;  
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Scan to remove");
        Serial.println("Waiting for item to remove...");
        delay(500);
    }

    // Check for new scanned product
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    byte block = 4;
    byte buffer[18];
    byte size = sizeof(buffer);

    MFRC522::MIFARE_Key key;
    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

    if (mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid)) != MFRC522::STATUS_OK) {
        Serial.println("Auth Failed!");
        lcd.setCursor(0, 1);
        lcd.print("Auth Failed!");
        return;
    }

    if (mfrc522.MIFARE_Read(block, buffer, &size) == MFRC522::STATUS_OK) {
        int priceValue = 0;
        String productName = "";

        for (byte i = 0; i < 16; i++) {
            char c = buffer[i];

            if (isdigit(c)) {
                priceValue = priceValue * 10 + (c - '0');
            } else if (isalpha(c) || c == ' ') {
                productName += c;
            }
        }

        if (priceValue > 0 && productName.length() > 0) {
            lcd.clear();
            lcd.setCursor(0, 0);

            if (removeNextItem) {
                if (totalSum >= priceValue) {  
                    totalSum -= priceValue;
                    Serial.print("Removed Item: ");
                    Serial.print(productName);
                    Serial.print(" Rs ");
                    Serial.println(priceValue);

                    lcd.print("Removed Item");
                    lcd.setCursor(0, 1);
                    lcd.print(productName);
                } else {  
                    Serial.print("Didn't Add Item: ");
                    Serial.println(productName);

                    lcd.print("Didn't Add Item");
                    lcd.setCursor(0, 1);
                    lcd.print(productName);
                }
                removeNextItem = false;  

            } else {
                totalSum += priceValue;
                wasItemAdded = true;  
                Serial.print("Added: ");
                Serial.print(productName);
                Serial.print(" Rs ");
                Serial.println(priceValue);

                lcd.print("Added: ");
                lcd.print(productName);
            }

            Serial.print("Total: Rs ");
            Serial.println(totalSum);

            lcd.setCursor(0, 1);
            lcd.print("Total: Rs ");
            lcd.print(totalSum);

            activateBuzzer();
        }
    } else {
        Serial.println("Read failed!");
        lcd.setCursor(0, 1);
        lcd.print("Read Failed!");
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(3000);
}

void confirmTransaction() {
    lcd.clear();
    
    if (totalSum == 0) {
        lcd.setCursor(0, 0);
        lcd.print("No items scanned");
        Serial.println("No items scanned!");
    } else {
        lcd.setCursor(0, 0);
        lcd.print("Pay At Counter");
        lcd.setCursor(0, 1);
        lcd.print("Total: Rs ");
        lcd.print(totalSum);

        Serial.println("Transaction Confirmed!");
    }

    delay(4000);
    totalSum = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scan Product:");
    Serial.println("Total reset to 0.");
}
