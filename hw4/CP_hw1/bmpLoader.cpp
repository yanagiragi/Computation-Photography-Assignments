#include "bmpLoader.h"

bool bmpLoader::BMP_Read(const char *filename, unsigned char **data, int &width, int &height)
{
	HBITMAP bmp_handle = (HBITMAP)LoadImageA(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	if(bmp_handle==INVALID_HANDLE_VALUE) return false;
	BITMAP bitmap;
	GetObject(bmp_handle, sizeof(BITMAP), (LPSTR)&bitmap);

	int bit_depth=bitmap.bmPlanes * bitmap.bmBitsPixel;
	if(bit_depth!=8 && bit_depth!=24 && bit_depth!=32)
	{
		printf("Error: File %s not open or its image depth is not supported %d.\n", filename, bit_depth); 
		DeleteObject(bmp_handle); 
		return false;
	}
	
	width  = bitmap.bmWidth;
	height = bitmap.bmHeight;
	printf("%s: width=%d, height=%d\n",filename,width,height);
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