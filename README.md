# ESP-IDF examples

This is a set of ESP-IDF examples to help you learn to program an ESP32.

> **Note:** Some examples are AI-generated. However, every single one has been manually verified, tested, and refined. All examples use ESP-IDF v6.

## Running these examples in your ESP32

### 1. Install the necessary dependencies

1. Install ESP-IDF (and the drivers if using Windows) using EIM ([official docs](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html#installation)).
2. Install the ESP-IDF VS Code extension by Espressif Systems.

### 2. Configure, build & flash

1. `Ctrl + Shift + P -> ESP-IDF: Run idf.py reconfigure Task`
2. `Ctrl + Shift + P -> ESP-IDF: Build Your Project`
3. `Ctrl + Shift + P -> ESP-IDF: Flash (UART) Your Project`

#### A Note for Windows users (skip this if you use Linux)

If you are on Windows, flashing might show this error:

```plaintext
A fatal error occurred: Could not connect to an Espressif device on any of the 1 available serial ports.
```

If that happens, the next time you try, press and hold the BOOT button on your board as soon as
you see `Connecting...`. Only stop pressing after a few seconds.

#### A Note for Linux users (skip this if you use Windows)

You might get this error while flashing:

```
A fatal error occurred: Could not open /dev/ttyUSB0, the port is busy or doesn't exist.
([Errno 13] could not open port /dev/ttyUSB0: [Errno 13] Permission denied: '/dev/ttyUSB0')

Hint: Try to add user into dialout or uucp group.
```

If that happens, run this command, log out, log back in (or reboot), and try flashing again:

```bash
sudo usermod -aG dialout $USER
```

You have to do it only once.

## Creating a new example

- `Ctrl + Shift + P -> ESP-IDF: New Project -> <your_esp_idf_version>`
- `ESP-IDF Templates -> sample_project -> Create Project (button)`
- Fill in the details:
  - Project name: Your project name.
  - Project directory: Your project directory.
  - ESP-IDF target: esp32.
  - ESP-IDF board: Custom board.
  - Serial port: Detect.
  - OpenOCD configuration files: Keep the default value.
  - ESP-IDF component directory: Keep the input empty.
- Create project.
- Open project.
- If you are prompted to generate `compile_commands.json`, accept it. If not, do `Ctrl + Shift + P -> ESP-IDF: Run idf.py reconfigure Task`, which does the same thing.
- If you are using Clangd in VS Code or its forks, run `ESP-IDF: Configure project for ESP-Clang` from the VS Code command palette to make sure Clangd IntelliSense works correctly.

## Troubleshooting

### Red squiggly lines under `#include "something"`

Try both. At least one of them will probably work.

- Option 1: `Ctrl + Shift + P -> ESP-IDF: Run idf.py reconfigure Task`
- Option 2: `Ctrl + Shift + P -> Add VS Code Configuration Folder`

### Reconfigure or build failure

Delete the `build` directory and try again.

## Tips

### Using `idf.py`

`Ctrl + Shift + P -> Open ESP-IDF Terminal`. You can use `idf.py` in this terminal (e.g., `idf.py reconfigure`).
