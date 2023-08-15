# rasfled2

RasFLED: A Multi-Colored Light Strip Control Program for Raspberry Pi

RasFLED is an innovative control program tailored for multi-colored light strips, compatible with Raspberry Pi. This versatile platform accommodates various control modules, with the current iteration (Version 2.021_230814) optimized for the Raspberry Pi 3b and featuring four distinct modules.

Module 1 - LED Control and Switch Pins:
Module 1 encompasses LED manipulation and integration with four switch pins plus a ground connection. The present configuration, designed with an automotive context, links each switch pin to a door's open or closed status. When a door opens, overhead lights activate alongside warning indicators. Conversely, closed doors result in lights being turned off.

Module 2 - User Interface:
The system operates effectively in a headless environment, activated via a -tty command option. However, the user interface significantly amplifies functionality through command inputs. Powered by OpenGL, this interface facilitates control over the remaining modules, while offering advanced features including an alert timer, day-night mode, hazard lights, color scheme adjustments, and more.

Module 3 - Automobile Integration:
By accessing the "Automobile" tab, users equipped with a CAM bus reader linked to their car's OBD2 interface can passively access and display a comprehensive range of vehicle data. This data encompasses velocity, acceleration, cruise control settings, tachometer readings, drive gear selection, transmission gear details, temperature metrics, pressure readings, battery voltage, historical temperature-speed graphs, and real-time readings. Additionally, this module governs LED animations based on speed and gear selection, and it overrides door switches by communicating directly with the car's computer. In the event of CAM bus communication interruption, the system reverts to Module 1's switch statuses as a backup.

Please note that much of the aforementioned information is subject to rapid changes, and the automobile module is inherently specific to the creator's vehicle make and model. Customization for other vehicles involves modifying the automobile CAM bus portion, including PID values and computation methodologies. Given the absence of a standardized internal car communication protocol, these adaptations are necessary.

Module 4 - ADS-B Transponder Receiver:
Module 4 accommodates an ADS-B transponder receiver, utilizing an RT radio receiver with an improvised antenna attached to the car's roof. Functioning autonomously, the Interface passively monitors flight logs, displaying status details via the ADSB tab. While its functionality primarily entails starting and stopping the flight aware system, users can also capture snapshots of the current status. Observing aircraft blips on the radar screen from distances of up to 100 miles adds an engaging dimension to long drives.

This marks the second version of RasFLED, with the initial version available at https://github.com/briefnotion/RasFLED. The first version boasts full capabilities but runs within a text console window, distinguishing it from RasFLED2. RasFLED2 shines through its refined graphical interface, boasting pixel-level control and access to over 10 colors. This interface upgrade justifies the rewrite and sets it apart from its predecessor.

In conclusion, RasFLED2 serves as a program designed to deliver dashboard visuals and vibrant lights, responding dynamically to various triggers and events.
