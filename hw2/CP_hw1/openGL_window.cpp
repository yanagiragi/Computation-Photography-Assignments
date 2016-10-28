#include "openGL_window.h"

void openGL_window :: InitNode () // Initialize Nodes we wish to draw
{	
	/* Assign Names */
	drawcell[0].name = "Torse";
	drawcell[1].name = "Neck";
	drawcell[2].name = "Head";
	drawcell[3].name = "leftArm";
	drawcell[4].name = "leftForeAem";
	drawcell[5].name = "rightArm";
	drawcell[6].name = "rightForeArm";
	drawcell[7].name = "Hips";
	drawcell[8].name = "leftLeg";
	drawcell[9].name = "rightLeg";

	/* Assign Dependicies */
	drawcell[0].parent = NULL;
	drawcell[1].parent = &drawcell[0];
	drawcell[2].parent = &drawcell[1];
	
	drawcell[3].parent = &drawcell[0];
	drawcell[4].parent = &drawcell[3];

	drawcell[5].parent = &drawcell[0];
	drawcell[6].parent = &drawcell[5];

	drawcell[7].parent = &drawcell[0];
	drawcell[8].parent = &drawcell[7];
	drawcell[9].parent = &drawcell[7];

	/* Assign data */
	drawcell[0].base.x = 0;
	drawcell[0].base.y = 0;
	drawcell[0].joint = drawcell[0].base;
	drawcell[0].height = 0.225;
	drawcell[0].width = 0.225;
	drawcell[0].nowAngle = 0;
	drawcell[0].dof = 0;
	
	drawcell[1].base.x = 0;
	drawcell[1].base.y = 0.25;
	drawcell[1].joint.x = 0;
	drawcell[1].joint.y = 0.08;
	drawcell[1].height = 0.2;
	drawcell[1].width = 0.05;
	drawcell[1].nowAngle = 0;
	drawcell[1].dof = 10;

	drawcell[2].base.x = 0;
	drawcell[2].base.y = 0.5;
	drawcell[2].joint.x = 0;
	drawcell[2].joint.y = 0.225;
	drawcell[2].height = 0.15;
	drawcell[2].width = 0.4;
	drawcell[2].nowAngle = 0;
	drawcell[2].dof = 10;

	drawcell[3].base.x = -0.275;
	drawcell[3].base.y = 0;
	drawcell[3].joint.x = -0.075;
	drawcell[3].joint.y = 0;
	drawcell[3].height = 0.05;
	drawcell[3].width = 0.3;
	drawcell[3].nowAngle = 0;
	drawcell[3].dof = 30;

	drawcell[4].base.x = -0.45;
	drawcell[4].base.y = 0;
	drawcell[4].joint.x = -0.325;
	drawcell[4].joint.y = 0;	
	drawcell[4].height = 0.05;
	drawcell[4].width = 0.3;
	drawcell[4].nowAngle = 0;
	drawcell[4].dof = 30;

	drawcell[5].base.x = 0.275;
	drawcell[5].base.y = 0;
	drawcell[5].joint.x = 0.075;
	drawcell[5].joint.y = 0;
	drawcell[5].height = 0.05;
	drawcell[5].width = 0.3;
	drawcell[5].nowAngle = 0;
	drawcell[5].dof = 30;

	drawcell[6].base.x = 0.45;
	drawcell[6].base.y = 0;
	drawcell[6].joint.x = 0.325;
	drawcell[6].joint.y = 0;
	drawcell[6].height = 0.05;
	drawcell[6].width = 0.3;
	drawcell[6].nowAngle = 0;
	drawcell[6].dof = 30;

	drawcell[7].base.x = 0;
	drawcell[7].base.y = -0.225;
	drawcell[7].joint.x = 0;
	drawcell[7].joint.y = -0.1;
	drawcell[7].height = 0.1;
	drawcell[7].width = 0.3;
	drawcell[7].nowAngle = 0;
	drawcell[7].dof =10; 

	drawcell[8].base.x = -0.125;
	drawcell[8].base.y = -0.225;
	drawcell[8].joint.x = -0.13;
	drawcell[8].joint.y = -0.12;
	drawcell[8].height = 0.3;
	drawcell[8].width = 0.05;
	drawcell[8].nowAngle = 0;
	drawcell[8].dof = 10;

	drawcell[9].base.x = 0.125;
	drawcell[9].base.y = -0.225;
	drawcell[9].joint.x = 0.13;
	drawcell[9].joint.y = -0.12;
	drawcell[9].height = 0.3;
	drawcell[9].width = 0.05;
	drawcell[9].nowAngle = 0;
	drawcell[9].dof = 10;

	/* Calculate Offset (Vector2) */
	for(int i = 1; i < 10; ++i){		
		drawcell[i].angleOffset = drawcell[i].nowAngle - drawcell[i].parent->nowAngle;		
	}

	return;
}
void openGL_window :: scaleUpVertex (int choose, float increment){
		switch(choose){
		case 4 : 
		case 6 : 
			if(drawcell[choose].width >= 0.4);
			else scaleVertex(choose,increment);
			break;
		case 8 : 	
		case 9 : 	
			if(drawcell[choose].height >= 0.34);
			else scaleVertex(choose,increment);
			break;
		default : break;	
	}
}
void openGL_window :: scaleDownVertex (int choose, float increment){
	switch(choose){
		case 4 : 
		case 6 : 
			if(drawcell[choose].width <= 0.3);
			else scaleVertex(choose,increment);
			break;
		case 8 : 	
		case 9 : 	
			if (drawcell[choose].height <= 0.3);
			else scaleVertex(choose,increment);
			break;
		default : break;	
	}
}
void openGL_window :: scaleVertex (int index, float increment){
	if(index == 4 || index == 6){
		drawcell[index].width += increment;
	}
	else if(index == 8 || index == 9){
		drawcell[index].height += increment;
		drawcell[index].base.y -= increment;
	}
}

void openGL_window :: rotateVertex (int index, float increment){
	choose = index;
	float value = clamp(drawcell[index].nowAngle + increment, -1 * drawcell[index].dof, drawcell[index].dof);
	drawcell[index].nowAngle = value;
	switch(index){
		case 1 : 
			drawcell[2].nowAngle = value + drawcell[2].angleOffset;
			break;
		case 3 : 
			drawcell[4].nowAngle = value + drawcell[4].angleOffset;
			break;
		case 5: 
			drawcell[6].nowAngle = value + drawcell[6].angleOffset;
			break;
		case 7: 
			drawcell[8].nowAngle = value + drawcell[8].angleOffset;
			drawcell[9].nowAngle = value + drawcell[9].angleOffset;
			break;
	}
}

float openGL_window :: clamp(float v, float min, float max){
	return (v > max) ? max : ((v < min) ? min : v);
}

float openGL_window :: magnitude(Vector2 v){
	return sqrt(v.x * v.x + v.y * v.y);
}

void openGL_window :: calculateVertex(){
	int angle = 1;
	if(isAuto){
			
			if(drawcell[4].nowAngle >= 29.0)
				isReverse = !isReverse;
			else if(drawcell[4].nowAngle <= -29.0){
				isReverse = !isReverse;	
			}
		
		for(int i = 0; i < 10; ++i){
			if(i == 2) continue; // skip head, or you may set HEAD DOF = 0			
			rotateVertex(i,angle * (isReverse) ? 1 : (-1) );					
		}
	}
	for(int i = 1; i < 10; ++i){
		// recalculate angle difference for next frame to use 
		drawcell[i].angleOffset = drawcell[i].nowAngle - drawcell[i].parent->nowAngle;
	}
}

void openGL_window :: draw()  // a.k.a. RenderFunc()
{
		// the valid() property may be used to avoid reinitializing your GL transformation for each redraw:
		if (!valid()) {
			valid(1);
			glLoadIdentity();
			glViewport(0,0,w(),h());
		}
		
		// Init Canvas
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBegin(GL_QUADS);
			glColor3f(1, 1, 1);
			glVertex2f(-1, -1);
			glVertex2f(-1,  1);
			glVertex2f( 1,  1);
			glVertex2f( 1, -1);
		glEnd();

		calculateVertex();
		
		glColor3f(0,0,0);
		glPointSize(5.0);

		for(int i = 0; i < 10; ++i){
			glPushMatrix();		
			glRotatef(drawcell[i].nowAngle,0,0,1);
			
			#pragma region
			if( i == 4 || i == 6 || i == 8 || i == 9){
					glPushMatrix();
						glLoadIdentity();
						glRotatef(drawcell[i].parent->nowAngle,0,0,1);
					
						glColor3f(0,1,0);				
						glBegin(GL_POINTS);
							glVertex2f(drawcell[i].joint.x, drawcell[i].joint.y);
							glVertex2f(0,0);
						glEnd();
					
						glTranslatef(drawcell[i].joint.x, drawcell[i].joint.y,1);
						glRotatef(drawcell[i].nowAngle - drawcell[i].parent->nowAngle ,0,0,1);
						
						// draw Rect Center Point
						/*glColor3f(1,0,0);
						glBegin(GL_POINTS);
							glVertex2f(drawcell[i].base.x - drawcell[i].joint.x, drawcell[i].base.y - drawcell[i].joint.y);
						glEnd();*/
					
						glColor3f(0,0,0);
						glBegin(GL_LINE_LOOP);
							glVertex2f(
								drawcell[i].base.x - drawcell[i].width/2 - drawcell[i].joint.x,
								drawcell[i].base.y + drawcell[i].height/2 - drawcell[i].joint.y
							);
							glVertex2f(
								drawcell[i].base.x + drawcell[i].width/2 - drawcell[i].joint.x,
								drawcell[i].base.y + drawcell[i].height/2 - drawcell[i].joint.y
							);
							glVertex2f(
								drawcell[i].base.x + drawcell[i].width/2 - drawcell[i].joint.x,
								drawcell[i].base.y - drawcell[i].height/2 - drawcell[i].joint.y
							);
							glVertex2f(
								drawcell[i].base.x - drawcell[i].width/2 - drawcell[i].joint.x,
								drawcell[i].base.y - drawcell[i].height/2 - drawcell[i].joint.y
							);
						glEnd();

					glPopMatrix();
			
				}
			#pragma endregion case4&case6&case8&case9
			else{
				
				if(i == 2){ // draw circle

					#pragma region
					glPushMatrix();
					glPointSize(1.0);
					glLoadIdentity();
					glRotatef(drawcell[i].nowAngle,0,0,1);
					glTranslatef(drawcell[i].base.x-0.1,drawcell[i].base.y-0.22,1);
					for(int j = 0; j < 360; ++j){
						glRotatef(1,0,0,1);
						glBegin(GL_POINTS);
							glVertex2f(0.01,0.01);
						glEnd();
					}
					for(int j = 0; j < 360; ++j){
						glRotatef(1,0,0,1);
						glBegin(GL_POINTS);
							glVertex2f(0.02,0.02);
						glEnd();
					}
					glPopMatrix();
					#pragma endregion  Left Circle					
					
					#pragma region 
					glPushMatrix();
					glPointSize(1.0);
					glLoadIdentity();
					glRotatef(drawcell[i].nowAngle,0,0,1);
					glTranslatef(drawcell[i].base.x+0.1,drawcell[i].base.y-0.22,1);
					for(int j = 0; j < 360; ++j){
						glRotatef(1,0,0,1);
						glBegin(GL_POINTS);
							glVertex2f(0.01,0.01);
						glEnd();
					}
					for(int j = 0; j < 360; ++j){
						glRotatef(1,0,0,1);
						glBegin(GL_POINTS);
							glVertex2f(0.02,0.02);
						glEnd();
					}
					glPopMatrix();
					#pragma endregion Right Circle

				}
				
				glColor3f(0,1,0);
				glPointSize(5.0);
				glBegin(GL_POINTS);
					glVertex2f(drawcell[i].joint.x, drawcell[i].joint.y);
				glEnd();

				glColor3f(0,0,0);										
				glBegin(GL_LINE_LOOP);
					glVertex2f(
						drawcell[i].base.x - drawcell[i].width/2 - drawcell[i].joint.x,
						drawcell[i].base.y + drawcell[i].height/2 - drawcell[i].joint.y
					);
					glVertex2f(
						drawcell[i].base.x + drawcell[i].width/2 - drawcell[i].joint.x,
						drawcell[i].base.y + drawcell[i].height/2 - drawcell[i].joint.y
					);
					glVertex2f(
						drawcell[i].base.x + drawcell[i].width/2 - drawcell[i].joint.x,
						drawcell[i].base.y - drawcell[i].height/2 - drawcell[i].joint.y
					);
					glVertex2f(
						drawcell[i].base.x - drawcell[i].width/2 - drawcell[i].joint.x,
						drawcell[i].base.y - drawcell[i].height/2 - drawcell[i].joint.y
					);
				glEnd();				
				
			}
			glPopMatrix();
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

int openGL_window :: handle_mouse(int event, int button, int x, int y)
{
		// Not Used
		// printf("Mouse Clicked On ( %d , %d )\n",x,y);
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