#include "filter.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define abs(a) a < 0 ? -1 * a : a
void filter_free_image(XImage *img){
    free(img -> data);
    free(img);
}

// assume grayscaled image
char *convolve(XImage *img, Matrix *m){ 
    int diff = m -> rows - 1;
    unsigned char *extended = (unsigned char *)calloc((img -> width + diff) * (img -> width + diff) * 4, sizeof(unsigned char));
    // extend with garbage values - shouldn't matter since it's the edge

    for(int i = 0; i < img -> height; i++){
        memcpy(extended + i * (img -> width + diff) * 4, img -> data + i * img -> width * 4, img -> width * 4);
        /*
           for(int j = 0; j < img -> width * 4; j++)
           extended[i * (img -> width + diff) * 4 + j] = img -> data[i * img -> width * 4 + j];
           */
    }
    unsigned char *result = (unsigned char *)calloc(img -> width * img -> height * 4, sizeof(unsigned char));
    int pixels = img -> width * img -> height;
    for(int i = 0; i < pixels; i++){
        int new_val = 0;;
        /* not grayscaled
           for(int j = 0; j < 3; j++){
           new_val = 0;
           for(int k = 0; k < m -> rows * m -> cols; k++){
           new_val += m -> array[k] * extended[((i / img -> width) * (img -> width + diff) + i % img -> width + (k / m -> rows) 
         * (img -> width + diff) + (k % m -> cols)) * 4 + j];
         }
         result[i * 4 + j] = (char)(new_val * scale);
         }
         */
        /* grayscaled */
        for(int j = 0; j < m -> rows * m -> cols; j++){
            new_val += m -> array[j] * (int)extended[((i / img -> width) * 
                    (img -> width + diff) + i % img -> width + (j / m -> rows) 
                    * (img -> width + diff) + (j % m -> cols)) * 4];
        }
        result[i * 4] = result[i * 4 + 1] = result[i * 4 + 2] = 
            abs((int)(new_val * m -> scale));
        result[i * 4 + 3] = img -> data[i * 4 + 3];

    }
    free(extended);
    return (char *)result;
}

char *convolve_color(XImage *img, Matrix *m){
    int diff = m -> rows - 1;
    unsigned char *extended = (unsigned char *)calloc((img -> width + diff) * (img -> width + diff) * 4, sizeof(unsigned char));
    // extend with garbage values - shouldn't matter since it's the edge

    for(int i = 0; i < img -> height; i++){
        memcpy(extended + i * (img -> width + diff) * 4, img -> data + i * img -> width * 4, img -> width * 4);
        /*
           for(int j = 0; j < img -> width * 4; j++)
           extended[i * (img -> width + diff) * 4 + j] = img -> data[i * img -> width * 4 + j];
           */
    }
    unsigned char *result = (unsigned char *)calloc(img -> width * img -> height * 4, sizeof(unsigned char));
    int pixels = img -> width * img -> height;
    for(int i = 0; i < pixels; i++){
        int new_val;
        /* not grayscaled */
        for(int j = 0; j < 3; j++){
            new_val = 0;
            for(int k = 0; k < m -> rows * m -> cols; k++){
                new_val += m -> array[k] * extended[((i / img -> width) * (img -> width + diff) + i % img -> width + (k / m -> rows) 
                        * (img -> width + diff) + (k % m -> cols)) * 4 + j];
            }
            //printf("scale: %f\n", m -> scale);
            //printf("new_vale: %X\n", (int)(new_val * m -> scale));
            result[i * 4 + j] = (int)(new_val * m -> scale);
        }
        /* grayscaled
           for(int j = 0; j < m -> rows * m -> cols; j++){
           new_val += m -> array[j] * extended[((i / img -> width) * 
           (img -> width + diff) + i % img -> width + (j / m -> rows) 
         * (img -> width + diff) + (j % m -> cols)) * 4];
         }
         result[i * 4] = result[i * 4 + 1] = result[i * 4 + 2] = 
         (int)(new_val * m -> scale);
         */
         result[i * 4 + 3] = img -> data[i * 4 + 3];
    }
    free(extended);
    return (char *)result;

}

XImage *gray_scale(XImage * img, double red_weight, double green_weight, double blue_weight){
    XImage *result = (XImage *)malloc(sizeof(XImage));
    unsigned char* usign_data = (unsigned char *)img -> data;
    unsigned char *data = (unsigned char *)calloc(img -> width * img -> height * 4, sizeof(unsigned char));
    for(int i = 0; i < img -> width * img -> height; i++){
        int gray = (int)(usign_data[i * 4] * blue_weight + 
                usign_data[i * 4 + 1] * green_weight + 
                usign_data[i * 4 + 2] * red_weight);
        data[i * 4] = data[i * 4 + 1] = data[i * 4 + 2] = gray;
        data[i * 4 + 3] = img -> data[i * 4 + 3];
    }
    *result = *img;
    result -> data = (char *)data;
    return result;
}

XImage *edge_detect(XImage *img, Matrix *g_x, Matrix *g_y){
    img = gray_scale(img, .2126, .7152, .0722);
    char *x = convolve(img, g_x);
    char *y = convolve(img, g_y);
    //char *data = (char *)calloc(img -> width * img -> height * 4, sizeof(char));
    for(int i = 0; i < img -> width * img -> height; i++){
        /*
           int avg = (abs(x[i * 4]) + abs(y[i * 4]) + 
           abs(x[i * 4 + 1]) + abs(y[i * 4 + 1]) + 
           abs(x[i * 4 + 2]) + abs(y[i * 4 + 2])) / 3;
           */
        /* not grayscaled
           int avg = (sqrt(x[i * 4] * x[i * 4] + y[i * 4] * y[i * 4]) +
           sqrt(x[i * 4 + 1] * x[i * 4 + 1] + y[i * 4 + 1] * y[i * 4 + 1]) +
           sqrt(x[i * 4 + 2] * x[i * 4 + 2] + y[i * 4 + 2] * y[i * 4 + 2])) / 3;
           data[i * 4] = data[i * 4 + 1] = data[i * 4 + 2] = avg;
           data[i * 4 + 3] = 0xFF;
           */
        /*grayscaled*/
        //int dist = abs(x[i * 4]) + abs(y[i * 4]) ign;
        int dist = sqrt((x[i * 4] * x[i * 4] + y[i * 4] * y[i * 4]) / 2);
        x[i * 4] = x[i * 4 + 1] = x[i * 4 + 2] = dist;
    }
    /*
       char *extended = (char *)calloc((img -> width + 2) * (img -> width + 2) * 4, sizeof(char));
       for(int i = 0; i < (img -> width + 2) * (img -> height + 2) * 4; i++)
       extended[i] = x[i];
       */
    XImage *result = (XImage *)malloc(sizeof(XImage));
    *result = *img;
    //result -> width = img -> width + 2;
    //result -> height = img -> height + 2;
    result -> data = x;
    filter_free_image(img);
    free(y);
    return result;
}

XImage *gauss_smooth(XImage *img, Matrix *gauss){
    XImage *result = (XImage *)malloc(sizeof(XImage));
    *result = *img;
    result -> data = convolve_color(img, gauss);
    return result;
}
