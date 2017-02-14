#ifndef _IMAGEQUILT_H
#define  _IMAGEQUILT_H

#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/Fl_Gl_Window.H>
#include <math.h>

class ImageQuilt : public Fl_Gl_Window 
{
	
	static void Timer_CB(void *userdata)
	{
			ImageQuilt *pb = (ImageQuilt*)userdata;
			pb->redraw();
			Fl::repeat_timeout(1.0/1.0, Timer_CB, userdata);
	}
	private:
		int tile;
		BYTE* raw;
		BYTE* res;
		
		
	public:
		ImageQuilt(
			BYTE* data, 
			int tileAmm,
			int nowmode,
			int grabx,
			int graby,
			int culpoint,
			int x,
			int y,
			int w,
			int h,
			const char *l=0
			) 
			: Fl_Gl_Window(x,y,w,h,l)

		{
			mode( FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL );
			Fl::add_timeout(1.0/24.0, Timer_CB, (void*)this);
			glViewport(x,y,w,h);

			frame = 0;					
			width = w;
			height = h;
			raw = data;
			res = cul = NULL;
			tile = tileAmm;
			raw_w = width / tile;
			raw_h = height / tile;
			cul_w = cul_h = 0;
			modes = nowmode;
			g_x = grabx;
			g_y = graby;
			cul_p = culpoint;
			tileOff = .5;			

			res = new unsigned char[width*height*3];			
			//newraw = new float[1024][1024][3];
			/*newraw = (float***)malloc(sizeof(float**) * 1024);
			for(int i = 0; i < 1024; ++i){
				newraw[i] = (float**)malloc(sizeof(float*) * 1024);
				for(int j = 0; j < 1024; ++j){
					newraw[i][j] = (float*)malloc(sizeof(float) * 3);
					newraw[i][j][0] = 0;
					newraw[i][j][1] = 0;
					newraw[i][j][2] = 0;
				}
			}*/

			/*for(int i = 0; i < tile * tile; ++i){
				for(int j = 0; j < raw_w * raw_h * 3; j ++){
					res[raw_w * raw_h * 3 * i + j ] = raw[j];
					//res[raw_w * raw_h * 3 * i + j ] = 250;
				}
			}*/
			
			// paste raw , place else white
			for(int i = 0; i < tile * tile; ++i){
				for(int j = 0; j < raw_w * raw_h * 3; j ++){
					res[raw_w * raw_h * 3 * i + j ] = 0;
				}
			}

			for(int i = 0; i < raw_h; ++i)
				for(int j = 0; j < raw_w ; ++j)
					for(int k = 0; k < 3; ++k)
						res[raw_w * i * 3 + j * 3 + k] = raw[raw_w * i * 3 + j * 3 + k];
						
			parseRaw();	

			debug = false;
		}

		~ImageQuilt()
		{
			delete res;			
			/*for(int i = 0; i < 1024; ++i){
				for(int j = 0; j < 1024; ++j){
					free(newraw[i][j]);
				}
				free(newraw[i]);
			}
			free(newraw);*/
			free(cul);
		}


		int modes;
		void trans();
		BYTE* grab(int , int, int);
		void Synthesis();
		void draw();
		void parseRaw();
		void paste(int,int);
		void paste_hor(int,int);
		void paste_ver(int,int);
		void patch(int,int, int);
		void previewResWithCul();
		double calcPixelErr(int new_x, int new_y, unsigned char *newcul);
		int calcOffset(int i, int j,int start_x, int start_y);
		int ImageQuilt::chooseOverlap_hor();
		/*bool ImageQuilt::specialcases();
		bool ImageQuilt::specialcases_For_mini();*/
		double sqrt(int x) { return sqrt((double)x); }

		void preview_debug();
		
		float tileOff;
		BYTE* cul;
		bool debug;
		int frame, width, height, raw_w, raw_h, cul_w, cul_h, g_x, g_y, cul_p;
		int org_g_x, org_g_y,org_cul_w,org_cul_h;
		//const int overlap;
		int overlap;
		//float ***newraw;
};

#endif