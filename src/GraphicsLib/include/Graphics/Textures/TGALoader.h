#pragma once

#include "Graphics/GLHeaders.h"
#include "vector"


GLuint LoadTGA(std::vector<uint8_t> *image, int *width, int *height, int *internalFormat, int *format, const char *fileName);

