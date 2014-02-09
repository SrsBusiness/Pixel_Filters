#include <X11/Xlib.h>
#ifndef TRANSFORM_H
#define TRANSFORM_H
struct Matrix{
    int *array;
    int rows, cols;
    int scale;
} typedef Matrix;


int *convolve(XImage *, Matrix *, double);
XImage *gray_scale(XImage *, double, double, double);
XImage *edge_detect(XImage *, Matrix *, Matrix *);
#endif
