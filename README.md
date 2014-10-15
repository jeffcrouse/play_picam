#play_picam

RPi cameras for the Play project

## Visual Effects

- https://github.com/KitchenTableCoders/intermediate-of/tree/master/week03
- http://bogost.com/games/a_television_simulator/
- http://forum.openframeworks.cc/t/giving-visuals-an-analog-feel/6607
- https://github.com/kylemcdonald/ofxCameraFilter
- https://github.com/kylemcdonald/ofxBlur


## Raspberry PI setup instructions

Taken mostly from [NOOBS setup](http://www.raspberrypi.org/help/noobs-setup/) and [Setup the Raspberry Pi for remote access with RDP or VNC and a static IP](http://www.ryukent.com/2013/04/setup-the-raspberry-pi-for-remote-access-with-rdp-or-vnc-and-a-static-ip/) and [Adafruit](https://learn.adafruit.com/adafruit-raspberry-pi-lesson-1-preparing-and-sd-card-for-your-raspberry-pi/overview)

1. Format SD card using SDFormatter https://www.sdcard.org/downloads/formatter_4/
1. Download NOOBS (http://www.raspberrypi.org/downloads/)
1. Extract files
1. Copy NOOB files to SD card
1. Boot up PI
1. Set internationalization options in raspi-config
1. Login: pi/raspberry
1. `sudo apt-get update`
1. `sudo apt-get upgrade`
1. `sudo apt-get install xrdp`
1. `sudo reboot`
1. `sudo ip addr show`
1. `startx`
1. [Make sure composite video works](http://elinux.org/R-Pi_Troubleshooting#Composite_displays_no_image) Is the Pi configuration forcing HDMI output? Inspect the file "config.txt" in the boot partition of your memory card and look for the line "hdmi_force_hotplug=1". If you see this line, comment it out (in particular, look at the end of "config.txt", in the section entitled "NOOBS Auto-generated Settings:")
1. [change hostname to something useful](http://www.howtogeek.com/167195/how-to-change-your-raspberry-pi-or-other-linux-devices-hostname/)
	1. `sudo nano /etc/hosts`
	1. `sudo nano /etc/hostname`
	1. `sudo /etc/init.d/hostname.sh`
	1. `sudo reboot`
1. [Set up Bonjour/ZeroConf for easy access on network](http://www.raspberrypi.org/forums/viewtopic.php?f=66&t=18207)
	1. sudo apt-get install libnss-mdns
1. Set up AFP (for file sharing with Macs)
	1. `sudo apt-get install netatalk`
1. Use "WiFi Config" to set up wireless
1. [Raspberry Pi - Getting Started (openFrameworks)](http://openframeworks.cc/setup/raspberrypi/Raspberry-Pi-Getting-Started.html)
	1. `cd`
	1. `curl -O http://www.openframeworks.cc/versions/v0.8.4/	of_v0.8.4_linuxarmv6l_release.tar.gz`
	1. `mkdir openFrameworks`
	1. `tar vxfz of_v0.8.4_linuxarmv6l_release.tar.gz -C openFrameworks --strip-components 1`
	1. `cd /home/pi/openFrameworks/scripts/linux/debian_armv6l`
	1. `sudo ./install_dependencies.sh`
	1. `make Release -C /home/pi/openFrameworks/libs/openFrameworksCompiled/project`
	1. `cd /home/pi/openFrameworks/examples/graphics/polygonExample`
	1. `make`
	1. `make run`
1. get https://github.com/jvcleave/ofxRPiCameraVideoGrabber
1. [Setup auto-login and run script](http://www.opentechguides.com/how-to/article/raspberry-pi/5/raspberry-pi-auto-start.html)
	1. Enable auto-login 
	1. `sudo nano /etc/profile`
1. When you are reset to shutdown: `sudo halt`
