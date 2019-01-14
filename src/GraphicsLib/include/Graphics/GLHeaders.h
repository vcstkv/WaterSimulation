#pragma once
#define GLEW_STATIC
#include "glew\glew.h"
#include <gl\GL.h>
#include "glfw3\glfw3.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm\glm.hpp"
#include "glm\gtx\transform.hpp"

#ifdef GRAPHICS_EXPORTS
#define GRAPHICS_API __declspec(dllexport) 
#else
#define GRAPHICS_API __declspec(dllimport)
#endif
