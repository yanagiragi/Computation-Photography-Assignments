#ifndef  _OPENGL_WINDOW_H
#define _OPENGL_WINDOW_H

#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Button.H>

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>

typedef struct {
	float x,y;
}Vector2;

typedef struct cell{
	char* name; // for debug
	float height, width;
	Vector2 base, joint;
	struct cell* parent;
	float dof; // value storing degree of freedom
	float nowAngle; // nowAngle = mid(nowAngle,parent->base.angle)
	float angleOffset;
}node;

class openGL_window : public Fl_Gl_Window { 
	static void Timer_CB(void *userdata)
	{
			openGL_window *pb = (openGL_window*)userdata;
			pb->redraw();
			Fl::repeat_timeout(1.0/24.0, Timer_CB, userdata);
	}

	void draw() ;
	void draw_overlay();
	int handle_mouse(int event, int button, int x, int y);

	public:
			openGL_window(int x,int y,int w,int h,const char *l=0) : Fl_Gl_Window(x,y,w,h,l) 
			{
					mode( FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL );
					Fl::add_timeout(1.0/24.0, Timer_CB, (void*)this);
					frame = 0;
					
					glViewport(x,y,w,h);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();

					width = w;
					height = h;
					isAuto = isReverse = false;

					InitNode();
			}
			void InitNode();
			void calculateVertex();
			void rotateVertex(int,float);
			void scaleUpVertex(int, float);
			void scaleDownVertex(int, float);
			void scaleVertex(int, float);
			float clamp(float,float,float);
			float magnitude(Vector2);
			int handle(int event);
			
			int frame;
			int width, height;
			int choose;
			bool isAuto, isReverse;
			node drawcell[10];
};

#endif