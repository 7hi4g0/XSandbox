#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <fontconfig/fontconfig.h>
#include <X11/Xft/Xft.h>

#include <window.h>

typedef struct {
	XftFont *font;
	XftDraw *draw;
	XRenderColor color;
	XftColor textColor;
} TextContext;

char windowName[] = "Example Window";
char fileName[] = "blank.ppm";
TextContext textCtx = {0};
TextContext *fontsCtx;
size_t fontsSize;

void prepare(void) {
	width = 900;
	height = 650;
}

void beforeLoop(GraphicsContext *ctx, Drawable dbl) {
	XftFontSet *fonts;

	textCtx.font = XftFontOpen(ctx->dpy, ctx->screen,
			XFT_FAMILY, XftTypeString, "liberation",
			XFT_SPACING, XftTypeInteger, XFT_MONO,
			//XFT_SIZE, XftTypeDouble, 32,
			0);

	textCtx.draw = XftDrawCreate(ctx->dpy, dbl, ctx->vi, ctx->cmap);

	textCtx.color.red = 0;
	textCtx.color.green = 0;
	textCtx.color.blue = 0;
	textCtx.color.alpha = 0xffff;

	XftColorAllocValue(ctx->dpy, ctx->vi, ctx->cmap, &textCtx.color, &textCtx.textColor);

	fonts = XftListFonts(ctx->dpy, ctx->screen,
						 XFT_SPACING, XftTypeInteger, XFT_MONO,
						 FC_LANG, XftTypeString, "en",
						 0,
						 XFT_FAMILY,
						 XFT_SLANT,
						 XFT_SIZE,
						 0);

	fontsSize = fonts->nfont;
	fontsCtx = (TextContext *) malloc(sizeof(TextContext) * fontsSize);

	for (int fontIndex = 0; fontIndex < fonts->nfont; fontIndex++) {
		XftPattern *pattern;
		XftResult result;
		XftFont *font;

		pattern = fonts->fonts[fontIndex];

		pattern = XftFontMatch(ctx->dpy, ctx->screen, pattern, &result);
		if (result == XftResultMatch) {
			font = XftFontOpenPattern(ctx->dpy, pattern);

			fontsCtx[fontIndex].font = font;
			fontsCtx[fontIndex].draw = textCtx.draw;
			fontsCtx[fontIndex].color = textCtx.color;
			fontsCtx[fontIndex].textColor = textCtx.textColor;
		}
	}
}

void draw(GraphicsContext *ctx, Drawable dbl) {
	for (int fontIndex = 0; fontIndex < fontsSize; fontIndex++) {
		XftValue value;

		XftPatternGet(fontsCtx[fontIndex].font->pattern, "fullname", 0, &value);

		XftDrawString8(textCtx.draw, &textCtx.textColor, fontsCtx[fontIndex].font, 200, 40 + (30 * fontIndex), value.u.s, strlen(value.u.s));
	}
}
