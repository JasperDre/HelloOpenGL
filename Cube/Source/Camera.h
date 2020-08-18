#pragma once

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class Camera
{
public:
	Camera();
	~Camera();

public:
	glm::mat4 myProjection;
	glm::mat4 myView;
};

Camera::Camera()
	: myProjection(0.0f)
	, myView(0.0f)
{
}

Camera::~Camera()
{
}