#pragma once
#include "Vector.h"
#include "Matrix.h"

typedef struct
{
	vec3_t position;
	vec3_t direction;
	vec3_t forwardVelocity;
	vec3_t sideStepVelocity;
	float yawAngle;
	float pitch;
} camera_t;

extern camera_t camera;

mat4_t C_GetCameraViewMat();