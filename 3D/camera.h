#pragma once
#include "Vector.h"

typedef struct
{
	vec3_t position;
	vec3_t direction;
	vec3_t forwardVelocity;
	float yawAngle;
} camera_t;

extern camera_t camera;