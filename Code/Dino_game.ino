#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Custom characters for the game
byte dino[8] = {
  0b00100,
  0b01110,
  0b01110,
  0b00100,
  0b01110,
  0b10100,
  0b10100,
  0b01100
};

byte obstacle[8] = {
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b00000,
  0b01110,
  0b01110
};

const int buttonPin = 2;
bool isJumping = false;
int dinoPosition = 1;
int obstaclePosition = 15;
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 300;
int Score =0;
void setup() {
  lcd.init();                      // initialize the lcd
  lcd.backlight();                 // turn on the backlight
  delay(3000);
  lcd.print("DINO GAME");
  lcd.setCursor(10, 0);
  lcd.write(byte(0));
  delay(2000);
  lcd.setCursor(0,1);
  lcd.print("Made with HPDJOY");
  delay(2000);
  // Create custom characters
  lcd.createChar(0, dino);
  lcd.createChar(1, obstacle);

  pinMode(buttonPin, INPUT_PULLUP); // Set button pin as input with pull-up resistor
}

void loop() {
  if (millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();

    // Check for button press to make the dino jump
    if (digitalRead(buttonPin) == LOW) {
      isJumping = true;
      dinoPosition = 0;
    }

    // Update dino position
    if (isJumping) {
      dinoPosition = 0;
      isJumping = false;
    } else {
      dinoPosition = 1;
    }

    // Update obstacle position
    obstaclePosition--;
    if (obstaclePosition < 0) {
      obstaclePosition = 15;
      Score++;
    }

    // Clear the screen and draw the new positions
    lcd.clear();
    lcd.setCursor(0, dinoPosition);
    lcd.write(byte(0));
    lcd.setCursor(obstaclePosition, 1);
    lcd.write(byte(1));

    // Check for collision
    if (obstaclePosition == 0 && dinoPosition == 1) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Game Over!");
      lcd.setCursor(0, 1);
      lcd.print("Score");
      lcd.setCursor(7,1);
      lcd.print(Score);
      while (1) {} // Stop the game
    }
  }
}
