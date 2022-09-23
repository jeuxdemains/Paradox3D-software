#include "camera.h"

camera_t camera =
{
	.position = {0, 0, 0},
	.direction = {0, 0, 1},
	.forwardVelocity = {0,0,0},
	.sideStepVelocity = {0,0,0},
	.yawAngle = 0.0f,
	.pitch = 0.0f
};

mat4_t C_GetCameraViewMat()
{
	vec3_t target = { 0,0,1 };
	vec3_t up_dir = { 0,1,0 };

	mat4_t cameraPitchRotMat = Mat4_MakeRotationX(camera.pitch);
	mat4_t cameraYawRotMat = Mat4_MakeRotationY(camera.yawAngle);
	mat4_t cameraRot = Mat4_MakeIdentity();
	cameraRot = Mat4_MulMat4(cameraPitchRotMat, cameraRot);
	cameraRot = Mat4_MulMat4(cameraYawRotMat, cameraRot);

	camera.direction = M_Vec3FromVec4(Mat4_MulVec4(cameraRot, M_Vec4FromVec3(target)));
	target = M_AddVec3(camera.position, camera.direction);
	mat4_t viewMat = Mat4_LookAt(camera.position, target, up_dir);

	return viewMat;
}