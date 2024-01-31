#include "Animation.h"

Anim::Anim()
	:model(glm::mat4(1.0f))
{
}

Anim::Anim(glm::vec3 start_pos)
{
	model = glm::translate(glm::mat4(1.0f), start_pos);
}

Anim::~Anim()
{
}

void Anim::Move(Animation_modes mode, float speed, bool animate)
{
	if (animate)
	{
		glm::mat4 temp = glm::mat4(1.0f);
		switch (mode)
		{
		case FOWARD:
			position += speed * this->forward;
			temp = glm::translate(glm::mat4(1.0f), position);
			model = temp;
			break;

		case BACK:
			position -= speed * this->forward;
			temp = glm::translate(glm::mat4(1.0f), position);
			model = temp;
			break;

		default:
			break;
		}
	}
	else
	{
		return;
	}
}

void Anim::Turn(Animation_modes mode, float speed, bool animate)
{
	if (animate)
	{
		glm::mat4 temp = glm::mat4(1.0f);
		switch (mode)
		{
		case TURN_RIGHT:
			temp = glm::rotate(model, glm::radians(speed * 10.0f), glm::vec3(0, 1, 0));
			forward = glm::transpose(glm::inverse(temp)) * glm::vec4(forward, 1.0f);
			//model = temp;
			break;

		case TURN_LEFT:
			temp = glm::rotate(model, -glm::radians(speed * 10.0f), glm::vec3(0, 1, 0));
			forward = glm::transpose(glm::inverse(temp)) * glm::vec4(forward, 1.0f);
			//model = temp;
			break;

		default:
			break;
		}
	}

	else
	{
		return;
	}
}
