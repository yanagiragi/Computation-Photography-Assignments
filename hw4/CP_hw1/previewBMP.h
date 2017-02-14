#ifndef  _PREVIEWBMP_H
#define  _PREVIEWBMP_H

#include "bmpLoader.h"

class previewBMP : public Fl_Gl_Window { 
	static void Timer_CB(void *userdata)
	{
			previewBMP *pb = (previewBMP*)userdata;
			pb->redraw();
			Fl::repeat_timeout(1.0/1.0, Timer_CB, userdata);
	}

	public:
			previewBMP(int x,int y,int w,int h,const char *l=0) : Fl_Gl_Window(x,y,w,h,l) 
			{
					mode( FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL );
					Fl::add_timeout(1.0/1.0, Timer_CB, (void*)this);
					frame = 0;					
					width = w;
					height = h;
					bmp.pixels = NULL;
					bmp_w =  bmp_h = 128;
					glViewport(x,y,w,h);
					change(0);
			}
			
			void draw() ;
			void change(int i);

			int frame, width, height, bmp_w, bmp_h;
			bmpLoader bmp;			
			
};

#endif