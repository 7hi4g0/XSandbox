#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <window.h>

/*
char windowName[] = "Blank Window";
char fileName[] = "window.ppm";
*/
uint32_t verbose;
uint32_t debug;
uint32_t width, height;

KEY_RELEASE(keyReleaseStub) {
}

BUTTON_RELEASE(buttonReleaseStub){
}

BUTTON_PRESS(buttonPressStub){
}

BUTTON_MOTION(buttonMotionStub){
}

KeyReleaseFn *keyReleaseFn = keyReleaseStub;
ButtonReleaseFn *buttonReleaseFn = buttonReleaseStub;
ButtonPressFn *buttonPressFn = buttonPressStub;
ButtonMotionFn *buttonMotionFn = buttonMotionStub;

int main(int argc, char *argv[]) {
	GraphicsContext gContext;
	XSetWindowAttributes swa;
	Atom delete_event;
	Window win;

	// Default global values
	width = 600;
	height = 600;
	verbose = 0;
	debug = 0;

	{
		char opt;

		while ((opt = getopt(argc, argv, ":dv")) != -1) {
			switch (opt) {
				case 'd':
					debug +=1;
					break;
				case 'v':
					verbose += 1;
					break;
				case ':':
					fprintf(stderr, "%c needs an argument\n", optopt);
					exit(-1);
					break;
				default:
					fprintf(stderr, "Unknown option %c\n", optopt);
					exit(-1);
					break;
			}
		}
	}

	prepare();
	
	gContext.dpy = XOpenDisplay(NULL);

	gContext.root = DefaultRootWindow(gContext.dpy);
	gContext.screen = DefaultScreen(gContext.dpy);
	gContext.cmap = DefaultColormap(gContext.dpy, gContext.screen);
	gContext.vi = DefaultVisual(gContext.dpy, gContext.screen);
	gContext.gc = DefaultGC(gContext.dpy, gContext.screen);

	swa.background_pixel = 0xFFFFFF;
	swa.colormap = gContext.cmap;
	swa.event_mask = StructureNotifyMask | KeyReleaseMask | ButtonReleaseMask | ButtonPressMask | ButtonMotionMask;

	win = XCreateWindow(gContext.dpy, gContext.root, 0, 0, width, height, 0, CopyFromParent, InputOutput, CopyFromParent, CWBackPixel | CWColormap | CWEventMask, &swa);
	//win = XCreateSimpleWindow(gContext.dpy, gContext.root, 0, 0, 600, 600, 0, 0xBCBCBC);
	
	delete_event = XInternAtom(gContext.dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(gContext.dpy, win, &delete_event, 1);

	XMapWindow(gContext.dpy, win);
	XStoreName(gContext.dpy, win, windowName);

	beforeLoop(&gContext, win);

	XEvent xev;
	KeySym keysym;
	char loop = 1;

	while (loop) {
		while (XPending(gContext.dpy) > 0) {
			XNextEvent(gContext.dpy, &xev);

			switch (xev.type) {
				case ClientMessage:
					if (xev.xclient.data.l[0] == delete_event) {
						loop = 0;
					}
					break;
				case KeyRelease:
					switch (keysym = XLookupKeysym(&xev.xkey, 0)) {
						default:
							if (verbose) {
								fprintf(stderr, "Untreated '%s' captured.\n", XKeysymToString(keysym));
							}
							break;
					}
					keyReleaseFn(xev.xkey);
					break;
				case ButtonPress:
					buttonPressFn(xev.xbutton);
					break;
				case ButtonRelease:
					buttonReleaseFn(xev.xbutton);
					break;
				case MotionNotify:
					buttonMotionFn(xev.xmotion);
					break;
				default:
					break;
			}
		}

		XClearWindow(gContext.dpy, win);

		draw(&gContext, win);

		XFlush(gContext.dpy);
		
		usleep(33000);
	}

	XDestroyWindow(gContext.dpy, win);
	XCloseDisplay(gContext.dpy);
}
