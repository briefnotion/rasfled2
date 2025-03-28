# Run at boot or headless

Added start_at_boot in the source folder with a few files that may help starting rasfled in a headless mode.
    
Includes: 
`myscript.services` - service file that needs to be copied to service directory as /`etc/systemd/system/myscript.service`
                            
Home directory needs to contain startup folder `progs.sh` and `start.sh` needs to copied to the startup folder.

`progs.sh` has 3 line of specifically for the raspberry pi 5: `insmod`, `dtoverlay` and `pinctrl` can be commented out when running on the rpi 3.

`startup` folder needs a `rasfled` folder created in it, to contain the rasfled executable.