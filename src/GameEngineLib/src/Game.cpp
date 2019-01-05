#define GAMEENGINE_EXPORTS
#include "Game.h"
#include <math.h>
#include <iostream>

#define PLANES_COUNT 300

using namespace glm;

GLuint shadingProgramID;

mat4 projection;
mat4 view;
vec3 cameraPosition;
vec3 cameraLook;
vec3 cameraZoom;

int screenWidth;
int screenHeight;

bool isStop = false;

Model *tea;

Model *car;

mat4 model;

mat4 carModel;

float velocity = 10.5f;
float currentVelocity = 1;

vec3 lightPos;

GLuint cProgID;

Game::Game(int width, int height)
{
	screenHeight = height;
	screenWidth = width;

	shadingProgramID = Graphics::LoadShaders("Data\\Shaders\\vertex.shader", "Data\\Shaders\\fragment.shader");

    glClearColor(.1f, .67f, 1.f, 1.f);
	//glClearColor(0.f, 0.f, 0.f, 1.f);
	cameraPosition = vec3(0.0f, 1.f, 1.0f);
	cameraLook = vec3(0, 0, 1);
	mat4 rotation = rotate(45.f, vec3(0,0,1));

	projection = perspective(45.0f, width * 1.0f / height, 0.1f, 100.0f);

	model = mat4(1);
	model = translate(model, vec3(0.f,0.f, 5.f));
	//model = scale(model, vec3(0.01));

	carModel = mat4(1);
	carModel = translate(carModel, vec3(0.f, 0.f, 1.f));
	carModel = carModel * rotate(radians(-90.f), vec3(0, 1, 0));
	carModel = scale(carModel, vec3(0.01));

	tea = new Model("Data\\Models\\cop\\m.gsm", shadingProgramID);
	tea->AddTexture(Graphics::LoadBitmap("Data\\Models\\cop\\ambient.tga"));
	tea->AddTexture(Graphics::LoadBitmap("Data\\Models\\cop\\alpha.tga"));

	//car = new Model("Data\\Models\\car69\\m.gsm", shadingProgramID);
	//car->AddTexture(Graphics::LoadBitmap("Data\\Models\\car69\\ambient.tga"));



	GLuint csID = Graphics::LoadShader("Data\\Shaders\\compute.shader", GL_COMPUTE_SHADER);
	
	if (csID == 0)
	{
		return;
	}

	cProgID = glCreateProgram();
	glAttachShader(cProgID, csID);
	glLinkProgram(cProgID);
	glDeleteShader(csID);


}

Game::~Game()
{
	glDeleteProgram(shadingProgramID);
	glDeleteProgram(cProgID);
}

void Game::Update(float delta)
{
	if(!isStop)
		lightPos = cameraPosition - cameraLook;
	currentVelocity = velocity * delta;
	view = lookAt(cameraPosition, cameraPosition + cameraLook, glm::vec3(0.0f, 1.0f, 0.0f));

	glUseProgram(cProgID);

	glUniform1i(glGetUniformLocation(cProgID, "numVertices"), tea->GetVerticesCount());

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tea->GetVerticesBuffer());

	glDispatchCompute(tea->GetVerticesCount()/128, 1, 1);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

	glUseProgram(0);
}

void Game::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, tea->GetVerticesBuffer());
	vec3 *data = (vec3*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 
											tea->GetVerticesCount() * sizeof(vec3), 
											GL_MAP_READ_BIT);
	for (size_t i = 0; i < 1; i++) 
	{
		std::cout << i << " : " << data[i].x << 
						  " : " << data[i].y << 
						  " : " << data[i].z << std::endl;
	}
	std::cout << "----------------------------------------\n";
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);*/

	tea->Draw(&model, &projection, &view, &lightPos);
	//car->Draw(&carModel, &projection, &view, &lightPos);
	//std::cout << glGetError() << std::endl;
}


void Game::OnKeyPress(int buttonID, bool isPressed)
{
	switch (buttonID)
	{
	case GLFW_KEY_SPACE:
		if (!isPressed)
			isStop = !isStop;
		break;
	case GLFW_KEY_W:
		if (!isPressed)
			tea->lightPower++;
		break;
	case GLFW_KEY_A:
		if (!isPressed)
			tea->ambColor += vec3(0.1, 0.1, 0.1);
		break;
	case GLFW_KEY_S:
		if (!isPressed)
			tea->lightPower--;
		break;
	case GLFW_KEY_D:
		if (!isPressed)
			tea->ambColor -= vec3(0.1, 0.1, 0.1);
		break;
	case GLFW_KEY_2:
		if (!isPressed)
			tea->specColor += vec3(0.1, 0.1, 0.1);
		break;
	case GLFW_KEY_1:
		if (!isPressed)
			tea->specColor -= vec3(0.1, 0.1, 0.1);
		break;
	case GLFW_KEY_G:
		if (!isPressed)
		{
			std::cout << std::endl << std::endl << "Spec: " << tea->specColor.x << " " << tea->specColor.y << " " << tea->specColor.z;
			std::cout << std::endl << "Amb: " << tea->ambColor.x << " " << tea->ambColor.y << " " << tea->ambColor.z;
			std::cout << std::endl << "Diff: " << tea->diffColor.x << " " << tea->diffColor.y << " " << tea->diffColor.z;
			std::cout << std::endl << "LightPower: " << tea->lightPower;

			std::cout << std::endl << "c: " << (cameraLook * currentVelocity).x << " " << (cameraLook * currentVelocity).y << " " << (cameraLook * currentVelocity).z;
		}
		break;
	case GLFW_KEY_UP:
		cameraPosition += cameraLook * currentVelocity;
		carModel = translate(cameraLook * currentVelocity) * carModel;
		break;
	case GLFW_KEY_DOWN:
		cameraPosition += cameraLook * -currentVelocity;
		carModel = translate(cameraLook * -currentVelocity) * carModel;
		break;
	case GLFW_KEY_RIGHT:
		cameraLook = vec3((rotate(radians(-0.5f), vec3(0, 1, 0)) * vec4(cameraLook, 1)));
		carModel = carModel * rotate(radians(-0.5f), vec3(0, 1, 0));
		break;
	case GLFW_KEY_LEFT:
		cameraLook = vec3((rotate(radians(0.5f), vec3(0, 1, 0)) * vec4(cameraLook, 1)));
		carModel = carModel * rotate(radians(0.5f), vec3(0, 1, 0));
		break;
	case GLFW_KEY_EQUAL:
		cameraPosition.y += currentVelocity;
		break;
	case GLFW_KEY_MINUS:
		cameraPosition.y += -currentVelocity;
		break;
	}
}

void  Game::OnKeyRelease(int buttonID)
{
	switch (buttonID)
	{
	case GLFW_KEY_W:

		break;
	case GLFW_KEY_A:

		break;
	case GLFW_KEY_S:

		break;
	case GLFW_KEY_D:

		break;
	case GLFW_KEY_G:
		break;
	case GLFW_KEY_UP:
		break;
	case GLFW_KEY_DOWN:
		break;
	case GLFW_KEY_RIGHT:
		break;
	case GLFW_KEY_LEFT:
		break;
	case GLFW_KEY_EQUAL:
		break;
	case GLFW_KEY_MINUS:
		break;
	}
}