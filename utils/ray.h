#ifndef _RAY_H_
#define _RAY_H_
#include "vector.h"

typedef struct Ray{
	vec3 start;
	vec3 dir;
	vec3 col;
	int inside;
	float weight;
	struct Ray *reflect;
	struct Ray *refract;
}Ray;

//typedef struct Ray Rayish;
Ray *initRay(vec3 start, vec3 dir, int in, float weight);

float calcFresnel(float inAngle, float transAngle, float n1, float n2);

void calcColor(Ray r, float weight, vec3 *color);

void deallocate (Ray *r);

vec3 shootRay(vec3 camPos, vec3 dir);

void traceRay(Ray *r, vec3 camPos, int counter);

int shadowTest(vec3 intPoint, vec3 lightPos);

vec3 calcLight(vec3 intPoint, vec3 lightPos, vec3 intNormal, vec3 camPos, Ray ray);

void calcIntersection(Ray *r, vec3 camPos);

#endif
