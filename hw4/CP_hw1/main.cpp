#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>

#include "glut.h"
#include "previewBMP.h"
#include "ImageQuilt.h"
#include "transferWindow.h"
#include "bmpLoader.h"

#include <stdio.h>
#include <time.h>

int choose = 0, tileAmm = 2, target = 0, ittr = 1;

Fl_Window *nowopen;
ImageQuilt *nowquilt;

int transWindow(void* bmpWindow)
{
	int w = (target == 0 ? 120 : 110);
	int h = (target == 0 ? 146 : 167);
	Fl_Window windows(w, h);
	
	bmpLoader bmp;	
	bmp.pixels = NULL;
	
	bool res = bmp.BMP_Read(target == 0 ? "texture/bill.bmp" : "texture/girl.bmp", &(bmp.pixels), w, h);
	if(res)
		printf("Change to %s\n", target == 0 ? "bill.bmp" : "girl.bmp");
	else
		printf("Unable to Change to %s\n", target == 0 ? "bill.bmp" : "girl.bmp");
	
	transferWindow imageWindow(((previewBMP*)bmpWindow)->bmp.pixels,((previewBMP*)bmpWindow)->bmp_w, ((previewBMP*)bmpWindow)->bmp_h, bmp.pixels, w, h ,0, 0, w, h);
	
	windows.end();
	windows.position(
		((previewBMP*) bmpWindow)->x_root() + 320 ,
		((previewBMP*) bmpWindow)->y_root() + 0 
	);
	windows.show();
		
	//imageWindow.show();
	//imageWindow.redraw_overlay();

	if(nowopen){
		nowopen->hide();
		//delete nowopen;
	}

	nowopen = &windows;
	//nowquilt = &imageWindow;

	return Fl::run();
}


int resWindow(void* bmpWindow)
{
	int w = ((previewBMP*) bmpWindow)->bmp_w;
	int h = ((previewBMP*) bmpWindow)->bmp_h;
	Fl_Window windows(w * tileAmm, h * tileAmm);
	ImageQuilt imageWindow(((previewBMP*) bmpWindow)->bmp.pixels, tileAmm, 0/*nowmode*/, 0, 0, 1, 0, 0, w* tileAmm, h * tileAmm);
	
	windows.end();
	windows.position(
		((previewBMP*) bmpWindow)->x_root() + 320 ,
		((previewBMP*) bmpWindow)->y_root() + 0 
	);
	windows.show();
		
	imageWindow.show();
	imageWindow.redraw_overlay();
	
	for(int i = 0;i < ittr; ++i){
		printf("Synthesis %d\n", i);
		imageWindow.Synthesis();
	}

	if(nowopen){
		nowopen->hide();
		//delete nowopen;
	}

	nowopen = &windows;
	nowquilt = &imageWindow;

	return Fl::run();
}

int resWindowDebug(void* bmpWindow)
{
	int w = ((previewBMP*) bmpWindow)->bmp_w;
	int h = ((previewBMP*) bmpWindow)->bmp_h;
	Fl_Window windows(w * tileAmm, h * tileAmm);
	ImageQuilt imageWindow(((previewBMP*) bmpWindow)->bmp.pixels, tileAmm, 0/*nowmode*/, 0, 0, 1, 0, 0, w* tileAmm, h * tileAmm);
	
	windows.end();
	windows.position(
		((previewBMP*) bmpWindow)->x_root() + 320 ,
		((previewBMP*) bmpWindow)->y_root() + 0 
	);
	windows.show();
		
	imageWindow.show();
	imageWindow.redraw_overlay();
	imageWindow.debug = true;
	imageWindow.Synthesis();

	if(nowopen){
		nowopen->hide();
		//delete nowopen;
	}

	nowopen = &windows;
	
	return Fl::run();
}

int resWindowSimple(void* bmpWindow)
{
	int w = ((previewBMP*) bmpWindow)->bmp_w;
	int h = ((previewBMP*) bmpWindow)->bmp_h;
	Fl_Window windows(w * tileAmm, h * tileAmm);
	ImageQuilt imageWindow(((previewBMP*) bmpWindow)->bmp.pixels, tileAmm, -1/*nowmode*/, 0, 0, 1, 0, 0, w* tileAmm, h * tileAmm);
	
	windows.end();
	windows.position(
		((previewBMP*) bmpWindow)->x_root() + 320 ,
		((previewBMP*) bmpWindow)->y_root() + 0 
	);
	windows.show();
		
	imageWindow.show();
	imageWindow.redraw_overlay();
	imageWindow.debug = true;
	imageWindow.Synthesis();

	if(nowopen){
		nowopen->hide();
		//delete nowopen;
	}

	nowopen = &windows;
	
	return Fl::run();
}

int newWindow(void* bmpWindow)
{		
	// preview cul
	int w = ((previewBMP*) bmpWindow)->bmp_w;
	int h = ((previewBMP*) bmpWindow)->bmp_h;
	Fl_Window *windows = new Fl_Window(w,h);		
	// data, tile, nowmode, g_x, g_y, cul_point, x, y, w, h
	ImageQuilt imageWindow(((previewBMP*) bmpWindow)->bmp.pixels, 1, 1/*nowmode*/, 0, 0, 2, 0, 0, w, h);

	windows->end();
	windows->position(
		((previewBMP*) bmpWindow)->x_root() + 320 ,
		((previewBMP*) bmpWindow)->y_root() + 0 
	);
	windows->show();
		
	imageWindow.show();
	imageWindow.redraw_overlay();

	if(nowopen)
		nowopen->hide();

	nowopen = windows;
		
	return Fl::run();
}

void syn_btn_cb(Fl_Widget *widget, void *window){
	printf("Synthesis With TileAmm = %d\n", tileAmm);
	int res = resWindow(window);
	if(!res)
		printf("Unable to Open new Window!\n");
}

void syndebug_btn_cb(Fl_Widget *widget, void *window){
	printf("Synthesis With Debug mode With TileAmm = %d\n", tileAmm);
	int res = resWindowDebug(window);
	if(!res)
		printf("Unable to Open new Window!\n");
}

void synsimple_btn_cb(Fl_Widget *widget, void *window){
	printf("Synthesis With Debug mode With TileAmm = %d\n", tileAmm);
	int res = resWindowSimple(window);
	if(!res)
		printf("Unable to Open new Window!\n");
}

void trans_btn_cb(Fl_Widget *widget, void *window){
	/*printf("Synthesis With TileAmm = %d\n", tileAmm);
	*/
	int res = transWindow(window);
	//if(!res)
	//	printf("Unable to Open new Window!\n");*/
	//int res2 = newWindow(window);
	
}

void choice1_cb(Fl_Widget *o, void *window)   // choice callback function
{
	int temp=int(((Fl_Choice*)o)->value());
	if(choose == temp) return;

	choose = temp;
	printf("Choice = %d\n", temp);
	((previewBMP*)window)->change(choose);
	((previewBMP*)window)->resize(
		(320 - ((previewBMP*)window)->bmp_w) / (choose == 5 ? 1: 2),
		(320 - ((previewBMP*)window)->bmp_h) / (choose == 5 ? 2: 4),
		((previewBMP*)window)->bmp_w * (choose == 5 ? 0.7 : 1),
		((previewBMP*)window)->bmp_h * (choose == 5 ? 0.7 : 1)
	);
}

void choice2_cb(Fl_Widget *o, void *p)   // choice callback function
{
	int temp=int(((Fl_Choice*)o)->value());
	tileAmm = temp + 2;
	printf("tileAmm = %d\n", tileAmm);
}

void choice3_cb(Fl_Widget *o, void *p)   // choice callback function
{
	int temp=int(((Fl_Choice*)o)->value());
	target = temp;
	printf("Target = %d\n", target);
}

void choice4_cb(Fl_Widget *o, void *p)   // choice callback function
{
	int temp=int(((Fl_Choice*)o)->value());
	ittr = temp == 0 ? 1 : 20;
	printf("iterration = %d\n", ittr);
}
//  main function
int main(int argc, char **argv) {

	srand(time(NULL));
	nowquilt = NULL;
	nowopen = NULL;

	Fl_Window window(320, 450);
	previewBMP bmpWindow(96, 48, 128 ,128);   

	/* List and Buttons */
	Fl_Choice *choice1=(Fl_Choice *)0;  
	Fl_Menu_Item menu_choice1[] = { 
	 {"berry", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"btile", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"cans_sc", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"fabric", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"greek", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"lobelia", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"mesh", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"rice", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"wool", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"pdir2", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"rock", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {0,0,0,0,0,0,0,0,0}
	};

	choice1 = new Fl_Choice(20, 244 , 100 , 20);
	choice1->down_box(FL_BORDER_BOX);
	choice1->menu(menu_choice1);
	choice1->callback(choice1_cb,&bmpWindow);

	/*Fl_Widget *bmp_btn;
	bmp_btn = new Fl_Button(180,234,100,20,"Change & Preview");
	bmp_btn->callback(bmp_btn_cb,&bmpWindow);	*/

	Fl_Choice *choice2=(Fl_Choice *)0;  
	Fl_Menu_Item menu_choice2[] = {  
	 {"2", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"3", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"4", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"5", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {0,0,0,0,0,0,0,0,0}
	};

	choice2 = new Fl_Choice(20, 284 , 100 , 20); 
	choice2->down_box(FL_BORDER_BOX);
	choice2->menu(menu_choice2);
	choice2->callback(choice2_cb,NULL);

	/*Fl_Choice *choice4=(Fl_Choice *)0;  
	Fl_Menu_Item menu_choice4[] = { 
	 {"itterate 1", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"itterate 20", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {0,0,0,0,0,0,0,0,0}
	};

	choice4 = new Fl_Choice(20, 324 , 100 , 20);
	choice4->down_box(FL_BORDER_BOX);
	choice4->menu(menu_choice4);
	choice4->callback(choice4_cb,&bmpWindow);*/

	Fl_Widget *bmp_btn2;
	bmp_btn2 = new Fl_Button(180,244,100,20,"Synthesis");
	bmp_btn2->callback(syn_btn_cb,&bmpWindow);	

	Fl_Widget *bmp_btnd;
	bmp_btnd = new Fl_Button(180,284,100,20,"Debug");
	bmp_btnd->callback(syndebug_btn_cb,&bmpWindow);	
	
	Fl_Widget *bmp_btns;
	bmp_btnd = new Fl_Button(180,324,100,20,"Simple");
	bmp_btnd->callback(synsimple_btn_cb,&bmpWindow);	
	

	Fl_Choice *choice3=(Fl_Choice *)0;  
	Fl_Menu_Item menu_choice3[] = {  
	 {"bill", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {"girl", 0,  0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0},
	 {0,0,0,0,0,0,0,0,0}
	};
	
	choice3 = new Fl_Choice(20, 384 , 100 , 20); 
	choice3->down_box(FL_BORDER_BOX);
	choice3->menu(menu_choice3);
	choice3->callback(choice3_cb,NULL);

	Fl_Widget *bmp_btn3;
	bmp_btn3 = new Fl_Button(180,384,100,20,"Transfer");
	bmp_btn3->callback(trans_btn_cb,&bmpWindow);	

	/*Fl_Widget *bmp_btn4;
	bmp_btn4 = new Fl_Button(180,344,100,20,"1/2");
	bmp_btn4->callback(trans2_btn_cb,&bmpWindow);	*/

	window.end();                 
	window.show(argc,argv);

	bmpWindow.show();       
	bmpWindow.redraw_overlay();

	return Fl::run();
}