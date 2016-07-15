#include "vector.h"
typedef struct{
	vec3 min;
	vec3 max;
	vec3 normal;
	vec3 color;
}Plane;

void initPlane(Plane *plane, vec3 inMin, vec3 inMax, vec3 inNormal, vec3 color);

void planeIntersection(vec3 start, vec3 dir, vec3 *intPoint, vec3 *intColor, vec3 *intNormal, Plane plane[5]);
