#ifndef TEXTURE_H
#define TEXTURE_H

#include "Graphics/GLHeaders.h"

// создает текстуру из TGA-файла с изображением
GLuint TextureCreateFromTGA(const char *fileName);

bool LoadFile(const char *fileName, bool binary, uint8_t **buffer, uint32_t *size);

// удаляет текстуру
void TextureDestroy(GLuint texture);

#endif /* TEXTURE_H */
