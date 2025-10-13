# LCD (GxEPD) wrapper — usage

This folder contains the lightweight `riego::lcd::Lcd` wrapper used across the irrigation project. The wrapper centralizes display setup and simple drawing primitives so the rest of the codebase can be display-agnostic.

Quick usage example

```cpp
#include "context.h" // or include the lcd header directly

// global instance (declared in context.h)
extern riego::lcd::Lcd customLcd;

void setup() {
    customLcd.setup();
    customLcd.clear();
    customLcd.text(0, 10, "Irrigation v1.0", GxEPD_BLACK);
    customLcd.refresh(); // full update
}

void loop() {
    // draw updates and use partial refresh for small changes
    customLcd.fillRectangle(0, 40, 80, 20, GxEPD_BLACK);
    customLcd.text(2, 42, "Valve 1: ON", GxEPD_WHITE);
    customLcd.refresh(0, 40, 80, 20); // partial update
}
```

Notes
- The wrapper uses `GxEPD` under the hood. For partial updates to work reliably, consult the display's datasheet and the GxEPD library documentation.
- Pin mapping used by the display (DC, RST, BUSY, CLK, MOSI, CS) are expected to be defined in `pinout.h`.
- Rotation, SPI frequency and refresh delays are configurable in `lcd.hpp` under `riego::lcd::config`.
