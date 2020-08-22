/*
 * Turn off RPi Touchscreen 7' backlight when screensaver is active
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/extensions/scrnsaver.h>

static Display *xdisplay;

static int errorHandler(Display *xdisplay, XErrorEvent *e) {
    char errbuf[BUFSIZ];
    XGetErrorText(xdisplay, e->error_code, errbuf, sizeof(errbuf));
    errx(1, "X11 error: %s", errbuf);
}

static void closeDisplay(void) {
    if (xdisplay != NULL)
        XCloseDisplay(xdisplay);
}

int main() {
    XEvent event;
    char backstate;
    int eventbase, errbase;

    int backlightfd = open("/sys/class/backlight/rpi_backlight/bl_power", O_RDWR | O_NONBLOCK);
    if(backlightfd == -1)
        errx(1, "Error opening backlight file: %s", strerror(errno));

    if ((xdisplay = XOpenDisplay(NULL)) == NULL)
        errx(1, "No X11 display");

    atexit(closeDisplay);

    if (!XScreenSaverQueryExtension(xdisplay, &eventbase, &errbase))
        errx(1, "No X11 MIT-SCREEN-SAVER extension");

    XSetErrorHandler(errorHandler);

    XScreenSaverSelectInput(xdisplay, DefaultRootWindow(xdisplay),ScreenSaverNotifyMask);
    while (XNextEvent(xdisplay, &event) == 0) {
        // technically we should fork here, but if your screensavers is blinking that rapidly, you might have other problems
        if (((XScreenSaverNotifyEvent *)&event)->state == ScreenSaverOn) {
            backstate = '1';
            write(backlightfd, &backstate, sizeof(char));
        } else if (((XScreenSaverNotifyEvent *)&event)->state == ScreenSaverOff) {
            backstate = '0';
            write(backlightfd, &backstate, sizeof(char));
        }
    }
    return 0;
}
