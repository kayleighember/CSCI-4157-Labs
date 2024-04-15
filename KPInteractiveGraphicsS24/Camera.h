#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
	glm::mat4 lookFrame;
	glm::mat4 referenceFrame;
	//glm::vec3 cameraPosition;
	float speed;	

public:
	Camera();
	~Camera();
	inline glm::mat4 GetLookFrame() { return lookFrame; }
	inline glm::mat4 GetReferenceFrame() { return referenceFrame; }
	inline float GetSpeed() { return speed; }

	inline void SetLookFrame(glm::mat4 lookFrame) { this->lookFrame = lookFrame; }
	void SetPosition(glm::vec3 position);
	inline void SetSpeed(float speed) { this->speed = speed; }

	glm::mat4 LookForward();
	glm::mat4 LookAtTarget(glm::vec3 cameraTarget);
	void MoveForward(double elapsedSeconds);
	void MoveBackward(double elapsedSeconds);
	void MoveLeft(double elapsedSeconds);
	void MoveRight(double elapsedSeconds);
	void MoveUp(double elapsedSeconds);
	void MoveDown(double elapsedSeconds);
	void LookLeft(double elapsedSeconds);
	void LookRight(double elapsedSeconds);
};

