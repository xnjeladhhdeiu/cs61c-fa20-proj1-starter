/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
//
//helper for evaluateonecell
int checkneighbour (Image *image, int row, int col) {
	if (image->image[row][col].R != 0) {
		return 1;
	}
	else {
		return 0;
	}
}


Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	uint8_t n_neigbours_alive = 0;
	uint8_t alive = image->image[row][col].R;
	int nrow = row;
	int ncol = col;
	for (int i = -1; i < 2; i++) {
		if (row + i < 0) {
			nrow = image->rows -1;
		}
		else if (row + i > image->rows - 1) {
			nrow = 0;
		}
		else {
			nrow = row + i;
		}
		for (int j = -1; j < 2; j++) {
			if (col + j < 0) {
				ncol = image->cols -1;
			}
			else if (col + j > image->cols - 1) {
				ncol = 0;
			}
			else {
				ncol = col + j;
			}
			if (i != 0 && j != 0) {
				n_neigbours_alive += checkneighbour(image, nrow, ncol);
			}
		}
	}

	/*create the cell and decide if it lives or dies */
	Color *newcolor = malloc(sizeof(Color));
	newcolor->R = 255;
	newcolor->G = 255;
	newcolor->B = 255;
	/*live cell stays alive if live neighbours adhere to the rule in the nt bit*/
	/*create mask to check */
	if (alive) {
		for (int i  = 0; i <= 8; i++) {
			uint32_t mask = (1 << i);	
			if (n_neigbours_alive == i && (rule & mask) != 0) {
				return newcolor;
			}		
		}
	}
	/* dead cell revives if live neigbours == 3 */
	else if (!alive) {
		for (int i = 9; i <= 18; i++) {
			uint32_t mask = (1 << i);
			if (n_neigbours_alive == i - 9 && (rule & mask) != 0) {
				return newcolor;
			}
		}
	}
	/* all other cells die or stay dead */
	else {
		newcolor->R = 0;
		newcolor->G = 0;
		newcolor->B = 0;
	}
	return newcolor;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	if (image == NULL) {
		printf("error; no image found");
		return NULL;
	}
	Image *nextiteration = malloc(sizeof(Image));
	nextiteration->rows = image->rows;
	nextiteration->cols = image->cols;
	nextiteration->image = malloc(sizeof(Color *) * nextiteration->rows);
	for (int i = 0; i < image->rows; i++) {
		nextiteration->image[i] = (Color *) malloc(sizeof(Color) * nextiteration->cols);
		for (int j = 0; j < image->cols; j++) {
			Color newcolor = *evaluateOneCell(image, i, j, rule);
			nextiteration->image[i][j] = newcolor;
		}
	}
	return nextiteration;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc <= 2) {
		printf(" ./gameOfLife\nusage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.\n");
		return -1;
	}
	Image *imagein = readData(argv[1]);
	if (imagein != NULL) {
		char *endptr;
		Image *imageout = life(imagein, strtol(argv[2], &endptr, 16));
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
