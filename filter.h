#include <X11/Xlib.h>
#ifndef TRANSFORM_H
#define TRANSFORM_H
struct Matrix{
    int *array;
    int rows, cols;
    double scale;
} typedef Matrix;

// frees XImages created by filter functions
// Might be redudant because XFree(), but included
// here just to be safe
void filter_free_image(XImage *);

char *convolve(XImage *, Matrix *);
char *convolve_color(XImage *, Matrix *);
// choose weights carefully. If color x heavily weighted,
// x becomes less distinguishable in the result
XImage *gray_scale(XImage *, double, double, double);

XImage *edge_detect(XImage *, Matrix *, Matrix *);
XImage *gauss_smooth(XImage *, Matrix *);
#endif
