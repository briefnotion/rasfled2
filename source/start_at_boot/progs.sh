#!/bin/bash
cd /home/delmane/source/rpi_ws281x/rp1_ws281x_pwm
sudo insmod ./rp1_ws281x_pwm.ko pwm_channel=2
sudo dtoverlay -d . rp1_ws281x_pwm
sudo pinctrl set 18 a3 pn
cd /home/delmane/startup/rasfled
sudo ./rasfled -tty
