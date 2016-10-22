#include "openGL_window.h"

void openGL_window :: draw()  // a.k.a. RenderFunc()
{
		// the valid() property may be used to avoid reinitializing your GL transformation for each redraw:
		if (!valid()) {
			valid(1);
			glLoadIdentity();
			glViewport(0,0,w(),h());
		}

		if(now_mode == 0) {
			drawcells.clear();
			tempcell.type = 0;
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBegin(GL_QUADS);
					glColor3f(1, 1, 1);glVertex2f(-1, -1);
					glColor3f(1, 1, 1);glVertex2f(-1,  1);
					glColor3f(1, 1, 1);glVertex2f( 1,  1);
					glColor3f(1, 1, 1);glVertex2f( 1, -1);
			glEnd();
		}
		else { // draw all cells
			for(unsigned int i = 0; i  < drawcells.size(); ++i){
				
				switch(drawcells[i].type){ 
					case 1: // points
						// printf("Draw points : %d,%d\n",drawcells[i].position[0].x,drawcells[i].position[0].y);
					    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						if(blink) 
							glColor3f((float)(rand()%255)/255,(float)(rand()%255)/255,(float)(rand()%255)/255);
						else
							gl_color(drawcells[i].rgb);
						glPointSize(5);
						glBegin( GL_POINTS );
							glVertex2f(
									(drawcells[i].position[0].x - width/2)/width *2, 
									(drawcells[i].position[0].y - height/2)/height * -2
							);
						glEnd();
						glPointSize(1);
						break;
					case 2 :  // lines
						// printf("Draw Lines : (%d,%d) - (%d,%d) \n",drawcells[i].position[0].x,drawcells[i].position[0].y,drawcells[i].position[1].x,drawcells[i].position[1].y);
					    // glClear(GL_COLOR_BUFFER_BIT);
						if(blink) 
							glColor3f((float)(rand()%255)/255,(float)(rand()%255)/255,(float)(rand()%255)/255);
						else
							gl_color(drawcells[i].rgb);

						glLineWidth(5);
						glBegin( GL_LINES);
							glVertex2f(
									(drawcells[i].position[0].x - width/2)/width *2, 
									(drawcells[i].position[0].y - height/2)/height * -2
								);
							glVertex2f(
									(drawcells[i].position[1].x - width/2)/width *2, 
									(drawcells[i].position[1].y - height/2)/height * -2
								);
						glEnd();
						glLineWidth(1);
						break;
					case 3 : // triangles
						// printf("Draw Triangles : (%f,%f) - (%f,%f)  - (%f,%f)\n",drawcells[i].position[0].x,drawcells[i].position[0].y,drawcells[i].position[1].x,drawcells[i].position[1].y,drawcells[i].position[2].x,drawcells[i].position[2].y);
					    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						if(blink) 
							glColor3f((float)(rand()%255)/255,(float)(rand()%255)/255,(float)(rand()%255)/255);
						else
							gl_color(drawcells[i].rgb);

						glBegin( GL_TRIANGLES );
								// Since glVertex2f(x,y) : x and y represents proportion, we need to calculate 
								// x = ( position_of_mouse_x - half_of_viewport_width ) / half_of_viewport_width
						        // so as y, but remember y-axis needs to be reversed.
								glVertex2f(
									(drawcells[i].position[0].x - width/2)/width *2, 
									(drawcells[i].position[0].y - height/2)/height * -2
								);
								glVertex2f(
									(drawcells[i].position[1].x - width/2)/width *2, 
									(drawcells[i].position[1].y - height/2)/height * -2
								);
								glVertex2f(
									(drawcells[i].position[2].x - width/2)/width *2, 
									(drawcells[i].position[2].y - height/2)/height * -2
								);
						glEnd();
					default : break;
				}
			}
		}
		glFlush();
		++frame;
}
void openGL_window :: draw_overlay()
{
		// Draw overlay function. 
		// the valid() property may be used to avoid reinitializing your GL transformation for each redraw:
		if ( !valid() ) {
			valid(1);
			glLoadIdentity();
			glViewport(0,0,w(),h());
		}
}
void openGL_window :: directAdd(int mode)
{
	struct cells tempcell;
	switch(mode){
		case 1:
			tempcell.rgb = fl_rgb_color(rand()%255,rand()%255,rand()%255);
			tempcell.type = tempcell.now = 1;
			tempcell.position[0].x = (float)(rand() % width);
			tempcell.position[0].y = (float)(rand() % height);
			drawcells.push_back(tempcell);
			break;
		case 2:			
			tempcell.rgb = fl_rgb_color(rand()%255,rand()%255,rand()%255);
			tempcell.type = tempcell.now = 2;
			tempcell.position[0].x = (float)(rand() % width);
			tempcell.position[0].y = (float)(rand() % height);
			tempcell.position[1].x = (float)(rand() % width);
			tempcell.position[1].y = (float)(rand() % height);
			drawcells.push_back(tempcell);
			break;
		case 3:
			tempcell.rgb = fl_rgb_color(rand()%255,rand()%255,rand()%255);
			tempcell.type = tempcell.now = 3;
			tempcell.position[0].x = (float)(rand() % width);
			tempcell.position[0].y = (float)(rand() % height);
			tempcell.position[1].x = (float)(rand() % width);
			tempcell.position[1].y = (float)(rand() % height);
			tempcell.position[2].x = (float)(rand() % width);
			tempcell.position[2].y = (float)(rand() % height);
			drawcells.push_back(tempcell);
			break;
	}
}
int openGL_window :: handle_mouse(int event, int button, int x, int y)
{
		printf("Mouse Clicked On ( %d , %d )\n",x,y);
		switch(now_mode){
			case 0 : // idle
				break;
			case 1 : // points
				struct cells tempcells; // since points push back immediately, we don't use openGL_window:tempcell
				struct vector2 temppos;
				tempcells.type = 1;
				tempcells.now = 1;
				temppos.x = x;
				temppos.y = y;
				tempcells.position[tempcells.now -1] = temppos;
				tempcells.rgb = fl_rgb_color(rand()%255,rand()%255,rand()%255);
				drawcells.push_back(tempcells);
				break;
			case 2 : // lines
				if(tempcell.type == 0){ // first data
					tempcell.type = 2;
					tempcell.now = 1;
					tempcell.position[0].x = x;
					tempcell.position[0].y = y;
				}
				else{
					tempcell.now = 2;
					tempcell.position[1].x = x;
					tempcell.position[1].y = y;
					tempcell.rgb = fl_rgb_color(rand()%255,rand()%255,rand()%255);
					drawcells.push_back(tempcell);
					tempcell.type = 0;
				}	
				break;
			case 3 : // triangles
				if(tempcell.type == 0){ // first data
					tempcell.type = 3;
					tempcell.now = 1;
					tempcell.position[0].x = x;
					tempcell.position[0].y = y;
				}
				else{
					if(tempcell.now == 1){
						tempcell.now = 2;
						tempcell.position[1].x = x;
						tempcell.position[1].y = y;
					}
					else{
						tempcell.now = 3;
						tempcell.position[2].x = x;
						tempcell.position[2].y = y;
						tempcell.rgb = fl_rgb_color(rand()%255,rand()%255,rand()%255);
						drawcells.push_back(tempcell);
						tempcell.type = 0;
					}					
				}	
				break;
			default : break;
		}
		return 0;
}

int  openGL_window :: handle(int event)
{
		switch (event) {
				case FL_PUSH:
						return handle_mouse(event,Fl::event_button(), Fl::event_x(),Fl::event_y());          
				default:
						return Fl_Window::handle(event);
		}
}