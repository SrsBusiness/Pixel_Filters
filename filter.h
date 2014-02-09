#include <X11/Xlib.h>
#ifndef TRANSFORM_H
#define TRANSFORM_H
struct Matrix{
    int *array;
    int rows, cols;
    int scale;
} typedef Matrix;

// frees XImages created by filter functions
// Might be redudant because XFree(), but included
// here just to be safe
void filter_free_image(XImage *);

int *convolve(XImage *, Matrix *, double);

// choose weights carefully. If color x heavily weighted,
// x becomes less distinguishable in the result
XImage *gray_scale(XImage *, double, double, double);

XImage *edge_detect(XImage *, Matrix *, Matrix *);
#endif
