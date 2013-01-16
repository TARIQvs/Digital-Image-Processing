#include<stdio.h>
#define FALSE 0
#define TRUE 1
#define L 256


//General image pgm5 image "structure" with histogram
struct IMAGEPARAMETERS
{
  int  numberOfColumns, numberOfBands,cd,highVal;
	unsigned long numberOfRows,totalPixels;
	unsigned char *image;	
	unsigned int histogram[L-1];
}IMAGEPARAMETERS;


//check for image validity
struct IMAGEPARAMETERS checkimage(char* imagename)
{
	FILE* fpIn;
	char *string;
	char c;
	int doneReading = FALSE;
	struct IMAGEPARAMETERS image1;
	
        fpIn = fopen(imagename,"rb"); /* argv[1] contains the filename */
        if (fpIn == NULL) 
        {
                fprintf(stderr,"%s either cannot be read or does not exist\n", imagename);
                exit;
        }
        string = (char *) calloc(256,1);
        /* initialized dynamic memory allocation */
        while (!doneReading && (c = (char) fgetc(fpIn)) != '\0')
        switch(c)       
        {
                case 'P':
                        c = (char) getc(fpIn);
                        switch(c) 
                        {
                                case '2':
                                image1.numberOfBands= 1;
                                image1.cd=0;
                                /* ppmType = PGMASCII; */
                                break;
                                case '3':
                                image1.numberOfBands = 3;
                                image1.cd=0;
                                /* ppmType = PPMASCII; */
                                break;
                                case '5':
                                image1.numberOfBands = 1;
                                image1.cd=1;
                                /* ppmType = PGMRAW; */
                                break;
                                case '6':
                                image1.numberOfBands = 3;
                                image1.cd=1;
                                /* ppmType = PPMRAW; */
                                break;
                        }
                        c = (char) getc(fpIn);
                        if (c != 0x0A) 
                        {
                                ungetc(c,fpIn);
                        }
                        else 
                        {
                                ungetc(c,fpIn);
                                //Page 1 of 2File: /home/praveen/Desktop/Link to...ses/DIP/programs/readImage.cpp
                                fgets(string,256,fpIn);
                        }
                break;

                case '#':
                        fgets(string,256,fpIn);
                break;
                case '1':
             	case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':         
                        ungetc(c,fpIn);
                        fscanf(fpIn,"%d %lu %d", &image1.numberOfColumns, &image1.numberOfRows, &image1.highVal);
                        doneReading = TRUE;
                        fgetc(fpIn);
                break;
        }
        image1.totalPixels=image1.numberOfColumns*image1.numberOfRows*image1.numberOfBands;
free(string);	
image1.cd==1?(image1.totalPixels=image1.totalPixels):(image1.totalPixels=4*image1.totalPixels);

image1.image = (unsigned char *) malloc(image1.totalPixels);
fread(image1.image,1,image1.totalPixels,fpIn);
return(image1);
}


//Equalizes and image and stores its transform values in the histogram[]

void equalize(struct IMAGEPARAMETERS *image1 )
{
	unsigned long inc=0;
	unsigned int k,j;
	float redistribute[L-1]={0};
	
	for(k=0;k < L;k++)
		image1->histogram[k]=0;
	for(inc=0; inc < (image1->totalPixels); inc ++)
	{
		image1->histogram[*(image1->image+inc)]++;
	}
	
	for(k=0;k < L;k++)
	{
		for(j=0;j <=k;j++ )
		{
			redistribute[k]+= image1->histogram[j];
		}
		redistribute[k]*=(L-1);
		redistribute[k]/=image1->totalPixels;
	}
	for(k=0;k < L;k++)
		image1->histogram[k]=redistribute[k]+0.5;
	
	for(inc=0;inc < image1->totalPixels; inc++)
	{
		*(image1->image+inc)=image1->histogram[*(image1->image+inc)];
	}
	
		
}

//function to equalize based on given transform values
void equalizee(struct IMAGEPARAMETERS *image,struct IMAGEPARAMETERS *refferenceimage)
{
	unsigned long inc=0;
	unsigned int k,j, distribution[L-1]={0};
		
	for(inc=0;inc < image->totalPixels; inc++)
	{
		*(image->image+inc)=refferenceimage->histogram[*(image->image+inc)];
	}
		
}

//draws a vertical bar from given x,y coordinates originating from top left corner and y is increasing down wards
void verti_bar(struct IMAGEPARAMETERS *image,int width,char colour,unsigned int x, unsigned int y)
{
	unsigned int i,j;
	for(i=y;i<image->numberOfRows;i++)
	{	
		for(j=0;j<width;j++)
		*(image->image+(i*image->numberOfColumns)+x+j)=colour;
	}
	
}


//inverts a black and white image to its color inverts
void colorinvert(struct IMAGEPARAMETERS image)
{
	unsigned int i;
	for(i=0;i<image.totalPixels;i++)
	{
		*(image.image+i)=255-*(image.image+i);
	}
}


//divides the whole by 2;
void colordown(unsigned char *image,unsigned int nopixels)
{
	unsigned int i;
	for(i=0;i<nopixels;i++)
	{
		*(image+i)=*(image+i)/2;
	}
}
 

//function to create an image based on the image array, breadth and height
void makeimage(struct IMAGEPARAMETERS image,char *imagename)
{
FILE *fpOut;
fpOut = fopen(imagename,"wb");
fprintf(fpOut,"P%d\n%d %lu\n255\n",5, image.numberOfColumns, image.numberOfRows);
fwrite(image.image,1,image.totalPixels,fpOut);

}


//creates a histogram in a blank image
void histogram(struct IMAGEPARAMETERS image,char* histplotname)
{
	unsigned int cuml;
	unsigned long max=0;
	unsigned char *imagematrix;
	int i,c;
	for(cuml=0;cuml<image.totalPixels;cuml++)
	{
		c=*(image.image+cuml);
		(*(image.histogram+c))++;
	}
	for(i=0;i<L;i++)
		for(i=0;i<L;i++) 
	{
		if(max<image.histogram[i])
			max=image.histogram[i];
		
			
	}
	
	for(i=0;i<L;i++)
	{
		image.histogram[i]=image.histogram[i]/20;
	}
	max=max/20+10;
	
	image.totalPixels=256*2*(max);
	image.numberOfRows=max;
	image.numberOfColumns=256*2;
	
	image.image= (unsigned char *) malloc(image.totalPixels);
	for(i=0;i<256;i++)
	{
		verti_bar(&image, 1,255,i*2,max-image.histogram[i]);
	}
	colorinvert(image);
	makeimage(image,histplotname);
	
}
                        
