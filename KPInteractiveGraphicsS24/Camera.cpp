#include "Camera.h"

Camera::Camera() {
	referenceFrame = glm::mat4(1.0f);
	lookFrame = glm::mat4(1.0f);
	//cameraPosition = glm::vec3(15.0f, 15.0f, 20.0f);
	speed = 10.0f;
}

Camera::~Camera() {

}

glm::mat4 Camera::LookForward() {
	// look forward (it returns a view matrix that looks forward). Use the look frame
	// position of the camera is slightly up and some distance away
	//lookFrame[3] = glm::vec4(0.0f, 3.0f, 20.0f, 1.0f);
	glm::vec3 cameraPosition = referenceFrame[3];
	// camera's forward direction is pointing away from viewer, so negate the z-axis
	glm::vec3 cameraForward = -lookFrame[2];
	// position and direction gives you the point where camera is looking
	glm::vec3 cameraTarget = cameraPosition + cameraForward;
	// the direction that the camera considers "up"
	glm::vec3 cameraUp = lookFrame[1];
	// represent the view through the position of the camera in world space, the point 
	// that the camera is looking at, and the vector of the camera, indicating which direction is "up"
	return glm::lookAt(cameraPosition, cameraTarget, cameraUp);
}

glm::mat4 Camera::LookAtTarget(glm::vec3 cameraTarget) {
	glm::vec3 cameraPosition = referenceFrame[3];
	glm::vec3 cameraUp = lookFrame[1];
	return glm::lookAt(cameraPosition, cameraTarget, cameraUp);
}

void Camera::SetPosition(glm::vec3 position) {
	referenceFrame[3].x = position.x;
	referenceFrame[3].y = position.y;
	referenceFrame[3].z = position.z;
}

void Camera::MoveForward(double elapsedSeconds) {
	glm::vec3 forward = -lookFrame[2];
	glm::vec3 position = referenceFrame[3];
	forward = forward * static_cast<float>(speed * elapsedSeconds);
	position = position + forward;
	SetPosition(position);
	return;
}

void Camera::MoveBackward(double elapsedSeconds) {
	glm::vec3 backward = lookFrame[2];
	glm::vec3 position = referenceFrame[3];
	backward = backward * static_cast<float>(speed * elapsedSeconds);
	position = position + backward;
	SetPosition(position);
	return;
}

void Camera::MoveLeft(double elapsedSeconds) {
	glm::vec3 toLeft = -lookFrame[0];
	glm::vec3 position = referenceFrame[3];
	toLeft = toLeft * static_cast<float>(speed * elapsedSeconds);
	position = position + toLeft;
	SetPosition(position);
	return;
}

void Camera::MoveRight(double elapsedSeconds) {
	glm::vec3 toRight = lookFrame[0];
	glm::vec3 position = referenceFrame[3];
	toRight = toRight * static_cast<float>(speed * elapsedSeconds);
	position = position + toRight;
	SetPosition(position);
	return;
}

void Camera::MoveUp(double elapsedSeconds) {
	glm::vec3 toUp = referenceFrame[1];
	glm::vec3 position = referenceFrame[3];
	toUp = toUp * static_cast<float>(speed * elapsedSeconds);
	position = position + toUp;
	SetPosition(position);
	return;
}

void Camera::MoveDown(double elapsedSeconds) {
	glm::vec3 toDown = -referenceFrame[1];
	glm::vec3 position = referenceFrame[3];
	toDown = toDown * static_cast<float>(speed * elapsedSeconds);
	position = position + toDown;
	SetPosition(position);
	return;
}

void Camera::LookLeft(double elapsedSeconds) { 
	glm::vec3 yAxis = lookFrame[1];
	float turnDelta = static_cast<float>(90.0f * elapsedSeconds);
	lookFrame = glm::rotate(lookFrame, glm::radians(turnDelta), yAxis);
	return;
}

void Camera::LookRight(double elapsedSeconds) {
	glm::vec3 yAxis = lookFrame[1];
	float turnDelta = static_cast<float>(-90.0f * elapsedSeconds);
	lookFrame = glm::rotate(lookFrame, glm::radians(turnDelta), yAxis);
	return;
}