#include <stdio.h>
#include <math.h>
#include "vector.h"

vec3 vecInit(){
	vec3 v;	
	v.x = 0.0f;
	v.y = 0.0f;
	v.z = 0.0f;
	return v;
}

vec3 vecInit2(float x, float y, float z){
	vec3 v;	
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

vec3 vecAdd(vec3 v1, vec3 v2){
	return vecInit2(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

vec3 vecSub(vec3 v1, vec3 v2){
	vec3 v;
	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;
	return v;
}

vec3 vecMult(float s, vec3 v){
	return vecInit2(s*v.x, s*v.y, s*v.z);
}

vec3 vecMult2(vec3 v1, vec3 v2){
	return vecInit2(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

float vecDot(vec3 v1, vec3 v2){
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; 
}

float vecNorm(vec3 v){
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

vec3 vecNormalize(vec3 v){
	float norm = vecNorm(v);
	return vecInit2(v.x/norm, v.y/norm, v.z/norm);
}

float vecAngle(vec3 v1, vec3 v2){
	return acos((vecDot(v1, v2)/(vecNorm(v1) * vecNorm(v2))));
}

void vecPrint(vec3 v){
	printf("vec3(%f, %f, %f) \n", v.x, v.y, v.z);
}


vec3 calcReflection(vec3 l, vec3 n){
	return vecNormalize(vecSub( vecMult(2.0f*vecDot(l, n), n) , l));
}



//b = ki/kr if above kr/ki if below surface 
// The -/+ means use minus when ndoti>=0 plus otherwise I vector från punkt till rayens origin.
//http://hugi.scene.org/online/hugi23/torefrac.htm
vec3 calcRefraction(vec3 l, vec3 n, int inside){
//Fel här nånstans när rayen lämnar 
	float ki = 1;
	float kr = 1.5;
	float b;
	if(inside == 0){ //if not inside object
		b = ki/kr;
	}
	else{
		b = kr/ki;
		//n = vecMult(-1.0f, n);
	}
	float nDotl = vecDot(n,l);
	vec3 refract;
	
	refract = vecAdd(vecMult(b,l), vecMult(-b*nDotl - sqrt(1.0f-pow(b,2.0f)*(1.0f-pow(nDotl, 2.0f))), n));

	return vecNormalize(refract);
}
