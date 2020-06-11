#include "pongTypes.h"
#include "Actor.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#ifndef CAMERA_H
#define CAMERA_H


// Defines several possible options for camera movement, 
// used as abstraction to stay away 
// from window-system specific input methods


// default camera values
const float YAW = -90.f;
const float PITCH = 0.f;
const float SPEED = 2.5f;
const float SENSITIVITY = .25f;
const float ZOOM = 45.0f;

// An abstraction camera class that processes input 
// and calculates the corresponding euler angles.
// vector matrices for use in OpenGl

class Camera: public Pong::Actor
{
public:
	// camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// constructor with vectors
	Camera(std::string name,
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), 
		   glm::vec3 up = glm::vec3(0.f, 1.f, 0.f),
		   float yaw = YAW,
		   float pitch = PITCH): Front(glm::vec3(0.f, 0.f, -1.f)), 
									MovementSpeed(SPEED),
									MouseSensitivity(SENSITIVITY),
									Zoom(ZOOM),
									Actor(name)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// constructor with scalar values
	Camera(std::string name,
		float posX, float posY, float posZ,
		float upX, float upY, float upZ,
		float yaw = YAW,
		float pitch = PITCH) : Front(glm::vec3(0.f, 0.f, -1.f)),
		MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVITY),
		Zoom(ZOOM),
		Actor(name)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX,upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// vew matrix calculated using euler angles and lookat matrix
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	// keyboard input
	virtual void ProcessKeyboard(Pong::Movements direction, float delta_time)
	{
		float velocity = MovementSpeed * delta_time;
		if (direction == Pong::Movements::FORWARD)
			Position += Front * velocity;
		if (direction == Pong::Movements::BACKWARD)
			Position -= Front * velocity;
		if (direction == Pong::Movements::LEFT)
			Position -= Right * velocity;
		if (direction == Pong::Movements::RIGHT)
			Position += Right * velocity;
		if (direction == Pong::Movements::UP)
			Position += Up * velocity;
		if (direction == Pong::Movements::DOWN)
			Position -= Up * velocity;
	}

	// processes input received from a mouse input system.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constraintPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// evite screen flipp
		if (constraintPitch)
		{
			if (Pitch > 89.9f)
				Pitch = 89.9f;
			else if (Pitch < -89.9f)
				Pitch = -89.9f;
		}
		// update Front, Right and Up vectos
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event.
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		else if (Zoom < 1.0f)
			Zoom = 1.0f;
		else if (Zoom > 45.0f)
			Zoom = 45.0f;
	}

private:
	// calculates the front vector from camera's euler angles
	void updateCameraVectors()
	{
		// calculate new front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

		Front = glm::normalize(front);
		// also recalculate right and up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif // CAMERA_H
