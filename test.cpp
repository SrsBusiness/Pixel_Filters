#include "filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <png.h>

int write_image(XImage *img, char *name){
    FILE *fp = fopen(name, "wb");
    if(!fp){
        printf("ERROR: Could not open file %s\n", name);
        exit(1);
    }
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png_ptr){
        printf("ERROR: Could not allocate write struct\n");
        exit(1);
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if(!info_ptr){
        printf("ERROR: Could not allocate info struct\n");
        png_destroy_write_struct(&png_ptr, NULL);
        exit(1);
    }
    if (setjmp(png_jmpbuf(png_ptr))){
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        exit(1);
    }
    png_init_io(png_ptr, fp);
    png_set_IHDR(png_ptr, info_ptr, img -> width, img -> height, 8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    char **rows = (char **)calloc(img -> height, sizeof(char **));
    for(int i = 0; i < img -> height; i++){
        rows[i] = img -> data + i * img -> width * 4;
    }
    png_set_rows(png_ptr, info_ptr, (png_bytepp)rows);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR, NULL);
}


int main(){
    Display *display = XOpenDisplay(NULL);
    if(!display)
        exit(1);
    Window root = XDefaultRootWindow(display);
    XWindowAttributes gwa;
    XGetWindowAttributes(display, root, &gwa);
    int width = gwa.width;
    int height = gwa.height;
    XImage *screen = XGetImage(display, root, 0, 0, width, height, AllPlanes, ZPixmap);
    printf("Width: %d, Height: %d\n", screen -> width, screen -> height);
    //write_image(screen, "test.png");
    int x_matrix[9] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
    int y_matrix[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
    /*
    int x_matrix[4] = {1, 0, 0, -1};
    int y_matrix[4] = {0, 1, -1, 0};
    */
    Matrix g_x = (Matrix){x_matrix, 3, 3, 1};
    Matrix g_y = (Matrix){y_matrix, 3, 3, 1};
    XImage *edges = edge_detect(screen, &g_x, &g_y);
    write_image(edges, "edges.png");
}

