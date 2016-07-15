#include "plane.h"



//Plane plane;

void initPlane(Plane *plane, vec3 inMin, vec3 inMax, vec3 inNormal, vec3 color){
	plane->min = inMin;
	plane->max = inMax;
	plane->normal = inNormal;
	plane->color = color;
}
//d=((p0-l0).n)/(l.n)
void planeIntersection(vec3 start, vec3 dir, vec3 *intPoint, vec3 *intColor, vec3 *intNormal, Plane plane[5]){
	*intPoint = vecInit2(10000.0f,0.0f,0.0f);
	*intColor = vecInit();
	*intNormal = vecInit();
	int i;
	for(i=0; i < 5; i++){
		vec3 tempPoint = vecInit();
		//en punkt mitt i planet
		vec3 p0 = vecInit2(plane[i].min.x,plane[i].min.y,plane[i].min.z);	
		//en punkt på linjen
		vec3 l0 = start;
		vec3 n = plane[i].normal;
		vec3 l = dir;
		float d = vecDot(vecSub(p0,l0),n)/vecDot(l,n);
		tempPoint = vecAdd(start,vecMult(d,dir));
		if(plane[i].normal.z!=0.0f){
			if(tempPoint.x >= plane[i].min.x && tempPoint.x <= plane[i].max.x && tempPoint.y >= plane[i].min.y && tempPoint.y <= plane[i].max.y && vecDot(dir, plane[i].normal) < 0){
				*intPoint = tempPoint;
				*intNormal = plane[i].normal;
				*intColor = plane[i].color;
			}
		}else if(plane[i].normal.y!=0.0f){
			if(tempPoint.x >= plane[i].min.x && tempPoint.x <= plane[i].max.x && tempPoint.z >= plane[i].min.z && tempPoint.z <= plane[i].max.z && vecDot(dir, plane[i].normal) < 0){
				*intPoint = tempPoint;
				*intNormal = plane[i].normal;
				*intColor = plane[i].color;
			}
		}else if(plane[i].normal.x!=0.0f){
			if(tempPoint.y >= plane[i].min.y && tempPoint.y <= plane[i].max.y && tempPoint.z >= plane[i].min.z && tempPoint.z <= plane[i].max.z && vecDot(dir, plane[i].normal) < 0){
				*intPoint = tempPoint;
				*intNormal = plane[i].normal;
				*intColor = plane[i].color;
			}
		}
	}
	//vecPrint(*intPoint);
	
}
