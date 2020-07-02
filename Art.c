//Eric Grant
//ewgrant
//print ascii art of gray scale image pixel file

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    int height;
    int width;
    double** grid;
} ImageData;

//draw horizontal border line
void drawLine(int width){
    for (int i = 0; i < width+2; i++)
    {
        if (i == 0 || i == width+1)
        {
            printf("+");
        } else {
            printf("-");
        }
    }
    printf("\n");
}

//print icon associated with given grey scale value
void printIcon(double num){
    if (num < 0.2)
    {
        printf(" ");
    } else if (num < 0.4) {
        printf(".");
    } else if (num < 0.6) {
        printf("o");
    } else if (num < 0.8) {
        printf("O");
    } else {
        printf("@");
    }
}

//scan in height and width
void getHeightWidth(ImageData* image){
    if (!scanf("%d %d", &image->height, &image->width)){
        printf("ERROR: Data invalid.");
    }
}

//fill grid with grey percent values
void fillGreyGrid(ImageData* image){
    image->grid = malloc(sizeof(double*) * image->height);
    for (int x = 0; x < image->height; x++)
    {
        image->grid[x] = malloc(sizeof(double) * image->width);
        for (int y = 0; y < image->width; y++)
        {
            if (!scanf("%lf", &image->grid[x][y]))
            {
                printf("ERROR: Data invalid.");
            }
        }
    }
}

//print values in grey grid, height, and width
void printAscii(ImageData* image, double delta){
    drawLine(image->width);
    for (int x = 0; x < image->height; x++)
    {
        printf("|");
        for (int y = 0; y < image->width; y++)
        {
            printIcon(image->grid[x][y] + delta);
        }
        printf("|");
        printf("\n");
    }
    drawLine(image->width);
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
    //if delta given, assign
    double shift = 0;
    if (argc > 1)
    {
        shift = atof(argv[1]);
    }

    ImageData image;

    //get width and height
    getHeightWidth(&image);

    //scan in greyscale values
    fillGreyGrid(&image);
    
    //print greyscale values
    printAscii(&image, shift);

    //free grid
    freeGreyGrid(&image);
}