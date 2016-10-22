#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>

#include "openGL_window.h"
#include "bmp_window.h"
#include <stdio.h>
#include <time.h>

//-------Button callback funtion
typedef struct glwins{
		openGL_window *win;
		bmp_window *win2 ;
}glwin;

void idle_btn_cb(Fl_Widget *widget, void *window){
	printf("Mode : Idle\n");
	((openGL_window*)((glwin*)window)->win)->tempcell.type = 0;
	((openGL_window*)((glwin*)window)->win)->now_mode = 0;
	((bmp_window*)((glwin*)window)->win2)->isLoadBmp = false;
}

void clear_btn_cb(Fl_Widget *widget, void *window){
	printf("Mode : Points\n");
	((openGL_window*)window)->tempcell.type = 0;
	((openGL_window*)window)->now_mode = 0;
}

void blink_btn_cb(Fl_Widget *widget, void *window){
	printf("Mode : Points\n");
	((openGL_window*)window)->blink =!((openGL_window*)window)->blink;
}

void points_btn_cb(Fl_Widget *widget, void *window){
	printf("Mode : Points\n");
	((openGL_window*)window)->tempcell.type = 0;
	((openGL_window*)window)->now_mode = 1;
	((openGL_window*)window)->directAdd(1);
}

void lines_btn_cb(Fl_Widget *widget, void * window){
	printf("Mode : Lines\n");
	((openGL_window*)window)->tempcell.type = 0;
	((openGL_window*)window)->now_mode = 2;
	((openGL_window*)window)->directAdd(2);
}

void tri_btn_cb(Fl_Widget *widget, void *window){
	printf("Mode : Triangles\n");
	((openGL_window*)window)->tempcell.type = 0;
	((openGL_window*)window)->now_mode = 3;
	((openGL_window*)window)->directAdd(3);
}

void dis_points_btn_cb(Fl_Widget *widget, void *window){
	printf("Mode : Points\n");
	((openGL_window*)window)->tempcell.type = 0;
	((openGL_window*)window)->now_mode = 1;
}

void dis_lines_btn_cb(Fl_Widget *widget, void * window){
	printf("Mode : Lines\n");
	((openGL_window*)window)->tempcell.type = 0;
	((openGL_window*)window)->now_mode = 2;
}

void dis_tri_btn_cb(Fl_Widget *widget, void *window){
	printf("Mode : Triangles\n");
	((openGL_window*)window)->tempcell.type = 0;
	((openGL_window*)window)->now_mode = 3;
}

void bmp_btn_cb(Fl_Widget *widget, void *window){
	printf("Load BMP!\n");
	((bmp_window*)window)->loadbmp();
	// glutSwapBuffers();
}

void exit_btn_cb(Fl_Widget *widget, void *window){
	exit(0);
}

//  main function
int main(int argc, char **argv) {

	srand(time(NULL));

	Fl_Window window(640 * 2, 480);  // Create a FLTK window. Resolution 600*400. 	

	openGL_window gl_win(10, 10, 600 ,400);   // Add openGL window in position (10,10).Resolution 600*400
	bmp_window gl_win2(650, 10, 600 ,400);   // Add openGL window in position (10,10).Resolution 600*400
	// window.resizable(&gl_win);

	Fl_Widget *idle_btn;
	idle_btn = new Fl_Button(10,420,100,20,"Idle");
	struct glwins new_gl_wins;
	new_gl_wins.win = &gl_win;
	new_gl_wins.win2 = &gl_win2;
	idle_btn->callback(idle_btn_cb,&new_gl_wins);

	Fl_Widget *exit_btn;
	exit_btn = new Fl_Button(10,450,100,20,"Exit");
	exit_btn->callback(exit_btn_cb,&window);

	Fl_Widget *clear_btn;
	clear_btn = new Fl_Button(120,420,100,20,"Clear");
	clear_btn->callback(clear_btn_cb,&gl_win);

	Fl_Widget *blink_btn;
	blink_btn = new Fl_Button(120,450,100,20,"Blink");
	blink_btn->callback(blink_btn_cb,&gl_win);

	Fl_Widget *points_btn;
	points_btn = new Fl_Button(230,420,100,20,"genPoints");
	points_btn->callback(points_btn_cb,&gl_win);

	Fl_Widget *lines_btn;
	lines_btn = new Fl_Button(340,420,100,20,"genLines");
	lines_btn->callback(lines_btn_cb, &gl_win);

	Fl_Widget *tri_btn;
	tri_btn = new Fl_Button(450,420,100,20,"genTirangles");
	tri_btn->callback(tri_btn_cb,&gl_win);
	
	Fl_Widget *dis_points_btn;
	dis_points_btn = new Fl_Button(230,450,100,20,"Points");
	dis_points_btn->callback(dis_points_btn_cb,&gl_win);

	Fl_Widget *dis_lines_btn;
	dis_lines_btn = new Fl_Button(340,450,100,20,"Lines");
	dis_lines_btn->callback(dis_lines_btn_cb, &gl_win);

	Fl_Widget *dis_tri_btn;
	dis_tri_btn = new Fl_Button(450,450,100,20,"Tirangles");
	dis_tri_btn->callback(dis_tri_btn_cb,&gl_win);

	Fl_Widget *bmp_btn;
	bmp_btn = new Fl_Button(650,420,100,20,"Load BMP");
	bmp_btn->callback(bmp_btn_cb,&gl_win2);
	
	//openGL_window bmpWindow(650, 10, 600 ,400);   // Add openGL window in position (10,10).Resolution 600*400

	window.end();                  // End of FLTK windows setting. 
	window.show(argc,argv);        // Show the FLTK window

	gl_win.show();                 // Show the openGL window
  
	gl_win.redraw_overlay();       // redraw 
	return Fl::run();
}