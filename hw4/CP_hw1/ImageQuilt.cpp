#include "ImageQuilt.h"

#define INFINITY 3.402823e+38

/*Vars*/
float newraw[1024][1024][3];

int ImageQuilt::calcOffset(int i, int j, int start_x, int start_y)
{
	int x = (start_x + j) / raw_w;
	int y = (start_y + i) / raw_h;

	if(x == 0 && y == 0) return 0;
	//return (raw_h * raw_w * y * tile + x * raw_h * raw_w - raw_w * raw_h * y) * 3;
	//return (raw_h * raw_w * y * tile + x * raw_h * raw_w - raw_w * raw_h * y) * 3;
	int org = ((start_y + i) * raw_w + (j + start_x))* 3;
	return -1 * org + 
			(
				( y * tile * raw_w * raw_h ) +
				( x * raw_w * raw_h ) + 
				((start_y + i) % raw_h) * raw_w + 
				(start_x + j) % raw_w 
			) * 3
			;
	
	if( (start_x + j >= raw_w) && (start_y + i >= raw_h) ){
		// both x and y exceed single raw pic
		return raw_w * raw_h * 3 * 2  - raw_w * 3;
		// why need minus raw_w * 3 ? => By Observation
	}
	else if(start_x + j >= raw_w){
		// x exceed single raw pic
		return raw_w * raw_h * 3 - raw_w * 3;					
	}
	else if(start_y + i >= raw_h){
		//  y exceed single raw pic
		return raw_w * raw_h * 3;
	}
	else{
		// All Clear, reset offset
		return 0;
	}
}

void ImageQuilt::preview_debug(){
	// Remind that it should never exceed 63 if width/tile = 64
	org_g_x = 47;
	org_g_y = 2;
		
	double mindiff = 1.79769e+308; 
	int mini = -1,minj = -1;	

	for(int i = 0; i < raw_h - width / cul_p; ++i){
		for(int j = 0; j < raw_w - width / cul_p; ++j){				
			BYTE* newcul = grab(i,j, width / cul_p);
			double diff = calcPixelErr(i,j,newcul);
				
			if(diff < mindiff){
				mindiff = diff;
				mini = i;
				minj = j;
			}
		}
	}

	printf("mindiff = %f, x = %d, y = %d\n", mindiff, mini, minj);
	cul = grab(mini,minj, raw_w / cul_p);;
	paste(org_g_x,org_g_y);

	int start_x = g_x;
	int start_y = g_y;
	int offset = 0;
	
	for(int i = 0; i < cul_h; ++i){
		for(int j = 0; j < cul_w; ++j){
			for(int k = 0; k < 3; ++k){				
				if((start_x + j >= (raw_w * tile)) || (start_y + i >= (raw_h * tile))){
					// check if exceed external border
					break;
				}

				if( (start_x + j >= raw_w) && (start_y + i >= raw_h) ){
					// both x and y exceed single raw pic
					offset = raw_w * raw_h * 3 * 2  - raw_w * 3;
					// why need minus raw_w * 3 ? => By Observation
				}
				else if(start_x + j >= raw_w){
					// x exceed single raw pic
					offset = raw_w * raw_h * 3 - raw_w * 3;					
				}
				else if(start_y + i >= raw_h){
					//  y exceed single raw pic
					offset = raw_w * raw_h * 3;
				}
				else{
					// All Clear, reset offset
					offset = 0;
				}

				
				// Border Debug
				if(i == 0 || j == 0 || i == cul_h-1 || j == cul_w-1){
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + k] = 255;
				}
				else{
					//if(cul[(i * cul_w + j )* 3 + 0] == 0 && cul[(i * cul_w + j )* 3 + 1] == 0 && cul[(i * cul_w + j )* 3 + 2] == 0)
						//break;
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + k] = cul[(i * cul_w + j )* 3 + k];
				}
				

			}
		}
	}
	
}

void ImageQuilt::parseRaw(){

	for(int i = 0; i < 1024; ++i)
		for(int j = 0; j < 1024; ++j)
			for(int k = 0; k < 3; ++k)
				newraw[i][j][k] = 0;

	// parse raw to newraw (float[][][3])
	for(int i = 0; i < raw_h; ++i){
		for(int j = 0; j < raw_w; ++j){
			for(int k = 0; k < 3; ++k){
				newraw[i][j][k] = (float)raw[(i * raw_w + j) * 3 + k];
			}
		}
	}
}

void ImageQuilt::patch(int x, int y,  int flag = 0){
	org_g_x = x;
	org_g_y = y;
	
	if(flag == 0)
		cul = grab(0, 0 , min(raw_w, raw_h));
	else if (flag == 1)
		cul = grab(0, 0 , raw_w);
	else
		cul = grab(0, 0 , raw_h);

	if(org_g_x == 0)
		paste_ver(org_g_x,org_g_y);
	else if(org_g_y == 0)
		paste_hor(org_g_x,org_g_y);
	else
		paste(org_g_x,org_g_y);
}

void ImageQuilt::paste_ver(int g_x,int g_y){
	
	// start place cul on (g_x, g_y)
	int start_x = g_x;
	int start_y = g_y;
	int offset = 0;
	
	float min = 3.402823e+38;
	int* mina;	
	mina = (int *)malloc(sizeof(int) * cul_w);

	for(int i = 0; i < cul_w; ++i)
		mina[i] = 0;

	for(int a = 0; a < cul_w; ++a, min = 3.402823e+38){
		
		float *dp = (float*)malloc(sizeof(float) * overlap);
		for(int a = 0; a < overlap; ++a)
			dp [a] = 0;

		for(int i = 0; i < overlap; ++i){
			
			offset = calcOffset(a, i, start_x, start_y);
			
			if(offset + ((i + start_y) * raw_w + (a + start_x)) > height * width * 3 || ( i * cul_w + a )* 3 + 3 > cul_w * cul_h){
				/*for(int tmp = i; tmp < overlap; ++tmp)
					dp[tmp] = INFINITY;
				break;*/
				break;
			}

			for(int k = 0; k < 3; ++k){
				float resf = res[offset + ((i  + start_y) * raw_w + (a + start_x))* 3 + k] - cul[( i * cul_w + a )* 3 + k];
				dp[i] += pow(resf,2);
			}
		}
				
		for(int i = 0; i < overlap; ++i){
			if(dp[i] < min){
				if(i == 0){
					min = dp[i];
					mina[a] = i;
				}
				else{
					if(abs(i - mina[a - 1]) <= 0.05 * overlap){ // use const .1 * overlap make the cutting curve smoother
						min = dp[i];
						mina[a] = i;
					}
				}
			}
		}
		
		if(min == INFINITY){ // not found, increase/decrease one unit(.1 * overlap) with cloest result
			int mina_temp = mina[a - 1];
			for(int i = 0; i < overlap; ++a){
				if(dp[i] < min){
					min = dp[i];
					mina_temp = i;
				}
			}
			mina[a] = mina_temp;
			if(mina_temp > mina[a - 1]){
				mina[a] = mina[a - 1] + 0.05 * overlap;
			}
			else if(mina_temp < mina[a - 1]){
				mina[a] = mina[a - 1] - 0.05 * overlap;
			}
			else{
				mina[a] = mina[a - 1];
			}
		}
	}
	
	for(int i = 0; i < cul_h; ++i){
		for(int j = 0; j < cul_w; ++j){
			for(int k = 0; k < 3; ++k){		
				if((start_x + j) >= (raw_w * tile) || (start_y + i >= (raw_h * tile))){
					break; // check if exceed external border
				} 
				
				offset = calcOffset(i,j,start_x, start_y);
				
				if(i > mina[j]){ // Start Pasting
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + k] = cul[(i * cul_w + j )* 3 + k];
				}
				else{
					;// Do Nothing
				}
			}
		}
	}

	// For debug
	if(debug){
		for(int i = 0; i < cul_h; ++i){
			for(int j = 0; j < cul_w; ++j){
				offset = calcOffset(i,j,start_x, start_y);
				//if(start_y + i > height || start_x + j > width) break; 
				/*if(i == overlap){				
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 0] = 255;
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 1] = 255;
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 2] = 255;
				
				}*/
				//printf("mini = %d\n", mina[j]);
				if(i < overlap){
					if(i == mina[j]){
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 0] = 255;
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 1] = 255;
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 2] = 255;
					}
				}

				/*if(i == cul_h -1 || i == 0 || j == 0 || j == cul_w -1){
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 0] = 244;
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 1] = 223;
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 2] = 66;
				}*/
			}
		}
	}

}

void ImageQuilt::paste_hor(int g_x,int g_y){
	int start_x = g_x;
	int start_y = g_y;
	int offset = 0;
	float min = INFINITY;
	int* mina;	
	mina = (int *)malloc(sizeof(int) * cul_h);
	
	for(int i = 0; i < cul_h; ++i)
		mina[i] = 0;

	for(int i = 0; i < cul_h; ++i, min = 3.402823e+38){
		
		float *dp = (float*)malloc(sizeof(float) * overlap);
		for(int a = 0; a < overlap; ++a)
			dp [a] = 0;

		for(int a = 0; a < overlap; ++a){
			
			offset = calcOffset(i, a,start_x, start_y);

			for(int k = 0; k < 3; ++k){
				float resf = res[offset + ((start_y + i) * raw_w + (a + start_x))* 3 + k] - cul[(i * cul_w + a )* 3 + k];
				dp[a] += pow(resf,2);
			}
		}
						
		for(int a = 0; a < overlap; ++a){
			if(dp[a] < min){
				if(i == 0){
					min = dp[a];
					mina[i] = a;
				}
				else{
					if(abs(a - mina[i - 1]) < 0.1 * overlap){
						// use const .1 * overlap make the cutting curve smoother
						min = dp[a];
						mina[i] = a;
					}
				}
			}
		}
		if(min == INFINITY){ // not found, increase/decrease one unit(.1 * overlap) with cloest result
			int mina_temp = mina[i - 1];
			for(int a = 0; a < overlap; ++a){
				if(dp[a] < min){
					min = dp[a];
					mina_temp = a;
				}
			}
				
			mina[i] = mina[i - 1] + (mina_temp > mina[i - 1]) ? 0.1 * overlap : ((mina_temp < mina[i - 1]) ? -0.1 * overlap : 0 );
		}
	}

	for(int i = 0; i < cul_h; ++i){
		for(int j = 0; j < cul_w; ++j){
			for(int k = 0; k < 3; ++k){				
				
				// start counting offset
				//if((start_x + j) >= (raw_w * tile) || (start_y + i >= (raw_h * tile))){
					// check if exceed external border
					//break;
				//} 
				
				offset = calcOffset(i,j,start_x, start_y);

				// Start Pasting
				//res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + k] = cul[(i * cul_w + j )* 3 + k];
				if(j >= mina[i]){
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + k] = cul[(i * cul_w + j )* 3 + k];
				}
				else{
					; // Do Nothing 
				}		
				// End pasting

			}
		}
	}

	
	if(debug){ // For debug
		for(int i = 0; i < cul_h; ++i){
			for(int j = 0; j < cul_w; ++j){
				if(j + start_x > width || start_y + i > height || offset + ((start_y + i) * raw_w + (j + start_x)) > width * height) continue;
				offset = calcOffset(i,j,start_x, start_y);
				/*if(j == overlap){				
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 0] = 255;
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 1] = 255;
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 2] = 255;
				
				}*/
				if(j < overlap){
					if(j == mina[i]){
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 0] = 255;
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 1] = 255;
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 2] = 255;
					}
				}
			
				/*if(i == cul_h -1 || i == 0 || j == 0 || j == cul_w -1){
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 0] = 244;
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 1] = 223;
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 2] = 66;
				}*/

			}
		}
	}
	free(mina);
}

void ImageQuilt::paste(int g_x,int g_y){
	
	// start place cul on (g_x, g_y)
	int start_x = g_x;
	int start_y = g_y;
	int offset = 0;
	
	float min = INFINITY;
	int* minx;
	int* miny;

	minx = (int *)malloc(sizeof(int) * cul_w);
	for(int i = 0; i < cul_w; ++i)
		minx[i] = 0;

	// min_x at each y
	miny = (int *)malloc(sizeof(int) * cul_h);
	for(int i = 0; i < cul_h; ++i)
		miny[i] = 0;

	//overlap = chooseOverlap();

	for(int i = 0; i < cul_h; ++i, min = 3.402823e+38){		
		float *dp = (float*)malloc(sizeof(float) * overlap);
		for(int a = 0; a < overlap; ++a)
			dp [a] = 0;

		for(int a = 0; a < overlap; ++a){
			offset = calcOffset(i, a,start_x, start_y);
			
			if((offset + ((start_y + i) * raw_w + (a + start_x))* 3) > height * width * 3  || ( i * cul_w + a )* 3 + 3 > cul_w * cul_h){
				dp[a] = INFINITY;
				continue;
			}
			
			for(int k = 0; k < 3; ++k){
				float resf = res[offset + ((start_y + i) * raw_w + (a + start_x))* 3 + k] -	cul[(i * cul_w + a )* 3 + k];
				dp[a] += pow(resf,2);
			}
		}
						
		for(int a = 0; a < overlap; ++a){
			if(dp[a] < min){
				if(i == 0){
					min = dp[a]; miny[i] = a;
				}
				else{
					if(abs(a - miny[i - 1]) < 0.1 * overlap){ // use const .1 * overlap make the cutting curve smoother
						min = dp[a]; miny[i] = a;
					}
				}
			}
		}

		if(min == INFINITY){ // not found, increase/decrease one unit(.1 * overlap) with cloest result
			int miny_temp = miny[i - 1];
			for(int a = 0; a < overlap; ++a){
				if(dp[a] < min){
					min = dp[a];
					miny_temp = a;
				}
			}
				
			miny[i] = miny[i - 1] + (miny_temp > miny[i - 1]) ? 0.1 * overlap : ((miny_temp < miny[i - 1]) ? -0.1 * overlap : 0 );
		}
	} // Calc miny Done.

	for(int a = 0; a < cul_w; ++a, min = 3.402823e+38){
		
		float *dp = (float*)malloc(sizeof(float) * overlap);
		for(int a = 0; a < overlap; ++a)
			dp [a] = 0;

		for(int i = 0; i < overlap; ++i){
			
			offset = calcOffset(a, i, start_x, start_y);
			if((offset + ((start_y + i) * raw_w + (a + start_x))* 3 + 3) > width * height * 3)
				continue;
			for(int k = 0; k < 3; ++k){
				float resf = res[offset + ((i  + start_y) * raw_w + (a + start_x))* 3 + k] - cul[( i * cul_w + a )* 3 + k];
				dp[i] += pow(resf,2);
			}
		}
				
		for(int i = 0; i < overlap; ++i){
			if(dp[i] < min){
				if(i == 0){
					min = dp[i];
					minx[a] = i;
				}
				else{
					if(abs(i - minx[a - 1]) <= 0.1 * overlap){
						// use const .1 * overlap make the cutting curve smoother
						min = dp[i];
						minx[a] = i;
					}
				}
			}
		}
		if(min == INFINITY){ // not found, increase/decrease one unit(.1 * overlap) with cloest result
			int minx_temp = minx[a - 1];
			for(int i = 0; i < overlap; ++a){
				if(dp[i] < min){
					min = dp[i];
					minx_temp = i;
				}
			}
				
			minx[a] = 
				minx[a - 1] + (minx_temp > minx[a - 1]) 
				? (0.1 * overlap > 0 ? 0.1 * overlap : 1) 
				: (
					(minx_temp < minx[a - 1]) ? 
						(-0.1 * overlap < -1 
						? -0.1 * overlap 
						: -1
						)  
					: 0 
				);
		}
	}
	
	for(int i = 0; i < cul_h; ++i){
		for(int j = 0; j < cul_w; ++j){
			for(int k = 0; k < 3; ++k){
				if((start_x + j) >= (raw_w * tile) || (start_y + i >= (raw_h * tile)))
					break; // check if exceed external border

				offset = calcOffset(i,j,start_x, start_y);
				
				int index = offset + ((start_y + i) * raw_w + (j + start_x))* 3 + k;
				if(index < (width * height * 3) ){
					if(j > miny[i] && i > minx[j]){
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + k] = cul[(i * cul_w + j )* 3 + k];
					}
				}
				
			}
		}
	}

	// For debug
	if(debug){
		for(int i = 0; i < cul_h; ++i){
			for(int j = 0; j < cul_w; ++j){
				offset = calcOffset(i,j,start_x, start_y);
				if(
						(start_y + i) >= height || 
						(j + start_x) >= width || 
						(offset + ((start_y + i) * raw_w + (j + start_x))) > (width * height) 
						
						
					)
					continue;
				if(i < overlap){
					if(i == minx[j]){
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 0] = 244;
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 1] = 241;
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 2] = 61;
					}
				}
				if(j < overlap){
					if(j == miny[i]){
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 0] = 64;
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 1] = 244;
						res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 2] = 206;
					}
				}
				
				/*if(i == cul_h -1 || i == 0 || j == 0 || j == cul_w -1){
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 0] = 244;
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 1] = 223;
					res[offset + ((start_y + i) * raw_w + (j + start_x))* 3 + 2] = 66;
				}*/
			}
		}
	}

}

int ImageQuilt::chooseOverlap_hor()
{
	// never used
	float min = INFINITY;
	int constoverlap = 0.5 * min(cul_w, cul_h) + 1;
	int nowi = constoverlap;
	float noww = 0;
	
	printf("constoverlap = %d\n", constoverlap);
	printf("choose overlap from raw(%d %d) cul(%d %d) = ",org_g_x, org_g_y, g_x,g_y);
	
	if(g_y == 0){
		for(int cc = 1; cc < constoverlap; ++cc, noww = 0){
			for(int j = 0; j < cul_h; ++j){
				for(int i = 0; i < cc; ++i){
					for(int k = 0; k < 3; ++k){
						//noww += 0;// pow((float)(newraw[org_g_y + k][org_g_x + cul_w - cc + i][k] - cul[(g_y + j) * cul_w + i + g_x][k]), 2);
						int r = newraw[org_g_y + k][org_g_x + cul_w - cc + i][k] - cul[((g_y + j) * cul_w + i + g_x) * 3 + k];
						noww += sqrt(pow((float)r, 2));
					}
					//printf("%d %d - %d %d\n",org_g_x + cul_w - cc + i,  org_g_y + j, i + g_x, g_y + j);
				}
			}
			//printf("\ncc = %d, noww = %f, min = %f\n", cc, noww / cc, min);
			if(noww / cc < min){				
				min = noww / cc;
				nowi = cc;
			}
		}
	}
	printf("%d\n", nowi);
	//return nowi;
	return constoverlap;
}

void ImageQuilt::Synthesis(){

	
	if(modes == -1){
		for(int i = 0; i < tile * tile; ++i){
			for(int j = 0; j < raw_w * raw_h * 3; j ++){
				res[raw_w * raw_h * 3 * i + j ] = raw[j];
			}
		}
	}
	else if(modes == 0){
		int off = tileOff * org_cul_w < 0 ? 1 : tileOff * org_cul_w;
		
		overlap = raw_w / 6;
		for(int i = raw_w; i < width + overlap; i += raw_w - overlap){
			patch(i - overlap, 0, 1);
		}

		overlap = raw_h / 3;
		for(int i = raw_h; i < height + overlap; i += raw_h - overlap){
			patch(0, i - overlap, 2);
			//break;
		}

		
		int mini = min(raw_w, raw_h);
		int maxi = max(width, height);
		overlap = mini / 6;
		/*for(int i2 = raw_w; i2 < width + overlap; i2 += mini - overlap){
			for(int i1 = raw_h - overlap; i1 < height + overlap; i1 += mini - overlap){
				patch(i1 - overlap, i2 - overlap);				
			}
		}*/

		for(int i2 = mini; i2 < maxi + overlap; i2 += mini - overlap){
			for(int i1 = mini - overlap; i1 < maxi + overlap; i1 += mini - overlap){
				patch(i1 - overlap, i2 - overlap);				
			}
		}


		/*for(int j = 0; j < width / raw_w * cul_p; ++j){
			printf("patch %d %d from minus overlap of %d %d! \n",width / cul_p * (j + 1) - overlap, 0 ,width / cul_p * (j + 1) , 0);
			patch(raw_w / cul_p * (j + 1) - overlap, 0);
		}

		overlap = min(raw_h, raw_w) / tile / 6;
		for(int i = raw_h / cul_p; i < height; i += raw_h / cul_p){
			patch(0, i - overlap);
			//cul_p = 2 + rand() % 2;
		}*/

		/*for(int i = raw_h / cul_p; i < height; i += raw_h / cul_p){
			for(int j = raw_w / cul_p; j < width; j += raw_w / cul_p){
				patch( j - overlap , i - overlap);
			}
		}*/
	}
	else if(modes == 1){
		preview_debug();
	}

}

double ImageQuilt::calcPixelErr(int new_x, int new_y, unsigned char *newcul)
{
	double err = 0;
	
	int off = tileOff * org_cul_w < 0 ? 1 : tileOff * org_cul_w;

	// No Considering grab() that sizes <= raw_h * raw_w,
	//printf("cul_w = %d \n",cul_w);
	// top
	for(int i = 0; i < off; ++i){
		int j;
		for(j = 0; j < cul_w; ++j){
			int b_err = newraw[org_g_y + i][org_g_x + j][0] - newcul[(i * cul_w + j) * 3 + 0];			
			int g_err = newraw[org_g_y + i][org_g_x + j][1] - newcul[(i * cul_w + j) * 3 + 1];
			int r_err = newraw[org_g_y + i][org_g_x + j][2] - newcul[(i * cul_w + j) * 3 + 2];
			
			err += sqrt(b_err * b_err + g_err * g_err + r_err * r_err);
			
			
		}
		//printf("%d %d %d %d\n",org_g_y + i,org_g_x + j,i,j);
		//system("PAUSE");
	}

	//if(new_x == 0 && new_y == 31)
	//	printf("err  = %f\n",err);
	// bottom
	/*for(int i = 0; i < off; ++i){
		for(int j = 0; j < cul_w; ++j){			
			int b_err = newraw[org_g_y  + cul_h - i -1][org_g_x + j][0] - newcul[((cul_h - i - 1) * cul_w + j) * 3 + 0];
			int g_err = newraw[org_g_y  + cul_h - i -1][org_g_x + j][1] - newcul[((cul_h - i - 1) * cul_w + j) * 3 + 1];
			int r_err = newraw[org_g_y + cul_h - i -1][org_g_x + j][2] - newcul[((cul_h - i - 1) * cul_w + j) * 3 + 2];						
			err += sqrt(b_err * b_err + g_err * g_err + r_err * r_err);
			//printf("%d %d %d %d\n",org_g_y  + cul_h - i -1, org_g_x + j, cul_h - i - 1 , j);
			//system("PAUSE");
		}
	}*/
	
	// left 
	//printf("off  = %f\n",off);
	for(int i = 0; i < off ; ++i){
		for(int j = 0 ; j < off; ++j){
			int b_err = newraw[org_g_y + i][org_g_x + j][0] - newcul[(i * cul_w + j) * 3 + 0];
			int g_err = newraw[org_g_y + i][org_g_x + j][1] - newcul[(i * cul_w + j) * 3 + 1];
			int r_err = newraw[org_g_y + i][org_g_x + j][2] - newcul[(i * cul_w + j) * 3 + 2];			
			//printf("%d %d %d %d\n", org_g_y + i, org_g_x + j, i , j);
			//system("PAUSE");
			err += sqrt(b_err * b_err + g_err * g_err + r_err * r_err);
		}
	}

	// right
	/*for(int i = 0; i < cul_h - off * 2; ++i){
		for(int j = 0 ; j < off; ++j){
			int b_err = newraw[org_g_y + off + i][org_g_x + cul_w - off + j ][0] - newcul[((i + off) * cul_w + cul_w - off + j  ) * 3 + 0];
			int g_err = newraw[org_g_y + off + i][org_g_x + cul_w - off + j ][1] - newcul[((i + off) * cul_w + cul_w - off + j  ) * 3 + 1];
			int r_err = newraw[org_g_y + off + i][org_g_x + cul_w - off + j ][2] - newcul[((i + off) * cul_w + cul_w - off + j  ) * 3 + 2];			
			err += sqrt(b_err * b_err + g_err * g_err + r_err * r_err);			
		}		
		//printf("%d %d %d %d\n", org_g_y + off + i , org_g_x + cul_w - off + j  , i + off, cul_w - off + j );
		//system("PAUSE");
	}*/

	
	return err;
	//return rand() % 100;
}

BYTE* ImageQuilt::grab(int x, int y, int len){
	
	if(res == NULL) return NULL;

	if(org_g_x >= width) cul_w = 0;
	else cul_w = org_g_x + len > width ? width - org_g_x - 1 :  len;

	if(org_g_y >= height) cul_h = 0;
	else cul_h = org_g_y + len > height ? height - org_g_y - 1:  len;
		
	//printf("len (%d %d) -> grab(%d, %d)\n", len, len, cul_w, cul_h);
	g_x = x;
	g_y = y;

	// if(&cul) delete &cul;
	BYTE* culs = new unsigned char[cul_w * cul_h * 3];

	// Find cul from newraw
	for(int i = 0; i < cul_h; ++i){
		for(int j = 0; j < cul_w; ++j){
			for(int k = 0; k < 3; ++k){
				culs[(i * cul_w + j )* 3 + k] = (float)newraw[y+i][x+j][k];
				//printf("%f ",(float)newraw[y+i][x+j][k]);
			}
		}
	}
	/*printf("Now : %d %d\n", org_g_x, org_g_y);
	printf("Grabing From (%d,%d) to (%d,%d), ",x ,y , x + cul_w - 1 , y + cul_h - 1);
	printf("grabbing space = %d %d, ", cul_w, cul_h);
	printf("size = %d\n",cul_w * cul_h);*/

	return culs;
}

void ImageQuilt::previewResWithCul()
{
	for(int i = 0; i < tile; i++){
		for(int j = 0; j < tile; j++){
			for(int k = 0; k < raw_w * raw_h * 3; k +=3){
				// Always Remeber to multiple 3 since it stores RGB Values.
				double b = (double) res[raw_w * raw_h * i * tile * 3 + raw_w * raw_h * j *3  + k + 0];
				double g = (double) res[raw_w * raw_h * i * tile  * 3 + raw_w * raw_h * j * 3 + k + 1];
				double r = (double) res[raw_w * raw_h * i  * tile * 3 + raw_w * raw_h * j  * 3 + k + 2];
				
				float x  = (j * raw_w + (float)( (k/3) % raw_w) - width/2)  / width *2;
				float y = (i * raw_h + (float)( k/ 3 / raw_w ) - height /2 )  / height * 2;
			
				glColor3f(b/255,g/255,r/255); // Always Remeber that scale starts from 0.0 to 1.0 !!!!!!
				glBegin(GL_POINTS);
					glVertex2f(x,y);
				glEnd();
			}
		}
	}
}

void ImageQuilt::draw(){
	if (!valid()) {
		valid(1);
		glLoadIdentity();
		glViewport(0,0,w(),h());
	}
	
	if(res == NULL){
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBegin(GL_QUADS);
					glColor3f(1, 1, 0);
					glVertex2f(-1, -1);
					glVertex2f(-1,  1);
					glVertex2f( 1,  1);
					glVertex2f( 1, -1);
			glEnd();
	}
	else{
		if(modes == 0){ // preview BMP
			previewResWithCul();
		}
		else{
			// preview grab
			int start_x = g_x;
			int start_y = g_y;
			int offset = 0;
	
			for(int i = 0; i < tile; i++){
				for(int j = 0; j < tile; j++){
					for(int k = 0; k < raw_w * raw_h * 3; k +=3){
						// Always Remeber to multiple 3 since it stores RGB Values.
						double b = (double) res[raw_w * raw_h * i * tile * 3 + raw_w * raw_h * j *3  + k + 0];
						double g = (double) res[raw_w * raw_h * i * tile  * 3 + raw_w * raw_h * j * 3 + k + 1];
						double r = (double) res[raw_w * raw_h * i  * tile * 3 + raw_w * raw_h * j  * 3 + k + 2];
				
						float x  = (j * raw_w + (float)( (k/3) % raw_w) - width/2)  / width *2;
						float y = (i * raw_h + (float)( k/ 3 / raw_w ) - height /2 )  / height * 2;
			
						glColor3f(b/255,g/255,r/255); // Always Remeber that scale starts from 0.0 to 1.0 !!!!!!
						glBegin(GL_POINTS);
							glVertex2f(x,y);
						glEnd();
					}
				}
			}
		}
	}
}

/*bool ImageQuilt::specialcases()
{
	// weird cases
	//if(raw_w == 192 && raw_h == 192) return true;
	if(width > height && tile == 5) return true;
	return false;
}

bool ImageQuilt::specialcases_For_mini()
{
	// weird cases
	return false;
	if(raw_w == 64 && raw_h == 64 && (tile == 3 || tile == 5)) return true; // bile
	if(raw_w == 128 && raw_h == 128 && tile >= 4 ) return true; // berry
	if(raw_w == 192 && raw_h == 192 && tile >= 4 ) return true; // can or mesh
	if(raw_w == 170 && raw_h == 170 && tile >= 3 ) return true; // frabic
	if(raw_w == 166 && raw_h == 150 && tile >= 3 ) return true; // greek
	if(raw_w == 268 && raw_h == 230 && tile >= 2) return true; // lobelia
	if(raw_w == 178 && raw_h == 178 && tile >= 3) return true; // wool
	if(raw_w == 197 && raw_h == 133 ) return true; // rice
	//if(height > width) return true;
	return false;
}*/