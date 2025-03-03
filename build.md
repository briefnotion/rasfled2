Building on a Raspberry Pi 3B


Install Build Essentials and Manuals:
sudo apt install build-essential
sudo apt install manpages-dev


Install gh:
https://github.com/cli/cli/blob/trunk/docs/install_linux.md


Clone RasFled2 Repo to source dir:
gh repo clone briefnotion/rasfled2


Install CMake:
sudo apt install cmake


Clone ImGui repo to source dir:
gh repo clone ocornut/imgui


Install OpenGL libs for RPI
sudo apt install libglfw3 libglfw3-dev


Install the Boost libraries:
sudo apt install libboost-all-dev


Install wiringpi:
https://github.com/WiringPi/WiringPi?form=MG0AV3
sudo apt install wiringpi


Clone and build rpi_ws281x repo to source dir:
https://github.com/jgarff/rpi_ws281x
gh repo clone jgarff/rpi_ws281x

sudo apt-get install scons

mkdir build
cd build
cmake -D BUILD_SHARED=OFF -D BUILD_TEST=OFF ..

cmake --build .

sudo make install

Blacklist Raspberry Pi audio chip
sudo nano /etc/modprobe.d/snd-blacklist.conf
with the line
blacklist snd_bcm2835


Clone stb repo to source dir:
https://github.com/nothings/stb?form=MG0AV3


Ensure directory names are correct in definitions.h
#define DEF_LOGS_DIR                       "/home/delmane/rasfled/"    // Create Directory
#define DEF_LOGS_DIR_TEST                  "/home/delmane/rasfled-t/"  // Create Directory


Create directories for settings
sudo mkdir /etc/rasfled/
sudo mkdir /etc/rasfled-t/

Copy everthing in the "the_etc_slash_RasFLED_directory" to those directories
sudo cp the_etc_slash_RasFLED_directory/* /etc/rasfled/
sudo cp the_etc_slash_RasFLED_directory/* /etc/rasfled-t/


PI Configuration:
  Interface:
    I2C -> ON

Ensure I2c:
  i2cdetect -y 1
    or
  i2cdetect -y 2

  For the HMC5883l chip, with channel 1e, output should look like:
        0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
    00:                         -- -- -- -- -- -- -- --
    10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- 1e --
    20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


Change to build dir and compile
./cmake.sh
./m

Took multiple compiles to get it done.  Possibly mscode is creating and setting 
  directory paths on its on.

after first run:
  copy the adverts to the ~/rasfled/adverts directory


