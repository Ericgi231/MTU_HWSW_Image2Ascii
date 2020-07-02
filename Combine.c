//Eric Grant
//ewgrant
//combine greyscale image data using masks and output to file

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//struct containing file names
typedef struct{
    char* mask;
    char* out;
    char** in;
} Files;

//struct for storing stats related to grey image data
typedef struct {
    int count;
    double min;
    double max;
    double mean;
} Stats;

//struct used to store grey image data 
typedef struct {
    int rows;
    int cols;
    double** grid;
} GreyGrid;

//struct used to store mask data 
typedef struct {
    int max;
    int* freq;
    int rows;
    int cols;
    int** grid;
} MaskGrid;

//init files structure
void initFiles(Files* files, int argc, char** argv){
    files->mask = malloc(sizeof(char) * 256);
    files->out = malloc(sizeof(char) * 256);
    
    strcpy(files->mask, argv[1]);
    strcpy(files->out, argv[2]);

    files->in = malloc(sizeof(char*) * argc-3);
    for (int i = 0; i < argc-3; i++)
    {
        files->in[i] = malloc(sizeof(char) * 256);
        strcpy(files->in[i], argv[i+3]);
    }
}

//uninit files structure
void uninitFiles(Files* files, int argc){
    free(files->mask);
    files->mask = NULL;

    free(files->out);
    files->out = NULL;

    for (int i = 0; i < argc-3; i++)
    {
        free(files->in[i]);
        files->in[i] = NULL;
    }

    free(files->in);
    files->in = NULL;
}

//init MaskGrid structure
int initMask(MaskGrid* mask, Files* files, int argc){
    //open file
    printf("Opening '%s' for input\n", files->mask);
    FILE* file = fopen(files->mask, "r");
    if (file == NULL)
    {
        printf("ERROR: Can't open '%s' for input!\n", files->mask);
        return 0;
    }

    //get dimensions of mask
    if(!fscanf(file, "%d", &mask->rows)){
        printf("ERROR: no width found\n");
        return 0;
    }
    if(!fscanf(file, "%d", &mask->cols)){
        printf("ERROR: no height found\n");
        return 0;
    }

    //read in mask numbers
    mask-> max = 0;
    mask->freq = malloc(sizeof(int) * argc);
    for (int i = 0; i < argc; i++)
    {
        mask->freq[i] = 0;
    }

    mask->grid = malloc(sizeof(int*) * mask->rows);
    for (int x = 0; x < mask->rows; x++)
    {
        mask->grid[x] = malloc(sizeof(int) * mask->cols);
        for (int y = 0; y < mask->cols; y++)
        {
            if (!fscanf(file, "%d", &mask->grid[x][y]))
            {
                printf("ERROR: Data invalid\n");
                return 0;
            }

            mask->freq[mask->grid[x][y]]++;

            if (mask->grid[x][y] > mask->max)
            {
                mask->max = mask->grid[x][y];
            }
            
        }
    }

    //close mask file
    fclose(file);

    return 1;
}

//uninit MaskGrid structure
void uninitMask(MaskGrid* mask){
    for (int x = 0; x < mask->rows; x++)
    {
        free(mask->grid[x]);
        mask->grid[x] = NULL;
    }
    
    free(mask->grid);
    mask->grid = NULL;
    
    free(mask->freq);
    mask->freq = NULL;
}

//init GreyGrid input structure
int initInput(GreyGrid* grid, MaskGrid* mask, char* file){
    //open each input file and store grid data
    printf("Opening '%s' for input\n", file);
    
    FILE* inFile = fopen(file, "r");

    if (inFile == NULL)
    {
        printf("ERROR: Can't open '%s' for input!\n", file);
        return 0;
    }

    if(!fscanf(inFile, "%d", &grid->rows)){
        printf("ERROR: no width found\n");
        fclose(inFile);
        return 0;
    }
    if(!fscanf(inFile, "%d", &grid->cols)){
        printf("ERROR: no height found\n");
        fclose(inFile);
        return 0;
    }

    if (grid->rows != mask->rows || grid->cols != mask->cols)
    {
        printf("ERROR: Input file '%s' size %d x %d invalid!\n", file, grid->rows, grid->cols);
        fclose(inFile);
        return 0;
    }

    double temp;
    grid->grid = malloc(sizeof(double*) * grid->rows);
    for (int x = 0; x < grid->rows; x++)
    {
        grid->grid[x] = malloc(sizeof(double) * grid->cols);
        for (int y = 0; y < grid->cols; y++)
        {
            if (!fscanf(inFile, "%lf", &temp))
            {
                printf("ERROR: Data invalid\n");
                fclose(inFile);
                return 0;
            }
            grid->grid[x][y] = temp;
        }
    }

    fclose(inFile);

    return 1;
}

//uninit GreyGrid structure
void uninitGrey(GreyGrid* grid){
    for (int x = 0; x < grid->rows; x++)
    {
        free(grid->grid[x]);
        grid->grid[x] = NULL;
    }
    free(grid->grid);
}

//init GreyGrid output structure 
void initOutput(GreyGrid* out, MaskGrid* mask, GreyGrid* input, int argc){
    out->cols = mask->cols;
    out->rows = mask->rows;
    out->grid = malloc(sizeof(double*) * out->rows);
    for (int x = 0; x < out->rows; x++)
    {
        out->grid[x] = malloc(sizeof(double) * out->cols);
        for (int y = 0; y < out->cols; y++)
        {
            if (mask->grid[x][y] >= argc-3)
            {
                out->grid[x][y] = 0.0;
            } 
            else
            {
                //printf("%d ", mask->grid[x][y]);
                //fflush(stdout);
                //printf("%.6f\n", input[0].grid[x][y]);
                //fflush(stdout);
                out->grid[x][y] = input[mask->grid[x][y]].grid[x][y];
            }
        }
    }
}

//print stats to terminal
void printStats(Stats stats){
    printf("count = %d, mean = %.6f, min = %.6f, max = %.6f\n", stats.count, stats.mean, stats.min, stats.max);
}

//print info about mask
void printMaskData(MaskGrid* mask){
    //print mask size
    printf("Mask size: %d x %d\n", mask->rows, mask->cols);

    //print mask percents
    double percent = 0;
    for (int i = 0; i <= mask->max; i++)
    {
        percent = 100.0 * ((double)mask->freq[i] / (mask->rows * mask->cols));
        printf("Mask %d: %.2f%%\n", i, percent);
    }
}

//compute stats related to a given grey iamge data and return them
Stats computeStatsFromDouble2D(GreyGrid* out){
    Stats stats;
    double sum = 0;
    stats.min = 1.0;
    stats.max = 0;
    stats.count = 0;

    for (int x = 0; x < out->rows; x++)
    {
        for (int y = 0; y < out->cols; y++)
        {
            if (out->grid[x][y] < stats.min)
            {
                stats.min = out->grid[x][y];
            }

            if (out->grid[x][y] > stats.max)
            {
                stats.max = out->grid[x][y];
            }

            stats.count++;
            sum += out->grid[x][y];
        }
    }

    stats.mean = sum / stats.count;

    return stats;
}

//output grey image data to a file
int writeDouble2DArrayToFile(GreyGrid* out, char* file){
    printf("Opening '%s' for output\n", file);
    FILE* outputFile = fopen(file, "w");
    if (outputFile == NULL)
    {
        printf("Failed to open '%s' for output!\n", file);
        return 0;
    }

    fprintf(outputFile, "%d %d\n", out->rows, out->cols);

    for (int x = 0; x < out->rows; x++)
    {
        for (int y = 0; y < out->cols; y++)
        {
            fprintf(outputFile, "%.6f ", out->grid[x][y]);
        }
        fprintf(outputFile, "\n");
    }

    fclose(outputFile);

    return 1;
}

int main(int argc, char** argv){
    //assure enough arguments given
    if (argc < 4)
    {
        printf("Combine <mask file> <output file> <grey file0> [grey file1] ...\n");
        return 1;
    }
    //init collection of file names
    Files files;
    initFiles(&files, argc, argv);
    
    //init mask grid
    MaskGrid mask;
    if(!initMask(&mask, &files, argc)){
        uninitFiles(&files, argc);
        return 0;
    }

    //print info on gathered mask data
    printMaskData(&mask);
    
    //init input grids
    GreyGrid* inputGrids;
    inputGrids = malloc(sizeof(GreyGrid) * argc);
    for (int i = 0; i < argc-3; i++)
    {
        if(!initInput(&inputGrids[i], &mask, files.in[i])){
            uninitFiles(&files, argc);
            uninitMask(&mask);
            for (int n = 0; n < i; n++)
            {
                uninitGrey(&inputGrids[n]);
            }
            free(inputGrids);
            inputGrids = NULL;
            return 0;
        }
    }

    //create output
    GreyGrid out;
    initOutput(&out, &mask, inputGrids, argc);

    //output to new file
    if(!writeDouble2DArrayToFile(&out, files.out)){
        uninitFiles(&files, argc);
        uninitMask(&mask);
        for (int i = 0; i < argc-3; i++)
        {
            uninitGrey(&inputGrids[i]);
        }
        free(inputGrids);
        inputGrids = NULL;
        uninitGrey(&out);
        return 0;
    }

    //get stats
    Stats stats = computeStatsFromDouble2D(&out);

    //print stats
    printStats(stats);

    //uninit
    uninitFiles(&files, argc);
    uninitMask(&mask);
    for (int i = 0; i < argc-3; i++)
    {
        uninitGrey(&inputGrids[i]);
    }
    free(inputGrids);
    inputGrids = NULL;
    uninitGrey(&out);
}
