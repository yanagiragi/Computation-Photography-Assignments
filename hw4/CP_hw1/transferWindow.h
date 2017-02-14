#ifndef _TRANS_H
#define _TRANS_H

#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include <math.h>
#include "ImageQuilt.h"

class transferWindow : public Fl_Gl_Window 
{
	static void Timer_CB(void *userdata)
	{
		transferWindow *pb = (transferWindow*)userdata;
		pb->redraw();
		Fl::repeat_timeout(1.0/1.0, Timer_CB, userdata);
	}

	public:
		transferWindow(BYTE* raws, int rawswidth, int rawsheight, BYTE* targets, int targetwidth, int targetheight, int x, int y, int w, int h, const char *l=0) : Fl_Gl_Window(x,y,w,h,l)
		{
			mode( FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL );
			Fl::add_timeout(1.0/24.0, Timer_CB, (void*)this);
			glViewport(x,y,w,h);

			width = w;
			height = h;
			raw_w = rawswidth;
			raw_h = rawsheight;
			tar_w = targetwidth;
			tar_h = targetheight;
			tileOff = .3;
			
			org_raw = raws;
			org_tar = targets;

			parse();

			synthesis();

			printf("Done\n");
			
		}

		void draw();
		void synthesis();
		void parse();
		BYTE* transferWindow::grab(int x, int y, int len);
		BYTE* transferWindow::selectPatch(int fromx, int fromy, int tox, int toy);
		BYTE* transferWindow::selectInitPatch(int fromx, int fromy, int tox, int toy);
		float transferWindow::calcErrFromBorder(int new_x, int new_y, BYTE *newcul);
		float transferWindow::calcErrFromAll(int new_x, int new_y, BYTE *newcul);
		double sqrt(int x) { return x == 0 ?  0 : sqrt((double)x); }

		BYTE* org_raw;
		BYTE* org_tar;
		int width, height, raw_w, raw_h, tar_w, tar_h, cul_w, cul_h;
		int g_x, g_y, org_g_x, org_g_y;
		float tileOff;
};

#endif