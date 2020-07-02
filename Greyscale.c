//Eric Grant
//ewgrant
//convert grid of rgb values in file to greyscale values

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    int height;
    int width;
    double** grid;
} ImageData;

//scan in height and width
void getHeightWidth(ImageData* image){
    if (!scanf("%d %d", &image->height, &image->width)){
        printf("ERROR: Data invalid.");
    }
}

//convert number from 0-255 scale to 0-1 scale
double valueToDecimal(double val){
    return val / 255;
}

//fill grid with grey percent values
void fillGreyGridFromRGB(ImageData* image){
    double r, g, b;
    image->grid = malloc(sizeof(double*) * image->height);
    for (int x = 0; x < image->height; x++)
    {
        image->grid[x] = malloc(sizeof(double) * image->width);
        for (int y = 0; y < image->width; y++)
        {
            if (!scanf("%lf %lf %lf", &r, &g, &b))
            {
                printf("ERROR: Data invalid.");
            }
            r = valueToDecimal(r);
            g = valueToDecimal(g);
            b = valueToDecimal(b);
            image->grid[x][y] = (0.299 * r) + (0.587 * g) + (0.114 * b);
        }
    }
}

//print values in grey grid, height, and width
void printGreyGrid(ImageData* image){
    printf("%d %d\n", image->height, image->width);
    for (int x = 0; x < image->height; x++)
    {
        for (int y = 0; y < image->width; y++)
        {
            printf("%.6f ", image->grid[x][y]);
        }
        printf("\n");
    }
}

//free image grid
void freeGreyGrid(ImageData* image){
    for (int x = 0; x < image->height; x++)
    {
        free(image->grid[x]);
        image->grid[x] = NULL;
    }
    free(image->grid);
    image->grid = NULL;
}

int main(int argc, char** argv)
{
    ImageData image;

    //get width and height
    getHeightWidth(&image);

    //scan in rgb values and convert to greyscale
    fillGreyGridFromRGB(&image);
    
    //print greyscale values
    printGreyGrid(&image);

    //free grid
    freeGreyGrid(&image);
}