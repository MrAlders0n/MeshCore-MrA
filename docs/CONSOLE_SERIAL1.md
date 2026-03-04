# CONSOLE_USE_SERIAL1 — UART Console on Ikoka Stick

On the Ikoka Stick (XIAO nRF52840), Serial1 (UART) and I2C share pins D6/D7.
This feature redirects the repeater console from USB Serial to UART Serial1,
allowing a headless setup (e.g. connected to a Raspberry Pi).

## Build

```bash
PLATFORMIO_BUILD_FLAGS="-D CONSOLE_USE_SERIAL1 -D MESH_PACKET_LOGGING=1" \
  pio run -e ikoka_stick_nrf_30dbm_repeater
```

The flag can be used with any `ikoka_stick_nrf_*_repeater` environment.

## Wiring (Ikoka Stick to Raspberry Pi)

| Ikoka Stick | Pi GPIO       |
|-------------|---------------|
| D6 (TX)     | GPIO 15 (RX)  |
| D7 (RX)     | GPIO 14 (TX)  |
| GND         | GND           |

Baud rate: **115200**

Make sure the Pi's serial login shell is disabled (`raspi-config` > Interface Options > Serial Port > No to login shell, Yes to hardware).

## What gets disabled

Since D6/D7 are shared between UART and I2C, enabling `CONSOLE_USE_SERIAL1`
automatically disables all I2C peripherals on those pins:

- **OLED display** — `DISPLAY_CLASS` is undefined at compile time
- **I2C sensors** — `sensors.begin()` is skipped (probing Wire without init causes a hard fault)
- **Hardware RTC** — `rtc_clock.begin(Wire)` is skipped (volatile fallback clock is used)
- **Wire bus** — `Wire.begin()` is skipped entirely

The LoRa radio, filesystem, and all non-I2C functionality remain fully operational.

## Files modified

| File | Change |
|------|--------|
| `examples/simple_repeater/ConsoleSerial.h` | `#undef DISPLAY_CLASS` when Serial1 is active |
| `variants/ikoka_stick_nrf/IkokaStickNRFBoard.cpp` | Skip `Wire.setPins()` / `Wire.begin()` |
| `variants/ikoka_stick_nrf/target.cpp` | Skip `rtc_clock.begin(Wire)` |
| `examples/simple_repeater/main.cpp` | Skip `sensors.begin()` |

All changes are guarded by `#ifndef CONSOLE_USE_SERIAL1` — without the flag,
the build is identical to upstream.

## Rebasing notes

When rebasing onto upstream, watch for:

- **`main.cpp`** — most likely to conflict since it's shared across all boards
- New I2C usage in the Ikoka Stick code path that isn't guarded — would cause
  a hard fault when `CONSOLE_USE_SERIAL1` is active
