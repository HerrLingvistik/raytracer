#include "vector.h"

typedef struct{
	vec3 pos;
	float rad;
	vec3 col;
	int trans;
}Sphere;

void initSphere(Sphere *sphere, vec3 p, float r, vec3 c, int trans);

void sphereIntersection(vec3 start, vec3 dir, int inside, vec3 *intPoint, vec3 *intColor, vec3 *intNormal, int *transparent, Sphere sphere[2]);
