# C++ SGG Asteroids Arcade

A modern C++ implementation of the classic Asteroids arcade game using the Simple Game Graphics (SGG) library.

![Asteroids Game Screenshot](https://raw.githubusercontent.com/loukas-vetoulis/C-SGG-Asteroids-Arcade/main/screenshots/gameplay.png)

## Description

This project is a faithful recreation of the iconic Asteroids arcade game originally released by Atari in 1979. The game puts you in control of a spaceship navigating through an asteroid field, where you must shoot and destroy asteroids while avoiding collisions.

## Features

- Classic Asteroids gameplay with modern C++ implementation
- Smooth controls and physics-based movement
- Multiple levels with increasing difficulty
- Score tracking and high score system
- Sound effects and visual effects
- SGG library for graphics rendering

## Prerequisites

Before you begin, ensure you have the following requirements:

- Microsoft Visual Studio 2019 or newer
- Microsoft Visual C++ Redistributable for Visual Studio 2019 or newer
- SGG library (included with the project)

## Installation

1. Clone the repository:
   ```
   git clone https://github.com/loukas-vetoulis/C-SGG-Asteroids-Arcade.git
   ```

2. Open the solution file in Visual Studio.

3. Build the solution in Release mode.

4. If you encounter the error "MSVCP140D.dll was not found", you need to install the Microsoft Visual C++ Redistributable package:
   - Download from [Microsoft's official website](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist)
   - Choose the x86 or x64 version based on your system architecture

## How to Play

1. Use the arrow keys to control your spaceship:
   - Up arrow: Thrust forward
   - Left/Right arrows: Rotate the ship
   - Space: Fire bullets

2. Destroy all asteroids to advance to the next level.

3. Avoid collisions with asteroids - they will destroy your ship.

4. Watch out for occasional flying saucers that will shoot at you.


## Game Controls

| Key           | Action                  |
|---------------|-------------------------|
| Up Arrow      | Thrust forward          |
| Left Arrow    | Thrust left             |
| Right Arrow   | Thrust right            |
| MouseLeftClick| Fire weapon             |
| Mouse         | Aim                     |

## Troubleshooting

- If you encounter "Stage1.exe - System Error: MSVCP140D.dll was not found", install the Microsoft Visual C++ Redistributable package as mentioned in the Installation section.
- If the game runs slowly, try running in Release mode instead of Debug.
- Make sure your graphics drivers are up to date.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Acknowledgements

- Based on the original Asteroids game by Atari (1979)
- SGG library for graphics rendering
- Inspiration and specific implementations from various asteroid game tutorials

## Contact

Loukas Vetoulis - [GitHub Profile](https://github.com/loukas-vetoulis)

Project Link: [https://github.com/loukas-vetoulis/C-SGG-Asteroids-Arcade](https://github.com/loukas-vetoulis/C-SGG-Asteroids-Arcade)
