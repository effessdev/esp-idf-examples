# Wi-Fi Station

## Before building

Open the project configuration menu. This can be done by

1. Running `ESP-IDF: SDK Configuration Editor (Menuconfig)` from the VS Code command palette (recommended) or
2. Running the command `idf.py menuconfig` from the terminal

In the configuration menu:

- Set `WiFi SSID`
- Set `WiFi Password`

The configuration is stored in `sdkconfig` file. The default configurations come from `sdkconfig.default` file.

> Optional: If you need, change the other options according to your requirements.

## Example Output

Note that the output, in particular the order of the output, may vary depending on the environment.

Console output if station connects to AP successfully:

```
I (589) wifi station: ESP_WIFI_MODE_STA
I (599) wifi: wifi driver task: 3ffc08b4, prio:23, stack:3584, core=0
I (599) system_api: Base MAC address is not set, read default base MAC address from BLK0 of EFUSE
I (599) system_api: Base MAC address is not set, read default base MAC address from BLK0 of EFUSE
I (629) wifi: wifi firmware version: 2d94f02
I (629) wifi: config NVS flash: enabled
I (629) wifi: config nano formatting: disabled
I (629) wifi: Init dynamic tx buffer num: 32
I (629) wifi: Init data frame dynamic rx buffer num: 32
I (639) wifi: Init management frame dynamic rx buffer num: 32
I (639) wifi: Init management short buffer num: 32
I (649) wifi: Init static rx buffer size: 1600
I (649) wifi: Init static rx buffer num: 10
I (659) wifi: Init dynamic rx buffer num: 32
I (759) phy: phy_version: 4180, cb3948e, Sep 12 2019, 16:39:13, 0, 0
I (769) wifi: mode : sta (30:ae:a4:d9:bc:c4)
I (769) wifi station: wifi_init_sta finished.
I (889) wifi: new:<6,0>, old:<1,0>, ap:<255,255>, sta:<6,0>, prof:1
I (889) wifi: state: init -> auth (b0)
I (899) wifi: state: auth -> assoc (0)
I (909) wifi: state: assoc -> run (10)
I (939) wifi: connected with #!/bin/test, aid = 1, channel 6, BW20, bssid = ac:9e:17:7e:31:40
I (939) wifi: security type: 3, phy: bgn, rssi: -68
I (949) wifi: pm start, type: 1

I (1029) wifi: AP's beacon interval = 102400 us, DTIM period = 3
I (2089) esp_netif_handlers: sta ip: 192.168.77.89, mask: 255.255.255.0, gw: 192.168.77.1
I (2089) wifi station: got ip:192.168.77.89
I (2089) wifi station: connected to ap SSID:myssid password:mypassword
```

Console output if the station failed to connect to AP:

```
I (589) wifi station: ESP_WIFI_MODE_STA
I (599) wifi: wifi driver task: 3ffc08b4, prio:23, stack:3584, core=0
I (599) system_api: Base MAC address is not set, read default base MAC address from BLK0 of EFUSE
I (599) system_api: Base MAC address is not set, read default base MAC address from BLK0 of EFUSE
I (629) wifi: wifi firmware version: 2d94f02
I (629) wifi: config NVS flash: enabled
I (629) wifi: config nano formatting: disabled
I (629) wifi: Init dynamic tx buffer num: 32
I (629) wifi: Init data frame dynamic rx buffer num: 32
I (639) wifi: Init management frame dynamic rx buffer num: 32
I (639) wifi: Init management short buffer num: 32
I (649) wifi: Init static rx buffer size: 1600
I (649) wifi: Init static rx buffer num: 10
I (659) wifi: Init dynamic rx buffer num: 32
I (759) phy: phy_version: 4180, cb3948e, Sep 12 2019, 16:39:13, 0, 0
I (759) wifi: mode : sta (30:ae:a4:d9:bc:c4)
I (769) wifi station: wifi_init_sta finished.
I (889) wifi: new:<6,0>, old:<1,0>, ap:<255,255>, sta:<6,0>, prof:1
I (889) wifi: state: init -> auth (b0)
I (1889) wifi: state: auth -> init (200)
I (1889) wifi: new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (1889) wifi station: retry to connect to the AP
I (1899) wifi station: connect to the AP fail
I (3949) wifi station: retry to connect to the AP
I (3949) wifi station: connect to the AP fail
I (4069) wifi: new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (4069) wifi: state: init -> auth (b0)
I (5069) wifi: state: auth -> init (200)
I (5069) wifi: new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (5069) wifi station: retry to connect to the AP
I (5069) wifi station: connect to the AP fail
I (7129) wifi station: retry to connect to the AP
I (7129) wifi station: connect to the AP fail
I (7249) wifi: new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (7249) wifi: state: init -> auth (b0)
I (8249) wifi: state: auth -> init (200)
I (8249) wifi: new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (8249) wifi station: retry to connect to the AP
I (8249) wifi station: connect to the AP fail
I (10299) wifi station: connect to the AP fail
I (10299) wifi station: Failed to connect to SSID:myssid, password:mypassword
```


## `idf_component.yml`

- Declares component dependencies and metadata.
- Used by the ESP-IDF Component Manager.
- Specifies which external components (and versions) to download.
- The resolved versions are recorded in `dependencies.lock`.

## How configurations are stored

- `sdkconfig` stores the current configuration. Not commited to version control.
- If `sdkconfig` was not found and it is required, it is created from `sdkconfig.defaults` (plus ESP-IDF defaults).
- We don't edit `sdkconfig` directly:
  - We use Menuconfig to edit `sdkconfig`.
  - Menuconfig can be launched using VS Code command palette (`ESP-IDF: SDK Configuration Editor (Menuconfig)`).
  - Menuconfig provides a graphical method to edit `sdkconfig`.
  - `Kconfig.projbuild` contains definitions, examples, etc. for the configuration options that are shown in Menuconfig so that it's easier for us to edit.

## Supported targets

- ESP32
- ESP32-C2
- ESP32-C3
- ESP32-C5
- ESP32-C6
- ESP32-C61
- ESP32-H2
- ESP32-P4
- ESP32-S2
- ESP32-S3
