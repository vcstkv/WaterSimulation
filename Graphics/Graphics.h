#pragma once

#include "GLHeaders.h"
#include "Model.h"
#include "Texture.h"

		class Graphics
		{
		public:
			GRAPHICS_API Graphics();
			static GRAPHICS_API GLuint LoadShaders(const char *vFilePath, const char *fFilePath);
			static GRAPHICS_API GLuint LoadBitmap(const char *filePath);
			static GRAPHICS_API GLuint LoadShader(const char* path, GLenum type);
		private:
			GLuint VertexArrayID;
		};



