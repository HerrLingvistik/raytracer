#ifndef INCLUDED_VECTOR_H
#define INCLUDED_VECTOR_H


typedef struct {
	float x;
	float y;
	float z;		
}vec3;

vec3 vecInit();

vec3 vecInit2(float x, float y, float z);

vec3 vecAdd(vec3 v1, vec3 v2);

vec3 vecSub(vec3 v1, vec3 v2);

vec3 vecMult(float s, vec3 v);

vec3 vecMult2(vec3 v1, vec3 v2);

float vecDot(vec3 v1, vec3 v2);

float vecNorm(vec3 v);

vec3 vecNormalize(vec3 v);

float vecAngle(vec3 v1, vec3 v2);

void vecPrint(vec3 v);

vec3 calcReflection(vec3 l, vec3 n);

vec3 calcRefraction(vec3 l, vec3 n, int inside);

#endif
