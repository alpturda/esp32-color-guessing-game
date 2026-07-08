# ESP32 Color Guessing Game

ESP32 Color Guessing Game is an embedded game project developed using an ESP32 microcontroller, an ST7789 TFT display, and a rotary encoder.

The game is inspired by code-breaking logic games. The player tries to guess a hidden 4-color code selected from an 8-color palette. Each guess is displayed on the TFT screen, and the system provides visual feedback for correct colors and correct positions.

## Features

- ESP32 based embedded game system
- ST7789 240x320 TFT display interface
- Rotary encoder based user input
- 8-color selectable palette
- 4-slot hidden color code
- Maximum 8 attempts
- Guess history displayed on screen
- Visual feedback system
- State machine based game flow

## Hardware Used

- ESP32 development board
- ST7789 240x320 TFT display
- Rotary encoder with push button
- Push button for back/delete function
- Jumper wires
- Breadboard or prototype board

## Pin Connections

| Component | ESP32 Pin |
|---|---|
| TFT CS | GPIO 5 |
| TFT DC | GPIO 2 |
| TFT RST | GPIO 4 |
| Encoder A | GPIO 32 |
| Encoder B | GPIO 33 |
| Encoder Push | GPIO 25 |
| Back Button | GPIO 26 |

## Game Logic

The system generates a random 4-color secret code at the beginning of each game. The player selects colors using the rotary encoder and confirms each selection by pressing the encoder button.

After each 4-color guess, the system compares the guess with the secret code and gives feedback:

- Green: Correct color and correct position
- Yellow: Correct color but wrong position
- White/Empty: No match

The player wins if all 4 colors are guessed in the correct positions within 8 attempts. If the player cannot find the correct code within 8 attempts, the secret code is shown on the screen.

## Technologies Used

- ESP32
- Arduino IDE
- C/C++
- Adafruit GFX Library
- Adafruit ST7789 Library
- SPI Communication
- Rotary Encoder Input
- Embedded UI Design

## Project Structure

```text
esp32-color-guessing-game
├── README.md
├── LICENSE
└── src
    └── esp32-color-guessing-game
        └── esp32-color-guessing-game.ino
