#ifndef	WINDOW_H
#define WINDOW_H

#define KEY_RELEASE(name)		void name(XKeyEvent xkey)
#define BUTTON_RELEASE(name)	void name(XButtonEvent xbutton)
#define BUTTON_PRESS(name)		void name(XButtonEvent xbutton)
#define BUTTON_MOTION(name)		void name(XMotionEvent xmotion)

typedef KEY_RELEASE(KeyReleaseFn);
typedef BUTTON_RELEASE(ButtonReleaseFn);
typedef BUTTON_PRESS(ButtonPressFn);
typedef BUTTON_MOTION(ButtonMotionFn);

typedef struct {
	Display *dpy;
	Visual *vi;
	Colormap cmap;
	GC gc;
	Window root;
	int screen;
} GraphicsContext;

extern char windowName[];
extern char fileName[];
extern uint32_t verbose;
extern uint32_t debug;
extern uint32_t width, height;
extern KeyReleaseFn *keyReleaseFn;
extern ButtonReleaseFn *buttonReleaseFn;
extern ButtonPressFn *buttonPressFn;
extern ButtonMotionFn *buttonMotionFn;

extern void prepare(void);
extern void beforeLoop(GraphicsContext *ctx, Drawable dbl);
extern void draw(GraphicsContext *ctx, Drawable dbl);

#endif
