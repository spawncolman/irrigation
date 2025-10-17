# Irrigation Controller (ESP32-S3)

This repository contains firmware for an irrigation controller built for ESP32-S3-based development boards (tested with `esp32-s3-devkitm-1`). It uses PlatformIO and the Arduino framework with e-paper display support (GxEPD), RTC, and a small state-machine UI.

## Table of contents
- Project overview
- Hardware requirements
- Software requirements
- Supported boards
- Repository layout
- Setup: clone, dependencies and build
- Flashing and serial monitor
- Usage
- Development notes
- License

## Project overview

The irrigation controller provides:

- A small user interface on an e-paper display (GxEPD) to view and edit programs.
- Valve control using GPIOs / H-bridge with current sensing.
- RTC support for scheduled programs.
- Power-saving features (optionally powering down the e-paper after updates).

Core design goals:

- Minimal, well-documented hardware abstraction for valves, relays and sensors.
- Centralized LCD wrapper for drawing text/rectangles and performing full/partial updates.
- Modular state machine for UI screens (main menu, edit, status, time, programming).

## Hardware requirements

- ESP32-S3 development board (esp32-s3-devkitm-1 recommended).
- 1.54" e-paper display compatible with GxEPD (JD79653A / GxGDEW0154M09)
- Relay(s) or H-bridge to drive valves/actuators.
- Current sensing hardware (as wired in `pinout.h`).
- Optional RTC module if persistent time is required (project uses `makuna/RTC`).

Check `pinout.h` and `src/` for mappings and wiring expected by the firmware.

## Software requirements

- PlatformIO (recommended) — https://platformio.org/
- Python (for PlatformIO tooling) — typically Python 3.8+
- VS Code (optional) with the PlatformIO extension for a friendly IDE setup

### PlatformIO dependencies

This project declares the following libraries in `platformio.ini`:

- FASTLED (for any LED/indicator usage)
- makuna/RTC@^2.5.0 (RTC support)
- zinggjm/GxEPD@^3.1.3 (e-paper display driver)

PlatformIO will automatically download these when you build the project. If you prefer to install them manually, use:

```bash
pio lib install "FASTLED"
pio lib install "makuna/RTC@^2.5.0"
pio lib install "zinggjm/GxEPD@^3.1.3"
```

## Supported boards

See `platformio.ini`. The default environment targets:

- `esp32-s3-devkitm-1` (platform: `espressif32`, framework: `arduino`)

You can add new environments to `platformio.ini` for other ESP32 variants.

## Repository layout

- `src/` — project source code
  - `main.cpp` — application entry point and initialization
  - `context.h` — global externs and singletons
  - `lcd/` — display wrapper and fonts
  - `state/` — UI states and state machine
  - `util/` — utility modules (boot, clock, relay, valve, sensors)
- `include/` — optional headers
- `lib/` — external libraries (if present locally)
- `platformio.ini` — build configuration and library dependencies

## Setup: clone, dependencies and build

1. Clone the repository:

```bash
git clone <your-repo-url> irrigation
cd irrigation
```

2. Install PlatformIO (if you don't have it already). The easiest way is to install the PlatformIO VS Code extension or use the PlatformIO Core CLI:

```bash
# Install platformio core via pip (optional)
python3 -m pip install -U platformio
```

3. Build the project (PlatformIO will fetch libraries automatically):

```bash
pio run
```

If the build fails with include errors (`GxEPD.h`, `SPI.h`, etc.), make sure PlatformIO finished downloading libraries and your `platformio.ini` environment is selected in your editor.

## Flashing and serial monitor

To flash the firmware to the default board:

```bash
pio run -t upload
```

To open the serial monitor:

```bash
pio device monitor -b 115200
```

## Usage

1. Power up the hardware with the e-paper, relays, valves and sensors connected as in `pinout.h`.
2. The device starts, initializes peripherals and shows the main menu.
3. Use the input device (button matrix / cruceta) to navigate menus and program schedules.

Typical API for the LCD wrapper (`riego::lcd::Lcd`):

- `setup()` — initialize SPI and display
- `clear()` — clear framebuffer to white
- `text(x,y, "Hello")` — draw text
- `fillRectangle(x,y,w,h)` — draw filled rectangle
- `refresh()` — full update (optionally powers down display)
- `refresh(x,y,w,h)` — partial window update

## Development notes

- The project uses a centralized `context.h` to declare hardware singletons (LCD, relays, valves). This simplifies access across states but can be refactored to use dependency injection for better testability.
- Display rotation, delays and SPI frequency are centralized in `src/lcd/lcd.hpp::riego::lcd::config`.
- If you need to mock the e-paper display for unit tests, extract an interface around the `GxEPD_Class` usage and provide a fake implementation during tests.

## Troubleshooting

- Include errors for `GxEPD.h`, `GxIO.h` or `SPI.h` usually mean PlatformIO hasn't downloaded dependencies or the includePath in your editor isn't configured. Run `pio run` and restart your editor.
- If the display doesn't respond, double-check SPI pins in `pinout.h` and ensure power pins (GPIOs used for reset/DC/CS/BUSY) match the wiring.

## Wiring (example)

Below is a simple wiring example for the 1.54" e-paper module used in this project. Adjust pins if you use another module or board.

ESP32-S3        -> EPD
----------------------
GPIO CUSTOMLCD_CLK_PIN (SCK)  -> CLK
GPIO CUSTOMLCD_DIN_PIN (MOSI) -> MOSI / SDI
GPIO CUSTOMLCD_BUSY_PIN       -> BUSY (MISO)
GPIO CUSTOMLCD_CS_PIN        -> CS / SS
GPIO CUSTOMLCD_DC_PIN        -> DC
GPIO CUSTOMLCD_RST_PIN       -> RST
3.3V                         -> VCC
GND                          -> GND

Example pin values used in the project (see `src/lcd/seengreat.h`):

- CUSTOMLCD_DIN_PIN = 11
- CUSTOMLCD_CLK_PIN = 12
- CUSTOMLCD_BUSY_PIN = 13
- CUSTOMLCD_CS_PIN  = 10
- CUSTOMLCD_DC_PIN  = 1
- CUSTOMLCD_RST_PIN = 2

Note: Confirm voltage levels: some e-paper displays require 3.3V signals only.

## Wokwi diagram

https://wokwi.com/projects/445077297242143745

## License

This project is released under the repository `LICENSE` located at the project root.

If you want, I can also:

- Add a short hardware wiring diagram to the README.
- Add a `CONTRIBUTING.md` with coding conventions.
- Create a `lcd/README.md` with examples specific to the display wrapper.# irrigation
ESP32 project for a garden irrigation 
