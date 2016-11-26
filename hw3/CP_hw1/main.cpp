#include <GL/glew.h>
#include <GL/GL.h>
#include <stdio.h>
#include <windows.h>  // for MS Windows
#include <math.h>
#include <glm/glm.hpp>
#include <GL/freeglut.h>
#include "SMFLoader.h"

using namespace glm;

typedef struct cell{
	char* name; // for debug
	float height, width;
	vec3 base;
	struct cell* parent;
	float dof; // value storing degree of freedom
	float nowAngle; // nowAngle = mid(nowAngle,parent->base.angle)
}node;

/* Global Variables */
SMFLoader smf;
node drawcell[10];
bool keyboardState[256],isReverse = false; 
char title[] = "CP_HW3";
float torseRotationAngle = 0.0;
vec3 cameraOffset, preFirstCameraOffset, preThirdCameraOffset; // camera position offset 
const float radian = 0.0174532925;
GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};


/* Function prototypes*/
// Mathmatics
float clamp(float v, float min, float max);
vec3 getNormal(GLfloat* vertex);
// GLs
void initGL();
void display();
void reshape(GLsizei width, GLsizei height);
void drawSolidCube();
// Robot and smf
void InitNode ();
void robotMovement(float x, float y, float z);
void rotateVertex (int index, float increment);
void drawsmf();
void drawRobot();
void drawMirror();
void drawMirrorRobotAndSmf();
// Keyboard events
void postDealKey();
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);

float clamp(float v, float min, float max)
{
	return (v > max) ? max : ((v < min) ? min : v);
}

void robotMovement(float x, float y, float z)
{	
	float incre_x = x * cosf(torseRotationAngle * radian) + z * sinf(torseRotationAngle * radian);
	float incre_y = y;
	float incre_z = -1.0 * x * sinf(torseRotationAngle * radian) + z * cosf(torseRotationAngle * radian);
	
	for(int i = 0; i < 7; ++i){		
		if((drawcell[0].base.x - incre_x) < 1.8 && (drawcell[0].base.x - incre_x) > -1.8) 
			drawcell[i].base.x -= incre_x;
		if((drawcell[0].base.z - incre_z) < 0.91 && (drawcell[0].base.z - incre_z) > -2) 		
			drawcell[i].base.z -= incre_z;
	}	
}

void rotateVertex (int index, float increment)
{	
	float value = clamp(drawcell[index].nowAngle + increment, -1 * drawcell[index].dof, drawcell[index].dof);
	drawcell[index].nowAngle = value;
}

void InitNode () // Initialize Nodes we wish to draw
{	
	/* Assign Names For Debug */
	drawcell[0].name = "Torse";
	drawcell[2].name = "Neck";
	drawcell[1].name = "Head";
	drawcell[3].name = "leftArm";
	drawcell[4].name = "rightArm";
	drawcell[5].name = "leftLeg";
	drawcell[6].name = "rightLeg";

	/* Assign Dependicies */
	drawcell[0].parent = NULL;
	drawcell[2].parent = &drawcell[0];
	drawcell[1].parent = &drawcell[2];
	
	drawcell[3].parent = &drawcell[0];	
	drawcell[4].parent = &drawcell[0];
	
	drawcell[5].parent = &drawcell[0];
	drawcell[6].parent = &drawcell[0];
	
	/* Assign data */
	drawcell[0].base.x = 0;
	drawcell[0].base.y = 0;
	drawcell[0].height = 0.1;
	drawcell[0].width = 0.1;
	drawcell[0].nowAngle = 0;
	drawcell[0].dof = 0;
	
	// Head
	drawcell[1].base.x = 0;
	drawcell[1].base.y = 0.2;
	drawcell[1].height = 0.05;
	drawcell[1].width = 0.15;
	drawcell[1].nowAngle = 0;
	drawcell[1].dof = 0;

	drawcell[2].base.x = 0;
	drawcell[2].base.y = 0.1;
	drawcell[2].height = 0.1;
	drawcell[2].width = 0.05;
	drawcell[2].nowAngle = 0;
	drawcell[2].dof = 10;

	// Left hand
	drawcell[3].base.x = -0.15;
	drawcell[3].base.y = 0.05;
	drawcell[3].height = 0.025;
	drawcell[3].width = 0.1;
	drawcell[3].nowAngle = 0;
	drawcell[3].dof = 10;

	// Right hand
	drawcell[4].base.x = 0.15;
	drawcell[4].base.y = 0.05;
	drawcell[4].height = 0.025;
	drawcell[4].width = 0.1;
	drawcell[4].nowAngle = 0;
	drawcell[4].dof = 10;

	// left leg
	drawcell[5].base.x = -0.08;
	drawcell[5].base.y = -0.15;
	drawcell[5].height = 0.1;
	drawcell[5].width = 0.03;
	drawcell[5].nowAngle = 0;
	drawcell[5].dof = 10; 

	// right leg
	drawcell[6].base.x = 0.08;
	drawcell[6].base.y = -0.15;
	drawcell[6].height = 0.1;
	drawcell[6].width = 0.03;
	drawcell[6].nowAngle = 0;
	drawcell[6].dof = 10;

	// lazy to change all base.y
	for(int i = 0; i < 7; ++i){		
		drawcell[i].base.y += 0.1;
	}

	for(int i = 0; i < 256; ++i)
		keyboardState[i] = false; // reset keyboard state

	cameraOffset.x = preThirdCameraOffset.x = 0;
	cameraOffset.y = preThirdCameraOffset.y = 1;
	cameraOffset.z = preThirdCameraOffset.z = 3;

	preFirstCameraOffset.x = 0;
	preFirstCameraOffset.y = 0.3;
	preFirstCameraOffset.z = 0;

	smf.load("rubber_duck.smf");
	
	return;
}

vec3 getNormal(GLfloat* vertex)
{
	vec3 a = vec3(*(vertex + 0), *(vertex + 1), *(vertex +2));
	vec3 b = vec3(*(vertex + 3), *(vertex + 4), *(vertex +5));
	vec3 c = vec3(*(vertex + 6), *(vertex + 7), *(vertex +8));
	vec3 u = b - a;
	vec3 v = c - a;
	vec3 n = vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.z);
	return normalize(n);
}

void drawsmf()
{	
	float* vertex = NULL;
	glPushMatrix();	
		glTranslatef(
			0.01,
			0.32,
			0
		); // We already rotated TorseAngle
		glRotatef(90,0,1,0);

		glColor3f(1,1,94/255.0); // rice yellow 
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
		glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
		glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
		
		for(int i=0; i < smf.getFaceSize(); ++i){
			vertex = smf.getFace(i);			
			glBegin(GL_TRIANGLES);
				vec3 normal = getNormal(vertex);
				glNormal3f(normal.x, normal.y, normal.z);
				glVertex3f(vertex[0],vertex[1],vertex[2]);
				glVertex3f(vertex[3],vertex[4],vertex[5]);
				glVertex3f(vertex[6],vertex[7],vertex[8]);
			glEnd();
		}

	glPopMatrix();
}

/* Initialize OpenGL Graphics */
void initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);// Set background color to black and opaque
	glClearDepth(1.0f);                  // Set background depth to farthest
	glEnable(GL_DEPTH_TEST);				// Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL);				// Set the type of depth-test
	glShadeModel(GL_SMOOTH);				// Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
	
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	//GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat diffuseLight[] = { 0.0f, 0.0f, 0.0, 0.0f }; // diffuse light cause strange effects?
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 0.0, 0.0, 400.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
					
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); 
	gluPerspective( 60, 0.4, 1.0, 500.0);   //³zµø§ë¼v 
	glMatrixMode(GL_MODELVIEW);				// To operate on model-view matrix
	glLoadIdentity();

	InitNode();
}

void drawRobot()
{		
	glTranslatef(drawcell[0].base.x , drawcell[0].base.y, drawcell[0].base.z);
	glRotatef(torseRotationAngle,0,1,0);
	
	for(int i = 7; i >= 0; --i){
		glPushMatrix();
			glTranslatef(
				drawcell[i].base.x - drawcell[0].base.x,
				drawcell[i].base.y - drawcell[0].base.y,
				drawcell[i].base.z - drawcell[0].base.z
			);
			
			if(i == 5 || i == 6){
				glRotatef(drawcell[i].nowAngle,1,0,0);
			} 
			else {
				glRotatef(drawcell[i].nowAngle,0,0,1);
			}
			
			glScalef(
				drawcell[i].width,
				drawcell[i].height,
				0.025 * ( (i <= 1 ) ? 2 : 1) // thicker head & torse
			);
			
			switch(i){
				case 0 : glColor3f(.5,.5,.5); break;
				case 1 : glColor3f(.2,.5,.5); break;
				case 2 : glColor3f(.8,.5,.5); break;
				default: // 3, 4, ,5 ,6
					glColor3f(1,1,94/255.0); break;
			}
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
			glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
			glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
			drawSolidCube();
			
		glPopMatrix();		
	}
}

void drawSolidCube()
{	
	for(int i = 0; i < 12; ++i){
		vec3 nor = getNormal(&(g_vertex_buffer_data[i+9]));
		glBegin(GL_TRIANGLES);		
			glNormal3f(nor.x,nor.y,nor.z);
			glVertex3f(g_vertex_buffer_data[i*9+0],g_vertex_buffer_data[i*9+1],g_vertex_buffer_data[i*9+2]);
			glVertex3f(g_vertex_buffer_data[i*9+3],g_vertex_buffer_data[i*9+4],g_vertex_buffer_data[i*9+5]);
			glVertex3f(g_vertex_buffer_data[i*9+6],g_vertex_buffer_data[i*9+7],g_vertex_buffer_data[i*9+8]);
		glEnd();
	}
	return ;
}

void drawMirrorRobotAndSmf()
{
	glPushMatrix();		
		float incre = drawcell[0].base.z + 2.3;
		for(int i = 0; i < 7; ++i)
			drawcell[i].base.z -= 2 * incre;
		glTranslatef(drawcell[0].base.x , drawcell[0].base.y, drawcell[0].base.z);
		glRotatef(-1 * torseRotationAngle,0,1,0);
	
		for(int i = 7; i >= 0; --i){
			glPushMatrix();
				glTranslatef(
					drawcell[i].base.x - drawcell[0].base.x,
					drawcell[i].base.y - drawcell[0].base.y,
					drawcell[i].base.z - drawcell[0].base.z
				);
			
				if(i == 5 || i == 6){
					glRotatef(drawcell[i].nowAngle,1,0,0);
				} 
				else {
					glRotatef(drawcell[i].nowAngle,0,0,1);
				}
			
				glRotatef(180.0,0,1,0); // rotate 180 deg to y-axis

				glScalef(
					drawcell[i].width,
					drawcell[i].height,
					0.025 * ( (i <= 1 ) ? 2 : 1) // thicker head & torse
				);
			
				switch(i){
					case 0 : glColor3f(.5,.5,.5); break;
					case 1 : glColor3f(.2,.5,.5); break;
					case 2 : glColor3f(.8,.5,.5); break;
					default: // 3, 4, ,5 ,6
						glColor3f(1,1,94/255.0); break;
				}
				glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
				glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
				glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
				drawSolidCube();
			
			glPopMatrix();		
		}
		
		float* vertex = NULL;
		glPushMatrix();	
			glTranslatef(
				0.01,
				0.32,
				0
			); // We already rotated TorseAngle
			glRotatef(90,0,1,0);

			glColor3f(1,1,94/255.0); // rice yellow 
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
			glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
			glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
		
			for(int i=0; i < smf.getFaceSize(); ++i){
				vertex = smf.getFace(i);			
				glBegin(GL_TRIANGLES);
					vec3 normal = getNormal(vertex);
					glNormal3f(normal.x, normal.y, normal.z);
					glVertex3f(vertex[0],vertex[1],vertex[2]);
					glVertex3f(vertex[3],vertex[4],vertex[5]);
					glVertex3f(vertex[6],vertex[7],vertex[8]);
				glEnd();
			}
		glPopMatrix();


		for(int i = 0; i < 7; ++i)
			drawcell[i].base.z += 2 * incre;

	glPopMatrix();

}
void drawMirror(){
	
	glPushMatrix(); // draw Mirror floor
		glColor3f(0,1,0);
		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glColorMaterial(GL_FRONT, GL_SPECULAR);		
		glTranslatef(0,-0.2,-4);
		glRotatef(90,1,0,0);
		glScaled(2,2,0.01);
		drawSolidCube();
		glColor3f(1,1,1);
	glPopMatrix();

	glPushMatrix(); // draw Mirror
		glTranslatef(0,1,-2);
		glScaled(3.8,2,0.01);
		glutWireCube(1);
	glPopMatrix();

	drawMirrorRobotAndSmf();	


}

/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	
	glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
	glLoadIdentity();

	if(cameraOffset.y == preThirdCameraOffset.y){
		gluLookAt(
			drawcell[0].base.x + cameraOffset.x,
			drawcell[0].base.y + cameraOffset.y,
			drawcell[0].base.z + cameraOffset.z,
			drawcell[0].base.x,
			drawcell[0].base.y,
			drawcell[0].base.z,
			0,1,0
		); // setup Third Person Camera
	}
	else{
		// Since we caluate robot.torse.base(position) in absolute way, we can't rotate coordinate here just to rotate camera
		// Or else lost precision ( Mat_rotate * Mat_perspective * Mat_rotateBack != Mat_perspective )
		vec3 org = vec3(
			(drawcell[0].base.x + cameraOffset.x) * cosf(torseRotationAngle * radian) + (drawcell[0].base.z + cameraOffset.z) * sinf(torseRotationAngle * radian),
			drawcell[0].base.y + cameraOffset.y,
			-1.0 * (drawcell[0].base.x + cameraOffset.x) * sinf(torseRotationAngle * radian) + (drawcell[0].base.z + cameraOffset.z) * cosf(torseRotationAngle * radian)
		);
		vec3 lookAt = vec3(
			(drawcell[0].base.x) * cosf(torseRotationAngle * radian) + (drawcell[0].base.z - 10) * sinf(torseRotationAngle * radian),
			drawcell[0].base.y + cameraOffset.y,
			-1.0 * drawcell[0].base.x * sinf(torseRotationAngle * radian) + (drawcell[0].base.z - 10) * cosf(torseRotationAngle * radian)
		);
		gluLookAt(
			org.x, org.y, org.z,
			lookAt.x, lookAt.y, lookAt.z,
			0,1,0
		); // setup First Person Camera
	}

	glPushMatrix();
		// draw floor
		glColor3f(0,1,0);
		glColorMaterial(GL_FRONT, GL_AMBIENT);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glColorMaterial(GL_FRONT, GL_SPECULAR);		
		glTranslatef(0,-0.2,-1);
		glRotatef(90,1,0,0);
		glScaled(2,2,0.01);
		drawSolidCube();
		glColor3f(1,1,1);
	glPopMatrix();

	// Line Debug of position increment
	/*glPushMatrix();
		glTranslatef(drawcell[0].base.x, 0, drawcell[0].base.z);
		glPointSize(5);
		glScalef(30,30,30);
		glColor3f(1,0,0);
		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(-1 * incre_x,-1 *  incre_y, -1 * incre_z);
		glEnd();
	glPopMatrix();*/
	
	drawMirror(); // ALWAYS call drawMirror before drawRobot Since drawRobot effect Matrixs for drawing smf via drawsmf()
		
	if(cameraOffset.y == preThirdCameraOffset.y){ // if first person view, don't draw models!
		drawRobot();
		drawsmf();
	}

	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)   
}
 
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void postDealKey(){

	if(drawcell[3].nowAngle >= drawcell[3].dof || drawcell[3].nowAngle <= -1 * drawcell[3].dof)
		isReverse = !isReverse;
	
	int key = -1;
	float armInc = 5.0 * (isReverse)?1:-1;

	for(int i = 97; i< 123; ++i){
		if(keyboardState[i])
			key = i;
		else
			continue;

		switch(key){
			case 'w':
				rotateVertex(3, -1 * armInc);
				rotateVertex(4, armInc);
				rotateVertex(5, armInc);
				rotateVertex(6, -1 * armInc);
				robotMovement(0,0,0.01);
				break;
			case 's':
				rotateVertex(3, armInc);
				rotateVertex(4, -1 * armInc);
				rotateVertex(5, -1 * armInc);
				rotateVertex(6, armInc);
				robotMovement(0,0,-0.01);
				break;
			case 'a':
				//robotMovement(0.01,0,0);
				torseRotationAngle += 1;
				break;
			case 'd':
				//robotMovement(-0.01,0,0);
				torseRotationAngle -= 1;
				break;
			case 'q':
				if(cameraOffset.y == preThirdCameraOffset.y)
					cameraOffset = preFirstCameraOffset;
				else
					cameraOffset = preThirdCameraOffset;
				break;
			default : break;
		}
	}
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	keyboardState[key] = true;
	postDealKey();
}
void keyboardUp(unsigned char key, int x, int y)
{
	keyboardState[key] = false;
	postDealKey();
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	glutInit(&argc, argv);								// Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE);	// Enable double buffered mode
	glutInitWindowSize(640, 480);				// Set the window's initial width & height
	glutInitWindowPosition(50, 50);			// Position the window's initial top-left corner
	glutCreateWindow(title);						// Create window with the given title
	glutDisplayFunc(display);							// Register callback handler for window re-paint event
	glutReshapeFunc(reshape);					// Register callback handler for window re-size event
	glutKeyboardFunc(keyboard);				// Reigister callback handler for keyboard event
	glutKeyboardUpFunc(keyboardUp);		// Reigister callback handler for keyboardUp event

	glewExperimental=true;							 // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	initGL();														// Our own OpenGL initialization
	
	glutMainLoop();										// Enter the infinite event-processing loop
	return 0;
}