// CREATED BY TARIQ V.S.
// 16-01-13

/* The this is the equalization program and is modified from the "readimage.c" */
#include <stdio.h> /* This header file defines the I/O functions */
#include <stdlib.h> /* include standard dfeinitions like exit() */
#include "generalfunctions.h"




int main(int argc, char **argv)
{
FILE *fpIn;
FILE *fpOut;
struct IMAGEPARAMETERS image1,image2;

unsigned long contrast[256]={0},max;
int i=0;


image1=checkimage(argv[1]);


/*Reads a refference image; Plots the histogram; Equalize it 3 times; creates a 3 times equalized image; plots its histogram; takes the argument 2; creates an image; equalize it with values of the first image; plots it; plots its histogram */
makeimage(image1,"refferenceimage.pgm"); // contained in graphics.h LOCAL header file
equalize(&image1);  		    // contained in the histogram.h LOCAL header file
equalize(&image1);
equalize(&image1);
makeimage(image1,"equalizedimage(3times equalized).pgm");
histogram(image1,"histequalized.pgm");


image2=checkimage(argv[2]);
makeimage(image2,"inputimage.pgm");
//histogram(image2,"inputimagehist5.pgm");
equalizee(&image2,&image1);
makeimage(image2,"reequalizedinputimage5.pgm");
//histogram(image2,"reiihist6.pgm");




}


