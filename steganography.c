/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	if (image == NULL) {
		printf("error: no image found");
		return NULL;
	}
	Color *newcolor = malloc(sizeof(Color));
	if (image->image[row][col].B % 2 == 0) {
		newcolor->R = 0;
		newcolor->G = 0;
		newcolor->B = 0;
	}
	else {
		newcolor->R = 255;
        newcolor->G = 255;
        newcolor->B = 255;
    }
	return newcolor;
	
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	if (image == NULL) {
		printf("error; no image found");
		return NULL;
	}
	Image *hiddenmessage = malloc(sizeof(Image));
	hiddenmessage->rows = image->rows;
	hiddenmessage->cols = image->cols;
	hiddenmessage->image = malloc(sizeof(Color *) * hiddenmessage->rows);
	for (int i = 0; i < image->rows; i++) {
		hiddenmessage->image[i] = (Color *) malloc(sizeof(Color) * hiddenmessage->cols);
		for (int j = 0; j < image->cols; j++) {
			Color newcolor = *evaluateOnePixel(image, i, j);
			hiddenmessage->image[i][j] = newcolor;
		}
	}
	return hiddenmessage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	if (argc <= 1) {
		return -1;
	}
	Image *imagein = readData(argv[1]);
	if (imagein != NULL) {
		Image *imageout = steganography(imagein);
		if (imageout != NULL) {
			writeData(imageout);
			freeImage(imagein);
			freeImage(imageout);
			return 0;		
		}
	free(imagein);	
	}
	return -1;
}
