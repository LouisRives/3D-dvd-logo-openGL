#define GLEW_STATIC
#include "camera.h"
#include "events.h"

#include <iostream>
#include <string>
#include "Animation.h"
#include "objectDraw.h"
#include "shapes.h"
#include "renderer/opengleShader.h"
#include "renderer/opengltexture.h"
#include "collision.h"
#include "object.h"

COLLIDE::sphere Dvd_BS;
COLLIDE::plane4D bottom_4D;
COLLIDE::plane4D Right_4D;
COLLIDE::plane4D Left_4D;
COLLIDE::plane4D Top_4D;


camera Camera;
events dispachter;


void manage_inputs(GLFWwindow* window, float ts)
{
	ts *= 10.5f;

		if (glfwGetKey(window, GLFW_KEY_A))
		{
			Camera.ProcessKeyboard(LEFT, ts);
		}

		else if (glfwGetKey(window, GLFW_KEY_D))
		{
			Camera.ProcessKeyboard(RIGHT, ts);
		}

		else if (glfwGetKey(window, GLFW_KEY_W))
		{
			Camera.ProcessKeyboard(FORWARD, ts);
		}

		else if (glfwGetKey(window, GLFW_KEY_S))
		{
			Camera.ProcessKeyboard(BACKWARD, ts);
		}

		if (dispachter.mouseMoved())
		{
			Camera.ProcessMouseMovement(dispachter.getCusorPosition()[0], dispachter.getCusorPosition()[1], true);
		}

}

// glfw calls this function whenever it needs to draw
void display(GLsizei count)
{
	GLcall(glDrawArrays(GL_TRIANGLES, 0, count));
}

// main function
int main(int argc, char** argv)
{
	//............................................................................
	if (!glfwInit()) {
		std::cout << "initialization failed!" << std::endl;
		return 0;
	}
	//................................................................................
	GLFWwindow* m_window = glfwCreateWindow(1280, 720, "3D Animation", nullptr, nullptr);

	int max, min, r;
	glfwGetVersion(&max, &min, &r);
	//..............................................................................
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, max);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, min);

	std::cout << "version >> " << max << "." << min << std::endl;
	//...............................................................
	glfwMakeContextCurrent(m_window);

	glfwSetErrorCallback((GLFWerrorfun)[](int error_code, const char* description) {
		fprintf(stderr, "Error: %s\n", description);
		});

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//.............................................................
	if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
	{
		std::cout << "error opengl loader not initialized" << std::endl;
	}

	//...........................................................
	//std::cout << glGetString(GL_VERSION) << std::endl;
	dispachter = events(m_window);
	Camera.Position = glm::vec3(18.0f,  10.0f,  20.0f);

	//...........................................................
	glm::mat4 projection = glm::infinitePerspective(glm::radians(90.0f), 16.0f/ 9.0f, 0.1f);
	glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 1.0f, 0.25f)),
		view = glm::mat4(1.0f);
	//.............................................................
	//create shader
	openglShader general_program = openglShader("asset/shaders/vertex.glsl", "asset/shaders/fragment.glsl", "asset/shaders/geometry.glsl", "Shader");
	general_program.Bind();
	
	
	glm::vec3 lightColor = glm::vec3(1.0f);
	float KD = 50.0f;
	float KA = 1.50f;
	float KS = 15.0f;

	glm::vec3 lightPos = glm::vec3(0.0f, 40.0f, 10.0f);

	//set Proection
	general_program.setUniformMat4("projection", projection);
	general_program.Bind();

	float deltaTime, currentFrame, lastFrame = 0;

	GLcall(glDisable(GL_CULL_FACE));
	GLcall(glEnable(GL_DEPTH_TEST));
	GLcall(glDepthFunc(GL_LEQUAL));

	Object Box = Object(vertices, sizeof(vertices), nullptr, 0);

	float unit = 2.5f; //scale / 10.0f
	float unit_z = 0.4f; //scale / 10.0f

	float  Xdistance = 50.0f;
	float  Ydistance = 30.0f;

	glm::vec3 Code_direction = glm::vec3(1.0f, 1.0f, 0.0f);
	glm::vec3 Code_location = Code_direction * Xdistance;

	//sets the Key Frames for Kite animation
	//Animation DvdAimatio = Animation(glm::vec3(0, 0, 0.0f), glm::quat());
	//DvdAimatio.pushBackFrame(Code_location, glm::quat(), 4.5f);

	Anim DvdObject  = Anim(glm::vec3(0.0f));
	DvdObject.setForward(glm::vec3(1.0f, 1.0f, 0.0f));

	glm::vec3 Scale(25.0f, 25.0f, 4.0f);
	int index = 0;

	Dvd_BS.setP1(DvdObject.get_Position(), 5.0f);

	bottom_4D.set(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);

	Right_4D.set(glm::vec3(-1.0f, 0.0f, 0.0f), 50.0f);

	Left_4D.set(glm::vec3(1.0f, 0.0f, 0.0f), 50.0f);

	Top_4D.set(glm::vec3(0.0f, -1.0f, 0.0f), 30.0f);
	
	Timer Collision_timer;

	opengl2Dtexture Dvd_img = opengl2Dtexture("asset/tex.png");
	Dvd_img.bindTextureToSlot(1);

	MeshObject Dvd = MeshObject("asset/dvd-logo.obj", "asset/", "asset/tex.png", general_program);

	while (!glfwWindowShouldClose(m_window))
	{
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE)) break;

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//events
		manage_inputs(m_window, deltaTime);
		Dvd_BS.setP1(DvdObject.get_Position(), 7.0f);


		Dvd_img.bindTextureToSlot(1);


		//sets view
		view = Camera.GetViewMatrix();

		//set light uniforms
		GLcall(index = glGetUniformLocation(general_program.getRendererId(), "position"));
		GLcall(glUniform3fv(index, 1, &lightPos[0]));

		GLcall(index = glGetUniformLocation(general_program.getRendererId(), "lcolor"));
		GLcall(glUniform3fv(index, 1, &lightColor[0]));

		GLcall(index = glGetUniformLocation(general_program.getRendererId(), "ambientIntensity"));
		GLcall(glUniform1f(index, KA));

		GLcall(index = glGetUniformLocation(general_program.getRendererId(), "specularIntensity"));
		GLcall(glUniform1f(index, KS));

		GLcall(index = glGetUniformLocation(general_program.getRendererId(), "intensity"));
		GLcall(glUniform1f(index, KD));

		GLcall(index = glGetUniformLocation(general_program.getRendererId(), "kc"));
		GLcall(glUniform1f(index, 1.0f));
		
		GLcall(index = glGetUniformLocation(general_program.getRendererId(), "kl"));
		GLcall(glUniform1f(index, 0.5f));

		GLcall(index = glGetUniformLocation(general_program.getRendererId(), "kq"));
		GLcall(glUniform1f(index, 0.044f));

		GLcall(index = glGetUniformLocation(general_program.getRendererId(), "viewPosition"));
		GLcall(glUniform3fv(index, 1, &Camera.Position[0]));

		//point_light_2.render(Camera.Position, general_program.getRendererId(), false, false);

		//sets view matrix
		general_program.Bind();
		general_program.setUniformMat4("view", view);
        general_program.setUniformMat4("projection", projection);

		GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLcall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));

		GLcall(glViewport(0, 0, 1280, 720));
		

		//draw 
		{
			general_program.setUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(-50.0f, 0.0, 0.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 60.0f, 2.0f)));
		
			general_program.setUniformVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
			Box.Draw_object(45);
		}

		{
			general_program.setUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 0.0, 0.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 60.0f, 2.0f)));

			general_program.setUniformVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
			Box.Draw_object(45);
		}

		{
			general_program.setUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 30.0, 0.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 2.0f, 2.0f)));

			general_program.setUniformVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
			Box.Draw_object(45);
		}

		{
			general_program.setUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -30.0, 0.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 2.0f, 2.0f)));

			general_program.setUniformVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
			Box.Draw_object(45);
		}

		DvdObject.Move(FOWARD, 0.03f, true);

		//draw 
		{
			//general_program.setUniformMat4("model", DvdObject.returnModel() *
			//	glm::scale(glm::mat4(1.0f), glm::vec3(6.5f, 6.5f, 0.2f)));

			general_program.setTextureSampler("Texture", 1, 1);

			general_program.setUniformVec3("color", glm::vec3(0.0f, 0.0f, 0.0f));
			
			
			Dvd.draw(DvdObject.returnModel()*
				glm::scale(glm::mat4(1.0f), glm::vec3(2.5f, 6.5f, 4.2f)), 3);

			//Box.Draw_object(45);
		}

		if(Collision_timer.elapsed() > 0.2)
		{

			if (COLLIDE::TestInfinitePlane(bottom_4D, Dvd_BS))
			{
				std::cout << "collision Recorded " << std::endl;
				Code_direction = bottom_4D.N;

				Code_direction = (bottom_4D.N * 2.0f) + (DvdObject.GetForward());
				DvdObject.setForward(Code_direction);

				Collision_timer.reset();
			}

			else if (COLLIDE::TestInfinitePlane(Left_4D, Dvd_BS))
			{
				std::cout << "collision Recorded " << std::endl;
				Code_direction = Left_4D.N;

				Code_direction = (Left_4D.N * 2.0f) + (DvdObject.GetForward());
				DvdObject.setForward(Code_direction);

				Collision_timer.reset();
			}

			else if (COLLIDE::TestInfinitePlane(Right_4D, Dvd_BS))
			{
				std::cout << "collision Recorded " << std::endl;
				Code_direction = (Right_4D.N * 2.0f) + (DvdObject.GetForward());
				DvdObject.setForward(Code_direction);

				Collision_timer.reset();
			}


			else if (COLLIDE::TestInfinitePlane(Top_4D, Dvd_BS))
			{
				std::cout << "collision Recorded " << std::endl;
				Code_direction = Top_4D.N;

				Code_direction = (Top_4D.N * 2.0f) + (DvdObject.GetForward());
				DvdObject.setForward(Code_direction);

				Collision_timer.reset();
			}

	
		}
		// swap the buffers
		glfwSwapBuffers(m_window);
		//polls all events recorded
		glfwPollEvents();
	}

	return 0;
}
