#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>

#include "glut.h"
#include "openGL_window.h"
#include <stdio.h>
#include <time.h>

int choose = 1;

void init_btn_cb(Fl_Widget *widget, void *window){
	float increment = 0.02;
	((openGL_window*)window)->InitNode();
}

void posLeft_btn_cb(Fl_Widget *widget, void *window){
	float increment = 0.02;
	((openGL_window*)window)->scaleUpVertex(choose,increment);
}

void posRight_btn_cb(Fl_Widget *widget, void *window){
	float increment = -0.02;
	((openGL_window*)window)->scaleDownVertex(choose,increment);
}

void rotLeft_btn_cb(Fl_Widget *widget, void *window){
	((openGL_window*)window)->rotateVertex(choose,2);
}

void rotRight_btn_cb(Fl_Widget *widget, void *window){
	((openGL_window*)window)->rotateVertex(choose,-2);
}


void auto_btn_cb(Fl_Widget *widget, void *window){
	((openGL_window*)window)->isAuto = !((openGL_window*)window)->isAuto;
}

void choice1_cb(Fl_Widget *o, void *p)   // choice callback function
{
	int temp=int(((Fl_Choice*)o)->value());
	openGL_window *gl_win = (openGL_window *)p;
	if(temp != 0)
		choose = temp + 2;
	else
		choose = temp + 1;
}

//  main function
int main(int argc, char **argv) {

	srand(time(NULL));

	Fl_Window window(640 , 520);  // Create a FLTK window. Resolution 600*400. 	

	openGL_window gl_win(20, 20, 600 ,400);   // Add openGL window in position (10,10).Resolution 600*400
	// window.resizable(&gl_win);
	
	Fl_Choice *choice1=(Fl_Choice *)0;  
	Fl_Menu_Item menu_choice1[] = {  // Create choice menu
	 {"Neck", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"LeftForeArm", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"LeftArm", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"RightForeArm", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"RightArm", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"Hips", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"LeftLeg", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"RightLeg", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {0,0,0,0,0,0,0,0,0}
	};

	
	Fl_Widget *init_btn;
	init_btn = new Fl_Button(20,430,100,20,"Init");
	init_btn->callback(init_btn_cb,&gl_win);

	choice1 = new Fl_Choice(20, 460 , 100 , 20);    // Add Choice ui in position (200,420). width=100 height=20  
	choice1->down_box(FL_BORDER_BOX);
	choice1->menu(menu_choice1);
	choice1->callback(choice1_cb,&gl_win);


	Fl_Widget *posLeft_btn;
	posLeft_btn = new Fl_Button(310,430,100,20,"[Scale] Up ");
	posLeft_btn->callback(posLeft_btn_cb,&gl_win);

	Fl_Widget *rotLeft_btn;
	rotLeft_btn = new Fl_Button(140,460,150,20,"[Rotate] CounterCW ");
	rotLeft_btn->callback(rotLeft_btn_cb,&gl_win);
	
	Fl_Widget *auto_btn;
	auto_btn = new Fl_Button(310,460,100,20,"Auto");
	auto_btn->callback(auto_btn_cb,&gl_win);
	Fl_Widget *rotRight_btn;

	rotRight_btn = new Fl_Button(430,460,150,20,"[Rotate] ClockWise ");
	rotRight_btn->callback(rotRight_btn_cb,&gl_win);
	
	Fl_Widget *posRight_btn;
	posRight_btn = new Fl_Button(310,490,100,20,"[Scale] Down");
	posRight_btn->callback(posRight_btn_cb,&gl_win);

	window.end();                  // End of FLTK windows setting. 
	window.show(argc,argv);        // Show the FLTK window

	gl_win.show();                 // Show the openGL window
  
	gl_win.redraw_overlay();       // redraw 
	return Fl::run();
}