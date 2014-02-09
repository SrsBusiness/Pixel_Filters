#include "filter.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define abs(a) a < 0 ? -1 * a : a
int *convolve(XImage *img, Matrix *m, double scale){ 
    int diff = m -> rows - 1;
    int *extended = (int *)calloc((img -> width + diff) * (img -> width + diff) * 4, sizeof(int));
    // extend with garbage values - shouldn't matter since it's the edge
    
    for(int i = 0; i < img -> height; i++){
        //memcpy(extended + i * (img -> width + diff) * 4, img -> data + i * img -> width * 4, img -> width * 4);
        for(int j = 0; j < img -> width * 4; j++)
            extended[i * (img -> width + diff) * 4 + j] = img -> data[i * img -> width * 4 + j];
    }
    int *result = (int *)calloc(img -> width * img -> height * 4, sizeof(int));
    int pixels = img -> width * img -> height;
    for(int i = 0; i < pixels; i++){
        int new_val;
        for(int j = 0; j < 3; j++){
            new_val = 0;
            for(int k = 0; k < m -> rows * m -> cols; k++){
                new_val += m -> array[k] * extended[((i / img -> width) * (img -> width + diff) + i % img -> width + (k / m -> rows) 
                        * (img -> width + diff) + (k % m -> cols)) * 4 + j];
            }
            result[i * 4 + j] = (char)(new_val * scale);
        }
        result[i * 4 + 3] = 0xFF;
    }
    return result;
}

XImage *gray_scale(XImage * img, double red_weight, double green_weight, double blue_weight){
}

XImage *edge_detect(XImage *img, Matrix *g_x, Matrix *g_y){
    int *x = convolve(img, g_x, 1);
    int *y = convolve(img, g_y, 1);
    char *data = (char *)calloc(img -> width * img -> height * 4, sizeof(char));
    for(int i = 0; i < img -> width * img -> height; i++){
        /*
        int avg = (abs(x[i * 4]) + abs(y[i * 4]) + 
            abs(x[i * 4 + 1]) + abs(y[i * 4 + 1]) + 
            abs(x[i * 4 + 2]) + abs(y[i * 4 + 2])) / 3;
            */
        int avg = (sqrt(x[i * 4] * x[i * 4] + y[i * 4] * y[i * 4]) +
            sqrt(x[i * 4 + 1] * x[i * 4 + 1] + y[i * 4 + 1] * y[i * 4 + 1]) +
            sqrt(x[i * 4 + 2] * x[i * 4 + 2] + y[i * 4 + 2] * y[i * 4 + 2])) / 3;
        data[i * 4] = data[i * 4 + 1] = data[i * 4 + 2] = avg;
        data[i * 4 + 3] = 0xFF;
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
    result -> data = data;
    return result;
}

