#include <stdlib.h>
#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main() {
	Display *dpy;

	dpy = XOpenDisplay(NULL);

	XVisualInfo *vi, viTemplate;
	int count;
	int index;

	viTemplate.screen = DefaultScreen(dpy);

	vi = XGetVisualInfo(dpy, VisualScreenMask, &viTemplate, &count);

	for (index = 0; index < count; index++) {
		printf("%d %d %u %d %x %x %x %d %d\n", vi->visualid, vi->screen, vi->depth, vi->class, vi->red_mask, vi->green_mask, vi->blue_mask, vi->colormap_size, vi->bits_per_rgb);
		vi++;
	}

	XCloseDisplay(dpy);

	return 0;
}
