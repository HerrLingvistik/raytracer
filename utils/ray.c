#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "ray.h"
#include "sphere.h"
#include "plane.h"


//KOLLA OM FRESNEL FUNKAR!!!
Sphere sphere[2];
Plane plane[5];
Plane lamp;
float Wt=0.4f, Wr=0.6f; //weight of transmitted and reflected ray
const int SPHEREHIT = 1;
const int PLANEHIT = 0;
const int LAMPHIT = 2;
const int MAX_DEPTH = 8;
Ray *initRay(vec3 start, vec3 dir, int in, float w){
	Ray *r = malloc(sizeof(Ray));//{start, dir, vecInit2(0,0,0), in, malloc(sizeof(Ray)), malloc(sizeof(Ray)};//malloc(sizeof(*r));
	r->start = start;
	r->dir = dir;
	r->col = vecInit2(0.0f,0.0f,0.0f);
	r->inside = in;

	r->reflect=0;
	r->refract=0;
	r->weight = w;
	return r;
}

float calcFresnel(float inAngle, float transAngle, float n1, float n2){
	return pow(fabs((n1*cos(inAngle) - n2*cos(transAngle))/(n1*cos(inAngle) + n2*cos(transAngle))),2);
}

void calcColor(Ray r, float weight, vec3 *color){
	if(r.reflect != 0){
		calcColor(*r.reflect, Wr, color);
	}
	if(r.refract != 0){
		calcColor(*r.refract, Wt, color);
	}
	
	*color =vecAdd(*color, vecMult(0.5f*r.weight, r.col));
}

void deallocate (Ray *r){
		if(r == 0)
			return;
		deallocate(r->reflect);
		r->reflect = 0;
		deallocate(r->refract);
		r->refract = 0;
		free(r);
}
vec3 shootRay(vec3 camPos, vec3 dir){
	initSphere(&sphere[0], vecInit2(-0.4f, -0.3f, 6.0f), 0.3f, vecInit2(1.0f, 1.0f, 1.0f), 1);
	initSphere(&sphere[1], vecInit2(0.65f, -0.65f, 6.65f), 0.3f, vecInit2(1.0f, 1.0f, 1.0f), 0);
	initPlane(&plane[0], vecInit2(-1.0f, -1.0f, 7.0f), vecInit2(1.0f, 1.0f, 7.0f), vecInit2(0.0f,0.0f,-1.0f), vecInit2(1.0f,1.0f,1.0f));
	initPlane(&plane[1], vecInit2(-1.0f, -1.0f, 5.0f), vecInit2(1.0f, -1.0f, 7.0f), vecInit2(0.0f,1.0f,0.0f), vecInit2(1.0f,1.0f,1.0f));
	initPlane(&plane[2], vecInit2(-1.0f, 1.0f, 5.0f), vecInit2(1.0f, 1.0f, 7.0f), vecInit2(0.0f,-1.0f,0.0f), vecInit2(1.0f,1.0f,1.0f));
	initPlane(&plane[3], vecInit2(-1.0f, -1.0f, 5.0f), vecInit2(-1.0f, 1.0f, 7.0f), vecInit2(1.0f,0.0f,0.0f), vecInit2(1.0f,0.0f,0.0f));
	initPlane(&plane[4], vecInit2(1.0f, -1.0f, 5.0f), vecInit2(1.0f, 1.0f, 7.0f), vecInit2(-1.0f,0.0f,0.0f), vecInit2(0.0f,0.0f,1.0f));
	Ray *r = initRay(camPos, dir, 0, 1.0f);
	
	traceRay(r, camPos, 0);
	
	vec3 color = vecInit();
	
	calcColor(*r, 1.0f, &color);
	
	deallocate(r);
	
	return color;
}

void traceRay(Ray *ray, vec3 camPos, int counter){
	calcIntersection(ray, camPos);
	if((ray->reflect) != 0 && counter < MAX_DEPTH){
		traceRay(ray->reflect, camPos, counter+1);					
	}
	if((ray->refract) && counter < MAX_DEPTH){
		traceRay(ray->refract, camPos, counter+1);						
	}
}

int shadowTest(vec3 intPoint, vec3 lightPos){
	int transparent = 0;
	
	vec3 l = vecNormalize(vecSub(lightPos, intPoint));

	vec3 intNormal, intColor, sIntPoint, sIntColor, sIntNormal, pIntPoint, pIntColor, pIntNormal;
	
	sphereIntersection(intPoint, l, 0, &sIntPoint, &sIntColor, &sIntNormal, &transparent, sphere);
	planeIntersection(intPoint, l, &pIntPoint, &pIntColor, &pIntNormal, plane);

	float pLength = vecNorm(vecSub(pIntPoint, intPoint));
	float sLength = vecNorm(vecSub(sIntPoint, intPoint));
	//Väggarna lägger inga skuggor så kolla bara sfärerna!
	vec3 intPoint2 = sIntPoint;
	intNormal = sIntNormal;
		 
	return (intPoint2.x != 10000.0f && transparent != 1 ) ? 1 : 0;
}

vec3 calcLight(vec3 intPoint, vec3 lightPos, vec3 intNormal, vec3 camPos, Ray ray){
	vec3 r, v, n, l, i_a = vecInit2(1.0f,1.0f,1.0f), i_s = vecInit2(1.0f,1.0f,1.0f), i_d = vecInit2(1.0f,1.0f,1.0f);
	float k_a=0.0f, k_d=0.8f, k_s=1.0f, alpha = 50.0f;

	v = vecNormalize(vecSub(camPos, intPoint));
	n = intNormal;

	l = vecNormalize(vecSub(lightPos, intPoint));
	if(vecDot(ray.dir, n) > 0)
		n = vecMult(-1.0f, n);

	r = calcReflection(l,n);
	vec3 amb = vecMult(k_a, i_a);
	vec3 diff = vecMult(fmax(fmin(k_d*vecDot(l,n), 1.0f),0.0f), i_d);
	vec3 spec = vecMult(fmax(fmin(pow(k_s * vecDot(r, v), alpha),1.0f), 0.0f), i_s);

	return vecAdd(amb, vecAdd(diff, spec));
} 

void calcIntersection(Ray *ray, vec3 camPos){
	int hit = -1;
	int transparent;
	vec3 intPoint, intNormal, intColor, sIntPoint, sIntColor, sIntNormal, pIntPoint, pIntColor, pIntNormal;
	sphereIntersection(ray->start, ray->dir, ray->inside, &sIntPoint, &sIntColor, &sIntNormal, &transparent, sphere);
	planeIntersection(ray->start, ray->dir,&pIntPoint, &pIntColor, &pIntNormal, plane);
	
	float pLength = vecNorm(vecSub(pIntPoint, ray->start));
	float sLength = vecNorm(vecSub(sIntPoint, ray->start));
	
	intPoint = (sLength<pLength) ? sIntPoint : pIntPoint;
	intNormal = (sLength<pLength) ? sIntNormal : pIntNormal;
	intColor = (sLength<pLength) ? sIntColor : pIntColor;
	hit = (sLength<pLength) ? SPHEREHIT : PLANEHIT;
	
	if(hit == PLANEHIT && (intNormal.y == -1.0f || intNormal.x == 1.0f)){
		if(intPoint.x >= -0.20f && intPoint.x <= 0.20f && intPoint.z >= 5.80f && intPoint.z <= 6.20f){	
			hit = LAMPHIT;
		}else if(intPoint.y >= -0.20f && intPoint.y <= 0.20f && intPoint.z >= 5.80f && intPoint.z <= 6.20f){
			hit = LAMPHIT;
		}
	}	
	if(intPoint.x != 10000.0f){	
		if(hit != LAMPHIT){			
			
			vec3 n = intNormal;
			if(vecDot(ray->dir, n) > 0)
				n = vecMult(-1.0f, n);

			vec3 light;
			if(shadowTest(intPoint, vecInit2(0.0f, 0.9f, 6.0f))!= 1)
			 light = vecMult(0.5f,calcLight(intPoint, vecInit2(0.0f, 0.9f, 6.0f), intNormal, camPos, *ray));
			else
				light = vecInit();
			vec3 light2;
			if(shadowTest(intPoint, vecInit2(-0.9f, 0.0f, 6.0f))!= 1)
				light2 = vecMult(0.5f,calcLight(intPoint, vecInit2(-0.9f, 0.0f, 6.0f), intNormal, camPos, *ray));
			else
				light2 = vecInit();
			ray->col = vecMult2(intColor, vecAdd(light, light2));
			vec3 reflectedRay;
			if(hit == SPHEREHIT){	
				//fresnel shit
				float R,T,inAngle,transAngle, n1, n2;
	
	
				reflectedRay = calcReflection(vecMult(-1.0f,ray->dir), intNormal);
				//if(transparent == 1){
				vec3 refractedRay = calcRefraction(ray->dir,n, ray->inside);
				//}
			
				if(ray->inside == 0){ //if not inside object
					n1=1;
					n2=1.5;
				}
				else{
					n1=1.5;
					n2=1;
				}
				//fresnel
				inAngle = vecAngle(vecMult(-1.0f,ray->dir),n);
				transAngle = vecAngle(refractedRay,vecMult(-1.0f,n));
	
				R = calcFresnel(inAngle, transAngle, n1, n2);
				T = 1.0f-R;
			
				if(transparent == 1){		
					ray->reflect = malloc(sizeof(Ray));			
					ray->reflect = initRay(intPoint, reflectedRay, ray->inside, ray->weight*R);
					ray->refract = malloc(sizeof(Ray));
					ray->refract = initRay(intPoint, refractedRay, (ray->inside == 1) ? 0 : 1, ray->weight*T);
				}else{
					ray->reflect = malloc(sizeof(Ray));			
					ray->reflect = initRay(intPoint, reflectedRay, ray->inside, ray->weight*0.8f);
					ray->refract = 0;
				}
			}
		}else{
			//Färgen på lamporna ska bli stark kompensera för falloff.
			ray->col = vecInit2(4.0f,4.0f,4.0f);
			ray->reflect=0;
			ray->refract=0;
		}
	}
	else{
		ray->reflect=0;
		ray->refract=0;
		ray->col =  vecInit2(0.0f,0.0f,0.0f);	
	}
	
}
