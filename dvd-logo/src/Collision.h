#pragma once
#include "glm/glm.hpp"

namespace COLLIDE
{
	struct restrictions
	{
		bool Forward_movement_allowed = true;
		bool Backward_movement_allowed = true;
		bool Left_movement_allowed = true;
		bool Right_movement_allowed = true;
	};

	struct plane4D
	{
		glm::vec3 N;
		float Distance;

		void set(glm::vec3 N, float R)
		{
			this->N = N;
			Distance = R;
		}

		void update(glm::mat3 Rotate, float Translate)
		{
			N = Rotate * N;
			Distance -= Translate;
		}
	};

	struct sphere
	{
		glm::vec3 P1;
		float Radius;

		void setP1(glm::vec3 g, float R)
		{
			P1 = g;
			Radius = R;
		}

		void update(glm::mat4 translation)
		{
			glm::vec4 result = translation * glm::vec4(P1, 1.0f);
			P1 = result / result.w;
		}
	};


	inline bool TestInfinitePlane(plane4D Plane, sphere Sphere_3D)
	{
		float Dot = glm::dot(Plane.N, Sphere_3D.P1);
		float final_plane_Dist = Plane.Distance - Sphere_3D.Radius;

		bool result = ( ( Dot + final_plane_Dist <= 0.0f ) ? true : false );

		return result;
	}
}
