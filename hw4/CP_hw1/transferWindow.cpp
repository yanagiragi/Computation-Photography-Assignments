#include "transferWindow.h"

#define INFINITY 3.402823e+38

float raw[230][268][3];
float tar[554][366][3];

float transferWindow::calcErrFromAll(int new_x, int new_y, BYTE *newcul)
{
	double err = 0;
	
	for(int i = 0; i < cul_h; ++i){
		for(int j = 0; j < cul_w; ++j){
			int b_err = tar[org_g_y + i][org_g_x + j][0] - newcul[(i * cul_w + j) * 3 + 0];
			int g_err = tar[org_g_y + i][org_g_x + j][1] - newcul[(i * cul_w + j) * 3 + 1];
			int r_err = tar[org_g_y + i][org_g_x + j][2] - newcul[(i * cul_w + j) * 3 + 2];
			// sqrt(square2 + sum) results in too big values
			err += (abs(b_err) + abs(g_err) + abs(r_err)) / 3;
		}
	}

	return err;
}

float transferWindow::calcErrFromBorder(int new_x, int new_y, BYTE *newcul)
{
	double err = 0;
	
	int off = tileOff * cul_w < 0 ? 1 : tileOff * cul_w;
	for(int i = 0; i < off; ++i){
		for(int j = 0; j < cul_w; ++j){
			int b_err = raw[org_g_y + i][org_g_x + j][0] - newcul[(i * cul_w + j) * 3 + 0];
			int g_err = raw[org_g_y + i][org_g_x + j][1] - newcul[(i * cul_w + j) * 3 + 1];
			int r_err = raw[org_g_y + i][org_g_x + j][2] - newcul[(i * cul_w + j) * 3 + 2];
			err += sqrt(b_err * b_err + g_err * g_err + r_err * r_err);
		}
	}

	for(int i = 0; i < off ; ++i){
		for(int j = 0 ; j < off; ++j){
			int b_err = raw[org_g_y + i][org_g_x + j][0] - newcul[(i * cul_w + j) * 3 + 0];
			int g_err = raw[org_g_y + i][org_g_x + j][1] - newcul[(i * cul_w + j) * 3 + 1];
			int r_err = raw[org_g_y + i][org_g_x + j][2] - newcul[(i * cul_w + j) * 3 + 2];			
			err += sqrt(b_err * b_err + g_err * g_err + r_err * r_err);
		}
	}

	return err;
}

BYTE* transferWindow::grab(int x, int y, int len){
	
	if(org_g_x >= width) cul_w = 0;
	else cul_w = org_g_x + len > width ? width - org_g_x - 1 :  len;

	if(org_g_y >= height) cul_h = 0;
	else cul_h = org_g_y + len > height ? height - org_g_y - 1:  len;
		
	g_x = x;
	g_y = y;

	BYTE* culs = new unsigned char[cul_w * cul_h * 3];

	// Find cul from newraw
	for(int i = 0; i < cul_h; ++i){
		for(int j = 0; j < cul_w; ++j){
			for(int k = 0; k < 3; ++k){
				culs[(i * cul_w + j )* 3 + k] = (float)raw[y+i][x+j][k];
			}
		}
	}
	/*printf("Now : %d %d\n", org_g_x, org_g_y);
	printf("Grabing From (%d,%d) to (%d,%d), ",x ,y , x + cul_w - 1 , y + cul_h - 1);
	printf("grabbing space = %d %d, ", cul_w, cul_h);
	printf("size = %d\n",cul_w * cul_h);*/

	return culs;
}

BYTE* transferWindow::selectInitPatch(int fromx, int fromy, int tox, int toy)
{
	BYTE* patch;
	int len = min(cul_w, cul_h);
	int minx = -1, miny = -1;
	float min = INFINITY;

	//org_g_x = g_x;
	//org_g_y = g_y;

	for(int i = 0; i < raw_h - fromy - 1; ++i){
		for(int j = 0; j < raw_w - fromx - 1; ++j){
			BYTE* tmp = grab(fromx + j, fromy + i, len);
			float ssd = calcErrFromAll(fromx,fromy,tmp);
			//printf("%d %d => [%f]\n",i,j,ssd);
			if(ssd < min){
				min = ssd;
				minx = j;
				miny = i;
			}
			free(tmp);
		}
	}
	//printf("min at (%d, %d), SSD = %f\n", minx, miny, min);
	patch = grab(minx,miny, len);
	return patch;
}

BYTE* transferWindow::selectPatch(int fromx, int fromy, int tox, int toy)
{
	BYTE* patch;
	int len = min(cul_w, cul_h);
	int minx = -1, miny = -1;
	double min = INFINITY;

	//org_g_x = g_x;
	//org_g_y = g_y;

	//printf("grabbing %d %d\n", fromx, fromy);
	for(int i = 0; i < toy - fromy - 1; ++i){
		for(int j = 0; j < tox - fromx - 1; ++j){
			//printf("grabbing %d + %d %d + %d\n", fromx,j, fromy,i);
			BYTE* tmp = grab(fromx + j, fromy + i, len);
			
			double ssd = calcErrFromBorder(fromx,fromy,tmp);
			//float ssd = 0;
			printf("%d %d ssd = %f\n",fromx + j, fromy + i, ssd);
			if(ssd < min){
				min = ssd;
				minx = j;
				miny = i;
			}
			free(tmp);
		}
	}
	patch = grab(minx,miny, len);
	return patch;
}

void transferWindow::synthesis()
{
	//int maxtile = max(width / raw_w, height/ raw_h);
	//printf("maxtile = %d\n",maxtile);
	
	/*org_g_y = 40;
	org_g_x = 140;
	cul_w = 40;
	cul_h = 40;
	BYTE* tmp = selectInitPatch(0,0, 40,40);
	for(int i = 0; i < cul_h; ++i){
		for(int j = 0; j < cul_w; ++j){
			for(int k = 0; k < 3; ++k){
				tar[org_g_y + i][org_g_x + j][k] = tmp[( i * cul_w + j ) * 3 + k];
			}
		}
	} return;*/

	int startx = 0;
	int starty = 0;
	cul_w = 5;
	cul_h = 5;

	for(int i2 = 0; i2 < height; i2 += 5){
		for(int j2 = 0; j2 < width; j2 += 5){
			org_g_y = i2;
			org_g_x = j2;
			printf("SelectInitPatch At %d %d\n", i2, j2);
			BYTE* tmp = selectInitPatch(startx,starty, startx + cul_w, starty + cul_h);
			//printf("%d %d\n", i2,cul_w);
			
			for(int i = 0; i < cul_h; ++i){
				for(int j = 0; j < cul_w; ++j){
					for(int k = 0; k < 3; ++k){
						tar[org_g_y + i][org_g_x + j][k] = tmp[( i * cul_w + j ) * 3 + k];
					}
				}
			}
			
			//j2 = width;
			//i2 = height;

			//if(cul_w == 0) break;
			//break;

			free(tmp);
		}
		//break;
		//if(cul_w == 0 && cul_h == 0) break;
	}


	printf("Synthesis Done.\n");
}

void transferWindow::parse()
{
	for(int i = 0; i < raw_h; ++i){
		for(int j = 0; j < raw_w; ++j){
			for(int k = 0; k < 3; ++k)
				raw[i][j][k] = org_raw[(i * raw_w + j) * 3 + k];
		}
	}

	for(int i = 0; i < tar_h; ++i){
		for(int j = 0; j < tar_w; ++j){
			for(int k = 0; k < 3; ++k){
				tar[i][j][k] = org_tar[(i * tar_w + j) * 3 + k];
				//printf("%f\n",targets[(i * tar_w + j) * 3 + k]);
			}
		}
	}

	printf("Parse Done\n");
}

void transferWindow::draw()
{
	if (!valid()) {
		valid(1);
		glLoadIdentity();
		glViewport(0,0,w(),h());
	}

	for(int i = 0; i < tar_h; i++){
		for(int j = 0; j < tar_w; j++){
				// Always Remeber to multiple 3 since it stores RGB Values.
				double b = (double) tar[i][j][0];
				double g = (double) tar[i][j][1];
				double r = (double) tar[i][j][2];
				
				float x  = (float)(j - width/2) / width *2;
				float y = (float)(i - height/2) / height * 2;
				
				glColor3f(b/255,g/255,r/255); // Always Remeber that scale starts from 0.0 to 1.0 !!!!!!

				glBegin(GL_POINTS);
					glVertex2f(x,y);
				glEnd();
			
		}
	}
}