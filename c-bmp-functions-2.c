#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdint.h>

#define TRUE 1
#define FALSE 0

#define DEVELOPING 1

#if DEVELOPING

	#define WIDTH 1000
	#define HEIGHT 600
	#define BMPARRAYSIZE WIDTH * HEIGHT * 8

#endif


typedef struct BMPHeader {
	uint16_t 	bfType;				//	2	must always be set to 'BM' to declare that this is a .bmp-file.													
	uint32_t 	bfSize;				//	4	specifies the size of the file in bytes.
	uint16_t 	bfReserved1;		//	2	must always be set to zero.
	uint16_t 	bfReserved2;		//	2	must always be set to zero.
	uint32_t	bfOffBits;			//	4	specifies the offset from the beginning of the file to the bitmap data.
} BMPHeader;

typedef struct BMPInfo {
	uint32_t 	biSize;				//	4	specifies the size of the BITMAPINFOHEADER structure, in bytes.
	uint32_t	biWidth;			//  4	specifies the width of the image, in pixels.
	uint32_t 	biHeight;			//  4	specifies the height of the image, in pixels.
	uint16_t	biPlanes;			//  2	specifies the number of planes of the target device, must be set to zero.
	uint16_t 	biBitCount;			//  2	specifies the number of bits per pixel.
	uint32_t 	biCompression;		//  4	Specifies the type of compression, usually set to zero (no compression).
	uint32_t 	biSizeOfImage;		//  4	specifies the size of the image data, in bytes. If there is no compression, it is valid to set this member to zero.
	uint32_t 	biXPelsPerMeter;	//  4	specifies the the horizontal pixels per meter on the designated targer device, usually set to zero.
	uint32_t 	biYPelsPerMeter;	//  4	specifies the the vertical pixels per meter on the designated targer device, usually set to zero.
	uint32_t	biClrUsed;			//  4	specifies the number of colors used in the bitmap, if set to zero the number of colors is calculated using the biBitCount member.
	uint32_t	biClrImportant;		//  4	specifies the number of color that are 'important' for the bitmap, if set to zero, all colors are important.
} BMPInfo;

typedef struct BMPQuad {
	uint8_t rgbBlue;				// 	1	specifies the blue part of the color.
	uint8_t rgbGreen;				//	1	specifies the green part of the color.
	uint8_t rgbRed;					//	1	specifies the red part of the color.
	uint8_t rgbReserved;			//  1	must always be set to zero.
} BMPQuad;

void create_base(BMPInfo * bmpInfo, BMPHeader * bmpHeader) {
// #if DEVELOPING
	int bfSize = BMPARRAYSIZE + sizeof(BMPHeader) + sizeof(BMPInfo) + sizeof(BMPQuad);
// #endif

}

int main() {
	return 0;
}