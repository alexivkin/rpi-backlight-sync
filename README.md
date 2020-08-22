# Raspberry Pi Touchscreen backlight sync

If you have a RPi that's on all the time, but rely on your screensaver to blank the screen and noticed that the backlight is not being turned off, while the screen is dark, this tool is for you.
It turns on and off the backlight of the Official RPi touchscreen in accordance with the screensaver state. No more light bleeding through the edge of the screen in a dark room and wasted power.
The best part is that it's event driven, not pull/timer based, so it uses almost no CPU by itself.

## Building

    sudo apt install gcc libxss-dev libx11-dev
    make
    make install

## Running

You can run it as root, but I recommend that you allow everyone to controll backlight via a udev rule as such:

        echo 'SUBSYSTEM=="backlight",RUN+="/bin/chmod 666 /sys/class/backlight/%k/brightness /sys/class/backlight/%k/bl_power"' | sudo tee /etc/udev/rules.d/61-brightness.rules

Then just start this tool after the X server starts or run it from a separate shell exporting the DISPLAY variable.

If you want to test it, set a short DPMS timeout like so: `xset s 10`

## Notes

The tool sets `/sys/class/backlight/rpi_backlight/bl_power` to 0 for on and 1 for off (sic), but some people noted that setting 0 for off and 255 for on in the fine grained `/sys/class/backlight/%k/brightness` works better. This however would mess up the brightness set by user
