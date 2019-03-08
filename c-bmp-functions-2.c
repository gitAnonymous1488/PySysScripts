#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdint.h>
#include<math.h>

#define TRUE 1
#define FALSE 0

#define DEVELOPING 1

// #if DEVELOPING

	#define WIDTH 			1000
	#define HEIGHT 			600
	#define BMPARRAYSIZE 	WIDTH * HEIGHT
	#define BMPPADDARRAY    BMPARRAYSIZE * 3
	#define BMPHDRSIZE		14
	#define BMPINFOSIZE		40
	#define BMPQUADSIZE		16

// #endif

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define ABS(a) (MAX(a,-1*a))

// So, you have to count everything individually becase
// It will give you a 2 byte buffer between 
// bfType and bfSize
typedef struct BMPHeader {
	uint16_t 	bfType;				//	2	must always be set to 'BM' to declare that this is a .bmp-file.													
	uint32_t 	bfSize;				//	4	specifies the size of the file in bytes.
	uint16_t 	bfReserved1;		//	2	must always be set to zero.
	uint16_t 	bfReserved2;		//	2	must always be set to zero.
	uint32_t	bfOffBits;			//	4	specifies the offset from the beginning of the file to the bitmap data.
} BMPHeader;						//	14

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
} BMPInfo;							//	40

typedef struct BMPQuad {
	uint8_t rgbBlue;				// 	4	specifies the blue part of the color.
	uint8_t rgbGreen;				//	4	specifies the green part of the color.
	uint8_t rgbRed;					//	4	specifies the red part of the color.
	uint8_t rgbReserved;			//  4	must always be set to zero.
} BMPQuad;

typedef struct BMPContainer {
	BMPHeader bmp_header;
	BMPInfo   bmp_info;
	BMPQuad   bmp_quad;
	int * 	  bmp_array;
} BMPContainer;

void convert_big_endian(uint32_t * i_conv) {
	uint32_t b0,b1,b2,b3;
	uint32_t res;

	b0 = (*i_conv & 0x000000ff) << 24u;
	b1 = (*i_conv & 0x0000ff00) << 8u;
	b2 = (*i_conv & 0x00ff0000) >> 8u;
	b3 = (*i_conv & 0xff000000) >> 24u;

	*i_conv = b0 | b1 | b2 | b3;
}

int get_offest_amount() {
	return BMPHDRSIZE + BMPINFOSIZE + BMPQUADSIZE;
}

void create_base(BMPHeader * bmp_header) {
// #if DEVELOPING
	uint16_t bfType			= 0x4D42;
	uint32_t bfSize         = BMPPADDARRAY + get_offest_amount();
	uint16_t bfReserved1    = 0;
	uint32_t bfOffBits      = get_offest_amount();
// #endif

	bmp_header->bfType      	= bfType;
	bmp_header->bfSize      	= bfSize;
	bmp_header->bfReserved1 	= bfReserved1;
	bmp_header->bfReserved2 	= bfReserved1;
	bmp_header->bfOffBits   	= bfOffBits;
}

void write_base(BMPHeader * bmp_header, int fid) {
	int written;
	if ((written = write(fid, &bmp_header->bfType, sizeof(bmp_header->bfType))) < 0)
		exit(1);
	if ((written = write(fid, &bmp_header->bfSize, sizeof(bmp_header->bfSize))) < 0)
		exit(1);
	if ((written = write(fid, &bmp_header->bfReserved1, sizeof(bmp_header->bfReserved1))) < 0)
		exit(1);
	if ((written = write(fid, &bmp_header->bfReserved2, sizeof(bmp_header->bfReserved2))) < 0)
		exit(1);
	if ((written = write(fid, &bmp_header->bfOffBits, sizeof(bmp_header->bfOffBits))) < 0)
		exit(1);
}

void create_info(char * bmp_info) {
// #if DEVELOPING
	uint32_t biSize          	= sizeof(BMPInfo);
	uint32_t biWidth         	= WIDTH;
	uint32_t biHeight        	= HEIGHT;
	uint16_t biPlanes      		= 1;
	uint16_t biBitCount    		= 24;
	uint32_t biCompression   	= 0;
	uint32_t biSizeOfImage   	= BMPPADDARRAY;
	uint32_t biXPelsPerMeter 	= 0;
	uint32_t biYPelsPerMeter 	= 0;
	uint32_t biClrUsed       	= 0;
	uint32_t biClrImportant  	= 0;
// #endif

	convert_big_endian(&biSize);
	convert_big_endian(&biWidth);
	convert_big_endian(&biHeight);
	convert_big_endian(&biCompression);
	convert_big_endian(&biSizeOfImage);
	convert_big_endian(&biXPelsPerMeter);
	convert_big_endian(&biYPelsPerMeter);
	convert_big_endian(&biClrUsed);
	convert_big_endian(&biClrImportant);

// biSize 			Big Endian

	bmp_info[0] = (biSize>>24) & 0xFF;
	bmp_info[1] = (biSize>>16) & 0xFF;
	bmp_info[2] = (biSize>>8) & 0xFF;
	bmp_info[3] = biSize & 0xFF;

// biWidth 			Big Endian

	bmp_info[4] = (biWidth>>24) & 0xFF;
	bmp_info[5] = (biWidth>>16) & 0xFF;
	bmp_info[6] = (biWidth>>8) & 0xFF;
	bmp_info[7] = biWidth & 0xFF;

// biHeight			Big Endian

	bmp_info[8] = (biHeight>>24) & 0xFF;
	bmp_info[9] = (biHeight>>16) & 0xFF;
	bmp_info[10] = (biHeight>>8) & 0xFF;
	bmp_info[11] = biHeight & 0xFF;

// biPlanes			Little Endian

	bmp_info[12] = biPlanes & 0xFF; 		
	bmp_info[13] = (biPlanes>>8) & 0xFF; 

// biBitCount 		Little Endian

	bmp_info[14] = biBitCount & 0xFF;		
	bmp_info[15] = (biBitCount>>8) & 0xFF; 	

// biCompression 	Big Endian

	bmp_info[16] = (biCompression>>24) & 0xFF;
	bmp_info[17] = (biCompression>>16) & 0xFF;
	bmp_info[18] = (biCompression>>8) & 0xFF;
	bmp_info[19] = biCompression & 0xFF;

// biSizeOfImage 	Big Endian

	bmp_info[20] = (biSizeOfImage>>24) & 0xFF;
	bmp_info[21] = (biSizeOfImage>>16) & 0xFF;
	bmp_info[22] = (biSizeOfImage>>8) & 0xFF;
	bmp_info[23] = biSizeOfImage & 0xFF;

// biXPelsPerMeter 	Big Endian

	bmp_info[24] = (biXPelsPerMeter>>24) & 0xFF;
	bmp_info[25] = (biXPelsPerMeter>>16) & 0xFF;
	bmp_info[26] = (biXPelsPerMeter>>8) & 0xFF;
	bmp_info[27] = biXPelsPerMeter & 0xFF;

// biYPelsPerMeter 	Big Endian

	bmp_info[28] = (biYPelsPerMeter>>24) & 0xFF;
	bmp_info[29] = (biYPelsPerMeter>>16) & 0xFF;
	bmp_info[30] = (biYPelsPerMeter>>8) & 0xFF;
	bmp_info[31] = biYPelsPerMeter & 0xFF;

// biClrUsed 		Big Endian

	bmp_info[32] = (biClrUsed>>24) & 0xFF;
	bmp_info[33] = (biClrUsed>>16) & 0xFF;
	bmp_info[34] = (biClrUsed>>8) & 0xFF;
	bmp_info[35] = biClrUsed & 0xFF;

// biClrImportant 	Big Endian

	bmp_info[36] = (biClrImportant>>24) & 0xFF;
	bmp_info[37] = (biClrImportant>>16) & 0xFF;
	bmp_info[38] = (biClrImportant>>8) & 0xFF;
	bmp_info[39] = biClrImportant & 0xFF;
}

void create_color_pallete(char * bmp_quad_array) {
// #if DEVELOPING
	uint8_t rgbBlue     	= 0xFF;
	uint8_t rgbGreen    	= 0xFF;
	uint8_t rgbRed      	= 0xFF;
	uint8_t rgbReserved 	= 0xFF;
// #endif

	uint8_t zero 			= 0x00;

// rgbRed 	Big Endian
	bmp_quad_array[0] 		= zero;
	bmp_quad_array[1] 		= zero;
	bmp_quad_array[2] 		= rgbRed;
	bmp_quad_array[3] 		= zero;

// rgbGreen 	Big Endian
	bmp_quad_array[4] 		= zero;
	bmp_quad_array[5] 		= rgbGreen;
	bmp_quad_array[6] 		= zero;
	bmp_quad_array[7] 		= zero;

// rgbBlue 	Big Endian
	bmp_quad_array[8] 		= rgbBlue;
	bmp_quad_array[9] 		= zero;
	bmp_quad_array[10] 		= zero;
	bmp_quad_array[11] 		= zero;

// rgbReserved 	Big Endian
	bmp_quad_array[12] 		= zero;
	bmp_quad_array[13] 		= zero;
	bmp_quad_array[14] 		= zero;
	bmp_quad_array[15] 		= rgbReserved;
}


// DRAWING
void initialize_bmp(char * bmp_array) {
	uint8_t grey = 0xCC;
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			bmp_array[i * WIDTH * 3 + 3 * j]		= grey;
			bmp_array[i * WIDTH * 3 + 3 * j + 1]	= grey;
			bmp_array[i * WIDTH * 3 + 3 * j + 2]	= grey;
		}
	}
}

// DRAWING
void draw_straight_line(int x1, int y1, int x2, int y2, char * bmp_array, uint32_t color) {
	int epsilon = x2 < x1 ? -1 : 1;
	int diff = x2 < x1 ? x1 - x2 : x2 - x1;
	int x, y;
	double delta =  ((double) (y2 - y1)) / (diff);

	for (int i = 0; i < diff; i++) {
		x = (int) (x1 + floor(i * epsilon));
		y = (int) (y1 + floor(i * delta));

		if ((x == 0 || x == WIDTH) && (y == 0 || y == HEIGHT)) {
			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * x]		= ((color >> 24) & 0xFF);
			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * x + 1]	= ((color >> 16) & 0xFF);
			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * x + 2]	= ((color >> 8) & 0xFF);
		}

		if ((0 <= x && x <= WIDTH) && (1 < y && y < HEIGHT)) {
			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * x]		= ((color >> 24) & 0xFF);
			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * x + 1]	= ((color >> 16) & 0xFF);
			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * x + 2]	= ((color >> 8) & 0xFF);

			bmp_array[((HEIGHT - y) - 1) * WIDTH * 3  + 3 * x]		= ((color >> 24) & 0xFF);;
			bmp_array[((HEIGHT - y) - 1) * WIDTH * 3  + 3 * x + 1]	= ((color >> 16) & 0xFF);;
			bmp_array[((HEIGHT - y) - 1) * WIDTH * 3  + 3 * x + 2]	= ((color >> 8) & 0xFF);;

			bmp_array[((HEIGHT - y) + 1) * WIDTH * 3  + 3 * x]		= ((color >> 24) & 0xFF);;
			bmp_array[((HEIGHT - y) + 1) * WIDTH * 3  + 3 * x + 1]	= ((color >> 16) & 0xFF);;
			bmp_array[((HEIGHT - y) + 1) * WIDTH * 3  + 3 * x + 2]	= ((color >> 8) & 0xFF);;
		}

		if ((1 < x && x < WIDTH) && (0 <= y && y <= HEIGHT)) {
			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * x]		= ((color >> 24) & 0xFF);;
			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * x + 1]	= ((color >> 16) & 0xFF);;
			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * x + 2]	= ((color >> 8) & 0xFF);;

			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * (x - 1)]		= ((color >> 24) & 0xFF);;
			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * (x - 1) + 1]	= ((color >> 16) & 0xFF);;
			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * (x - 1) + 2]	= ((color >> 8) & 0xFF);;

			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * (x + 1)]		= ((color >> 24) & 0xFF);;
			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * (x + 1) + 1]	= ((color >> 16) & 0xFF);;
			bmp_array[(HEIGHT - y) * WIDTH * 3  + 3 * (x + 1) + 2]	= ((color >> 8) & 0xFF);;
		}
	}

}

int main() {
	int fid, written, closed;
	char * bmp_file_name = "testing.bmp";
	char bmp_info_array[40];
	char bmp_quad_array[BMPQUADSIZE];
	char * bmp_array = (char * ) malloc(BMPPADDARRAY);

	BMPHeader bmp_header;
	BMPQuad bmp_quad;

	create_base(&bmp_header);
	create_info(bmp_info_array);
	create_color_pallete(bmp_quad_array);

	initialize_bmp(bmp_array);
	draw_straight_line(0, 0, 100, 100, bmp_array, 0xFF00FF00);

	// draw_straight_line(100, 0, 0, 100, bmp_array, 0xFF000000);


	if ((fid = open(bmp_file_name,O_WRONLY | O_CREAT | O_TRUNC, 0777)) < -1)
		exit(1);

	// HEADER
	write_base(&bmp_header,fid);

	// INFO
	if ((written = write(fid, bmp_info_array, BMPINFOSIZE)) < BMPINFOSIZE)
		exit(1);

	// QUAD COLOR INTERP
	if ((written = write(fid, bmp_quad_array, BMPQUADSIZE)) < BMPQUADSIZE)
		exit(1);

	// ACTUAL 
	if ((written = write(fid, bmp_array, BMPPADDARRAY)) < BMPPADDARRAY)
		exit(1);


	if ((closed = close(fid)) < 0)
		exit(1);

	return 0;
}