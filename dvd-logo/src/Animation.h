#pragma once
#include <iostream>
#include <string>
#include "glm/gtc/matrix_transform.hpp"
#include <chrono>


enum Animation_modes : int
{
	TURN_RIGHT,
	TURN_LEFT,
	FOWARD,
	BACK,
	INVALID_
};

class Timer {
public:
	Timer() : beg_(clock_::now()) {}

	void reset() { beg_ = clock_::now(); }

	double elapsed() const {
		return std::chrono::duration_cast<second_> (clock_::now() - beg_).count();
	}

private:
	typedef std::chrono::high_resolution_clock clock_;
	typedef std::chrono::duration<double, std::ratio<1> > second_;
	std::chrono::time_point<clock_> beg_;
};

//is used in the main class
inline bool turnAnim = true;
inline bool moveAnim = true;

class Anim
{
	glm::mat4 model;
	glm::vec3 position;
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
public:
	Anim();
	Anim(glm::vec3 start_pos);
	~Anim();

	void setForward(glm::vec3 f)
	{
		forward = glm::normalize(f);
	}

	glm::vec3 GetForward()
	{
		return forward;
	}

	void Turn(Animation_modes mode, float speed, bool animate);
	void Move(Animation_modes mode, float speed, bool animate);

	glm::vec3 get_Position() { return position; }

	glm::mat4 returnModel() { return model; }
};

