#ifndef _BMPLOADER_H
#define _BMPLOADER_H

#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>

class bmpLoader
{
	public:
			BYTE * pixels;
			bool BMP_Read(const char *filename, unsigned char **data, int &width, int &height);			
			
};

#endif