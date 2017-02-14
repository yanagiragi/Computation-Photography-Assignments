#include "previewBMP.h"

void previewBMP::draw(){
	if (!valid()) {
		valid(1);
		glLoadIdentity();
		glViewport(0,0,w(),h());
	}

	if(bmp.pixels == NULL){
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBegin(GL_QUADS);
					glColor3f(1, 1, 1);glVertex2f(-1, -1);
					glColor3f(1, 1, 1);glVertex2f(-1,  1);
					glColor3f(1, 1, 1);glVertex2f( 1,  1);
					glColor3f(1, 1, 1);glVertex2f( 1, -1);
			glEnd();
	}
	else{
		// preview BMP
		for(int i = 0; i < bmp_w * bmp_h * 3 ; i += 3){
			double b = (double) bmp.pixels[i + 0];
			double g = (double) bmp.pixels[i + 1];
			double r = (double) bmp.pixels[i + 2];
			float x  = ( (float)((i/3) % bmp_w) - (bmp_w /2) ) / bmp_w * 2 ,	y = ( (float)( i / 3 / bmp_w ) - (bmp_h /2) )  / bmp_h *2;
			
			glColor3f(b/255,g/255,r/255); // Always Remeber that scale starts from 0.0 to 1.0 !!!!!!
			glBegin(GL_POINTS);
				glVertex2f(x,y);
			glEnd();			
		}
	}
}

void previewBMP::change(int index){
	
	char *buf;
	switch(index){
		case 0: buf = "texture/berry.bmp"; break;
		case 1: buf = "texture/btile.bmp"; break;
		case 2: buf = "texture/cans_sc.bmp"; break;
		case 3: buf = "texture/fabric.bmp"; break;
		case 4: buf = "texture/greek.bmp"; break;
		case 5: buf = "texture/lobelia.bmp"; break;
		case 6: buf = "texture/mesh.bmp"; break;
		case 7: buf = "texture/rice.bmp"; break;
		case 8: buf = "texture/wool.bmp"; break;
		case 9: buf = "texture/pdir2.bmp"; break;
		case 10: buf = "texture/rock.bmp"; break;
	}
	bmp.pixels = NULL;
	bool res = bmp.BMP_Read(buf, &(bmp.pixels), bmp_w, bmp_h);
	if(res)
		printf("Change to %s\n", buf);
	else
		printf("Unable to Change to %s\n", buf);
}