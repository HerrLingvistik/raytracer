#include <stdio.h>
#include <stdlib.h>
#include "utils/vector.h"
#include "utils/ray.h"
#include <math.h>
//Något är fel med kameran saker syns inte i vissa positioner
const int dimX =512, dimY = 512;

typedef struct{
	vec3 pos;
}camera;

float *pixels;

void createImage(){
	camera cam;
	cam.pos = vecInit2(0.0f, 0.0f, 0.0f);
	int i, x=0, y=0;
	vec3 cameraRay;
	vec3 pixelPos;
	float pixSize = 2.0f/dimX;
	for(i=0; i<dimX*dimY*3; i+=3){
		//Ray r;
		vec3 col;
		int tempX = 0, tempY=0;
		int j = 0;
		for(j = 0; j<16; j++){
			
			pixelPos = vecAdd(vecInit2(-1.0f, 1.0f, 4.6f), vecInit2(x*pixSize + tempX*pixSize/3.0f, -y*pixSize - tempY*pixSize/3.0f, 0.0f));
			cameraRay = vecNormalize(vecSub(pixelPos, cam.pos));		
			col = vecAdd(col, shootRay(cam.pos, cameraRay));
			tempX++;
			if(tempX>3){
				tempX=0;
				tempY++;
			}		
		}		

		col = vecInit2(col.x/16.0f, col.y/16.0f, col.z/16.0f);

		x++; 
		if(x==dimX){
			x=0;
			y++;	
			//vecPrint(pixelPos);
		}		
		

		pixels[i] = col.x;
		pixels[i+1] = col.y;
		pixels[i+2] = col.z;
	}
	
}
  
int findMax(){
	float maxRGB;
	int i;	
	for (i = 0; i < dimY*dimY*3; i++)
  {
    maxRGB = (pixels[i] > maxRGB) ? pixels[i] : maxRGB;
  }
	printf("%f\n", maxRGB);
	return maxRGB;
}

void writeImage(){
	//float scale = findMax();
  int i;
  FILE *fp = fopen("first.ppm", "wb"); /* b - binary mode */
  (void) fprintf(fp, "P6\n%d %d\n255\n", dimX, dimY);
  for (i = 0; i < dimY*dimY*3; i+=3)
  {
    static unsigned char color[3];
    color[0] = fmin(pixels[i], 1.0f)*255;//i % 256;  /* red */
    color[1] = fmin(pixels[i+1],1.0f)*255;//j % 256;  /* green */
    color[2] = fmin(pixels[i+2], 1.0f)*255;//(i * j) % 256;  /* blue */
    (void) fwrite(color, 1, 3, fp); 
  }
  (void) fclose(fp);
}

int main(){
	printf("Scatmans world!!!\n");
	/*vec3 v1 = vecInit2(1,0,1);
	vec3 v2 = vecInit2(0,-1,0);
	printf(" ANGLE BRE %f", vecAngle(v1,v2));*/
	pixels =(float *) malloc(dimX*dimY*3*sizeof(float));
	createImage();
	
	writeImage();
	//vec3 start = vecInit();
	vec3 test = vecInit2(2.0f,1.0f,1.0f);
	//ray r = shootRay(start, dir);
	vecPrint(vecNormalize(test));
	//vecPrint(vecMult(2.0f, dir));	

	//vec3 v = vecInit2(3,3,3), v2 = vecInit2(2,2,2);

	//printf("struters: %f \n", vecDot(v, v2));

	return 0;
}
