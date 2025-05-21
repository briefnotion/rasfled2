# Building on a Raspberry Pi 3B and Raspberry Pi 5

This is a smiple list of instructions that guide me on installing Rasfled on a new RPI system.

Not all, but most, items used to create rasfled is available through Amazon and is on the rasfled list: https://www.amazon.com/hz/wishlist/ls/12PBNFZNUHIIO?ref_=wl_share

Consider:  
https://github.com/PlummersSoftwareLLC/NightDriverStrip

## Install Build Essentials and Manuals
Necessay for compiler and building tools
```
sudo apt install build-essential
sudo apt install manpages-dev
```

## Install gh
Installs GitHub.  Necessary for getting sorce code that Rasfled will need  
https://github.com/cli/cli/blob/trunk/docs/install_linux.md

## Clone RasFled2 Repo to source dir
Rasfled source to create the `rasfled` execuatble.  
`gh repo clone briefnotion/rasfled2`

## Install CMake
Needed to crate the rasfled 'makefile'  
`sudo apt install cmake`

## Clone ImGui repo to source dir
Necessary Graphical User Interface code for running rasfled in OpenGL  
`gh repo clone ocornut/imgui`

## Install OpenGL libs for RPI
Installs the graphics drivers that imgui uses to access the OpenGL lay.  May not be necessary but the `CMakeLists.txt` will be looking for it.  
`sudo apt install libglfw3 libglfw3-dev`

## Install the Boost libraries
The booste libraries are used to show the core temps on the main screen.  
`sudo apt install libboost-all-dev`

## Install wiringpi
WiringPi is, last I checked, required to run the rpi_ws281x LED drivers.  
https://github.com/WiringPi/WiringPi?form=MG0AV3

`sudo apt install wiringpi` likely doesn't work.

I followed these steps:  
`wget https://github.com/WiringPi/WiringPi/releases/download/3.14/wiringpi_3.14_arm64.deb`  
`sudo chmod 644 /home/delmane/work/wiringpi_3.14_arm64.deb`  
`sudo apt install ./wiringpi_3.14_arm64.deb`

## Clone and build rpi_ws281x repo to source dir
Repository containing the LED light drivers. Instalation can be conveluted so follow the instructions well.  
https://github.com/jgarff/rpi_ws281x

`gh repo clone jgarff/rpi_ws281x`

### For installing on RPI 3B follow these instructions
`sudo apt-get install scons`
```
mkdir build
cd build
cmake -D BUILD_SHARED=OFF -D BUILD_TEST=OFF ..
```

```
cmake --build .
```

```
sudo make install
```

Blacklist Raspberry Pi audio chip  
`sudo nano /etc/modprobe.d/snd-blacklist.conf`  
by adding the following line to the file  
`blacklist snd_bcm2835`

### For installing on RPI 5 follow these instructions
https://github.com/jgarff/rpi_ws281x/wiki/Raspberry-Pi-5-Support  
my post: https://github.com/jgarff/rpi_ws281x/issues/528#issuecomment-2784422523

Dont forget to run (as is not part of the instructions but necessary or Segmentation fault will occur):

`sudo make install`

## Clone stb repo to source dir
STB is the source code needed to load .bmp images as textures for the adverts to be displayed  
https://github.com/nothings/stb?form=MG0AV3

## PI Configuration
I2c is needed for the compass comunications.  The setting is located on the Raspberry's setup program under Interface.

Interface:  
I2C -> ON

## Ensure I2c
`i2cdetect -y 1`
or
`i2cdetect -y 2`

For the HMC5883l chip, with channel 1e, output should look like:
```
    0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:                         -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- 1e --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
```

## To get logs to work
Install:  
`sudo apt install rsyslog`

Settings:  
`settings:  sudo nano /etc/rsyslog.conf`

Enable modules by removing the #.

## Set the program up
The `definitions.h` file will contain most of the setting that need to be made for your build.  Most things can be left as is, but for live build, `TEST` vars should be false, and anything with a `COMMS_AUTOSTART_...` should be set to false if system isn't available, or true if it is.

## Create Directory Structure
Ensure directory names are correct in definitions.h  
For example, your home directory is likely to be `pi`
```
#define DEF_LOGS_DIR                       "/home/delmane/rasfled/"    // Create Directory
#define DEF_LOGS_DIR_TEST                  "/home/delmane/rasfled-t/"  // Create Directory
```

Create directories to contain the settings
```
sudo mkdir /etc/rasfled/
sudo mkdir /etc/rasfled-t/
```

Copy everthing in the "the_etc_slash_RasFLED_directory" to those directories
```
sudo cp the_etc_slash_RasFLED_directory/* /etc/rasfled/
sudo cp the_etc_slash_RasFLED_directory/* /etc/rasfled-t/
```

## Change to build dir and compile
After all these preperations were made, the program should compile sucessfully.  A few scripts that I use to create the execuatable is located in the build folder. `cmake.sh` is likely only needed to be called once. `m` will need to be called on first build and after, any, if any, changes to the sorce. 
```
./cmake.sh
./m
```
My experice required multiple compiles to for completeion.  Possibly because mscode is creating and setting directory paths on its on.

On sucessful compile, a `rasfled` executable should be available in the build folder.  

To run in graphical mode
```
sudo ./rasfled
```

or in text only mode
```
sudo ./rasfled -tty
```

There are additional instructions to get rasfled to run in a headless mode in the `source/start_at_boot` folder, which requires a `~/startup/rasfled` folder. `pu.sh` and `qpu.sh` will move the rasfled exe to those folders.  the pu scripts aren't fully developed, though.

## After first run
copy the adverts to the `~/rasfled/adverts directory`

## Setting up the service
`mkdir ~/startup`

move *.sh to ~/startup

Check their permissions with because the scripts need to have executable status.

Add the service file to the system directory and continue.  
`sudo cp myscript.service /etc/systemd/system/`  
`sudo systemctl daemon-reload`  
`sudo systemctl enable myscript.service`

Test  
`sudo systemctl start myscript.service`

Verify  
`sudo systemctl status myscript.service`

Inspect  
`journalctl -u myscript.service`
