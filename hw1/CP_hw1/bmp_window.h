#ifndef _BMP_WINDOW_H
#define _BMP_WINDOW_H

#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>

class bmp_window : public Fl_Gl_Window { 
	static void Timer_CB(void *userdata)
	{
			bmp_window *pb = (bmp_window*)userdata;
			pb->redraw();
			Fl::repeat_timeout(1.0/24.0, Timer_CB, userdata);
	}

	void draw() ;
	void draw_overlay();
	
	public:
			bmp_window(int x,int y,int w,int h,const char *l=0) : Fl_Gl_Window(x,y,w,h,l) 
			{
					mode( FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL );
					Fl::add_timeout(1.0/24.0, Timer_CB, (void*)this);
					frame = 0;
					width = w;
					height = h;
					glViewport(x,y,w,h);
					isLoadBmp = false;
					bmp_w = bmp_h  = 512;
					pixels = (BYTE *)malloc(sizeof(BYTE) * bmp_w);
			}
			void loadbmp();
			bool BMP_Read(const char *filename, unsigned char **data, int &width, int &height);
			void drawPoint(float x, float y, float r, float g, float b);

			int frame, width, height , bmp_w , bmp_h;
			bool  isLoadBmp;
			BYTE * pixels;
};


#endif