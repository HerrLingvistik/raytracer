#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "sphere.h"

//Sphere sphere;

void initSphere(Sphere *sphere, vec3 p, float r, vec3 c, int trans){
	sphere->pos = p;
	sphere->rad = r;
	sphere->col = c;
	sphere->trans = trans;
}

void sphereIntersection(vec3 start, vec3 dir, int inside, vec3 *intPoint, vec3 *intColor, vec3 *intNormal, int *transparent, Sphere sphere[2]){
	
	vec3 tempPoint;	
	float underRoot;
	int i;	
	*intPoint = vecInit2(10000.0f, 0.0f, 0.0f);
	for(i=0; i<2; i++){
		underRoot =  vecDot(dir, vecSub(start, sphere[i].pos)) * vecDot(dir, vecSub(start, sphere[i].pos)) - vecNorm(vecSub(start, sphere[i].pos)) * vecNorm(vecSub(start, sphere[i].pos)) + sphere[i].rad*sphere[i].rad;// ( (l.(o-c))² - ||o-c||² + r² )
		dir = vecNormalize(dir);
		if(underRoot > 0){
			float dist1, dist2; 
			dist1 = -(vecDot(dir, vecSub(start, sphere[i].pos)))+sqrt(underRoot);
			dist2 = -(vecDot(dir, vecSub(start, sphere[i].pos)))-sqrt(underRoot);//-(//-(l . (o - c))+/-sqrt...
			float dist=1000.0f;
			if(dist1>0.001f)
				dist = dist1;
			if(dist2 > 0.001f)
				if(dist2 < dist)
					dist = dist2;		
			// = (dist1<dist2) ? dist1:dist2;
			
			//*intPoint = vecAdd(start, vecMult(dist, dir));
			tempPoint = vecAdd(start, vecMult(dist, dir));
			
			//check if point is to close! and not inside
			
			if(dist<0.001f || vecDot(vecNormalize(vecSub(tempPoint, start)), dir) < 0.95 || dist == 1000.0f){
				//*intPoint = vecInit2(10000.0f, 0.0f, 0.0f);
				tempPoint = vecInit2(10000.0f, 0.0f, 0.0f);
			}else if(vecNorm(*intPoint) == 10000.0f || vecNorm(vecSub(tempPoint, start)) < vecNorm(vecSub(*intPoint, start))){
				*intColor = sphere[i].col;
				*intPoint = tempPoint;
				*intNormal = vecNormalize(vecSub(tempPoint, sphere[i].pos));
				if(sphere[i].trans == 1)
					*transparent = 1;
				else
					*transparent = 0;
			}
		}else if(underRoot == 0){
			float dist = -(vecDot(dir, vecSub(start, sphere[i].pos)))+sqrt(underRoot);
			//*intColor = sphere[i].col;
			tempPoint = vecAdd(start, vecMult(dist, dir));
			//*intNormal = vecNormalize(vecSub(tempPoint, sphere[i].pos));
			if(dist<0.001f || vecDot(vecNormalize(vecSub(tempPoint, start)), dir) < 0.95){
				tempPoint = vecInit2(10000.0f, 0.0f, 0.0f);
			}else if(vecNorm(*intPoint) == 10000.0f || vecNorm(vecSub(tempPoint, start)) < vecNorm(vecSub(*intPoint, start))){
				*intColor = sphere[i].col;
				*intPoint = tempPoint;
				*intNormal = vecNormalize(vecSub(tempPoint, sphere[i].pos));
				if(sphere[i].trans == 1)
					*transparent = 1;
				else
					*transparent = 0;
			}
		}else{
			tempPoint = vecInit2(10000.0f, 0.0f, 0.0f);//NULL;
		}
	}
	//*intPoint = tempPoint;
	
}	
