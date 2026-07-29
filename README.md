# ESP-IDF examples

This is a set of ESP-IDF examples to help you learn to program an ESP32.

> **Note:** Most examples are AI-generated. However, every single one has been manually verified, tested, and refined. All examples use ESP-IDF v6.

## How do I run these examples in my ESP32?

### 1. Install the necessary dependencies

1. Install ESP-IDF (and the drivers if using Windows) using EIM ([official docs](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html#installation)).
2. Install the ESP-IDF VS Code extension by Espressif Systems.

### 2. Build your project

Open the command palette (`Ctrl + Shift + P`) and select

```
ESP-IDF: Build Your Project
```

### 3. Flash your project

Open the command palette (`Ctrl + Shift + P`) and select

```
ESP-IDF: Flash Your Project
```

#### A Note for Windows users (skip this if you use Linux)

If you are on Windows, flashing might show this error:

```plaintext
A fatal error occurred: Could not connect to an Espressif device on any of the 1 available serial ports.
```

If that happens, press and hold the BOOT button on your board as soon as
you see `Connecting...`.

#### A Note for Linux users (skip this if you use Windows)

Before flashing, run this command, log out, and log back in (or reboot):

```bash
sudo usermod -aG dialout $USER
```

You have to do it only once.

<!--
In Linux, you should create a `/etc/udev/rules.d/99-esp32.rules` file, paste the following contents into the file, and restart the system before flashing. You have to do it only once per system.

```plaintext
KERNEL=="ttyUSB*", MODE="0666"
KERNEL=="ttyACM*", MODE="0666"
```
-->

## How do I create a new example?

- From the command palette, select `ESP-IDF: New Project` and select the ESP-IDF version.
- From the "ESP-IDF Templates" dropdown, select the `sample_project` template and click the create project button.
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
- Generate compile commands if prompted.
- Write your code and submit a pull request.
