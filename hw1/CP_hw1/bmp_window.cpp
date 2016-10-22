#include "bmp_window.h"

void bmp_window::draw()
{
	if (!valid()) {
		valid(1);
		glLoadIdentity();
		glViewport(0,0,w(),h());
	}
	if(isLoadBmp){
		for(int i = 0; i < bmp_w * bmp_h * 3 ; i += 3){
			double b = (double) pixels[i + 0];
			double g = (double) pixels[i + 1];
			double r = (double) pixels[i + 2];
			float x  = ( (float)((i/3) % bmp_w) - (bmp_w /2) ) / bmp_w * 2 ,	y = ( (float)( i / 3 / bmp_w ) - (bmp_h /2) )  / bmp_h *2;
			
			drawPoint(x,y,r,g,b);
		}
	}
	else {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBegin(GL_QUADS);
					glColor3f(1, 1, 1);glVertex2f(-1, -1);
					glColor3f(1, 1, 1);glVertex2f(-1,  1);
					glColor3f(1, 1, 1);glVertex2f( 1,  1);
					glColor3f(1, 1, 1);glVertex2f( 1, -1);
			glEnd();
	}
}

void bmp_window::drawPoint(float x, float y, float r, float g, float b)
{
		//Fl_Color temp = fl_rgb_color(b,g,r);		
		//gl_color(temp);
		glColor3f(b/255,g/255,r/255); // Always Remeber that scale starts from 0.0 to 1.0 !!!!!!
		glBegin(GL_POINTS);
			glVertex2f(x,y);
		glEnd();				
}

void bmp_window::draw_overlay()
{
	// Draw overlay function. 
	// the valid() property may be used to avoid reinitializing your GL transformation for each redraw:
	if ( !valid() ) {
		valid(1);
		glLoadIdentity();
		glViewport(0,0,w(),h());
	}
}

void bmp_window::loadbmp()
{
	char *filename = "supermario.bmp";	
	if( BMP_Read(filename,&pixels,bmp_w,bmp_h) ){
		printf("Successfully Loading BMP!");
		isLoadBmp = true;
	}
	else {
		printf("Failed to load BMP");
	}
}
bool bmp_window::BMP_Read(const char *filename, unsigned char **data, int &width, int &height)
{
	HBITMAP bmp_handle = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	if(bmp_handle==INVALID_HANDLE_VALUE) return false;
	BITMAP bitmap;
	GetObject(bmp_handle, sizeof(BITMAP), (LPSTR)&bitmap);

	int bit_depth=bitmap.bmPlanes * bitmap.bmBitsPixel;
	if(bit_depth!=8 && bit_depth!=24 && bit_depth!=32)
	{
		printf("Error: File not open or its image depth is not supported %d.\n", filename, bit_depth); 
		DeleteObject(bmp_handle); 
		return false;
	}
	
	width  = bitmap.bmWidth;
	height = bitmap.bmHeight;
    if(*data) delete *data;
	*data=new unsigned char[width*height*3];
	
	unsigned char *ptr=(unsigned char *)(bitmap.bmBits);
	for(int j=0; j<height; j++)
	{
		unsigned char *line_ptr=ptr;
		for(int i=0; i<width; i++) 
		{
			if(bit_depth==8)
			{
				(*data)[(j*width+i)*3+0]=line_ptr[0];
				(*data)[(j*width+i)*3+1]=line_ptr[0];
				(*data)[(j*width+i)*3+2]=line_ptr[0];
				line_ptr++;
			}
			if(bit_depth==24)
			{
				(*data)[(j*width+i)*3+0]=line_ptr[2];
				(*data)[(j*width+i)*3+1]=line_ptr[1];
				(*data)[(j*width+i)*3+2]=line_ptr[0];
				line_ptr+=3;
			}
			if(bit_depth==32)
			{
				(*data)[(j*width+i)*3+0]=line_ptr[2];
				(*data)[(j*width+i)*3+1]=line_ptr[1];
				(*data)[(j*width+i)*3+2]=line_ptr[0];
				line_ptr+=4;
			}
		}
		ptr+=bitmap.bmWidthBytes;
	}

	DeleteObject(bmp_handle);
	return true;
}