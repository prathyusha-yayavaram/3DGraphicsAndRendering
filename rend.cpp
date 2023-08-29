#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
/*   CS580 HW   */
#include    "stdafx.h"  
#include	"Gz.h"


GzRender::GzRender(int xRes, int yRes)
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- set display resolution
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- allocate memory for pixel buffer
 */
	this->xres = min(xRes, MAXXRES);
	this->yres = min(yRes, MAXYRES);
	framebuffer = new char[3 * xres * yres];
	pixelbuffer = new GzPixel[xres * yres];
}

GzRender::~GzRender()
{
/* HW1.2 clean up, free buffer memory */
	delete framebuffer;
	delete pixelbuffer;
}

int GzRender::GzDefault()
{
/* HW1.3 set pixel buffer to some default valuese - start a new frame */
	for (int i = 0; i < xres * yres; i++) {
		pixelbuffer[i] = { 1600,1400,1300,1,0 };
		framebuffer[3 * i] = '1600';
		framebuffer[3 * i + 1] = '1400';
		framebuffer[3 * i + 2] = '1300';
	}
	return GZ_SUCCESS;
}


int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
/* HW1.4 write pixel values into the buffer */
	if (i >= 0 && i < xres && j >= 0 && j < yres) {
		int index = ARRAY(i, j);
		pixelbuffer[index] = { r,g,b,a,z };
	}
	return GZ_SUCCESS;
}


int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
/* HW1.5 retrieve a pixel information from the pixel buffer */
	if (i >= 0 && i < xres && j >= 0 && j < yres) {
		int index = ARRAY(i, j);
		*r = pixelbuffer[index].red;
		*g = pixelbuffer[index].green;
		*b = pixelbuffer[index].blue;
		*a = pixelbuffer[index].alpha;
		*z = pixelbuffer[index].z;
	}
	return GZ_SUCCESS;
}


int GzRender::GzFlushDisplay2File(FILE* outfile)
{
/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */
	fprintf(outfile, "P6 %d %d 255\n", xres, yres);
	for (int i = 0; i < xres * yres; i++) {
		GzPixel pixel = pixelbuffer[i];
		GzIntensity red = pixel.red;
		GzIntensity blue = pixel.blue;
		GzIntensity green = pixel.green;

		red = max(0, red);
		red = min(red, 4095);
		red = red >> 4;

		blue = max(0, blue);
		blue = min(blue, 4095);
		blue = blue >> 4;

		green = max(0, green);
		green = min(green, 4095);
		green = green >> 4;

		char color[3] = { (char)(red & 0b11111111), (char)(green & 0b11111111), (char)(blue & 0b11111111) };
		fwrite(color, 1, 3, outfile);
	}
	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer()
{
/* HW1.7 write pixels to framebuffer: 
	- put the pixels into the frame buffer
	- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red 
	- NOT red, green, and blue !!!
*/
	for (int i = 0; i < xres * yres; i++) {
		GzPixel pixel = pixelbuffer[i];
		GzIntensity red = pixel.red;
		GzIntensity blue = pixel.blue;
		GzIntensity green = pixel.green;

		red = max(0, red);
		red = min(red, 4095);
		red = red >> 4;

		blue = max(0, blue);
		blue = min(blue, 4095);
		blue = blue >> 4;

		green = max(0, green);
		green = min(green, 4095);
		green = green >> 4;

		framebuffer[3 * i] = (char)(blue & 0b11111111);
		framebuffer[3 * i + 1] = (char)(green & 0b11111111);
		framebuffer[3 * i + 2] = (char)(red & 0b11111111);
	}
	return GZ_SUCCESS;
}