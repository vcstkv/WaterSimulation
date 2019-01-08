#include "Graphics/Textures/TGALoader.h"
#include "FilesIO/FilesIOLibrary.h"
#include <stdio.h>
#include <vector>

// формат заголовка TGA-файла
#pragma pack(1)
struct TGAHeader
{
	uint8_t  idlength;
	uint8_t  colormap;
	uint8_t  datatype;
	uint8_t  colormapinfo[5];
	uint16_t xorigin;
	uint16_t yorigin;
	uint16_t width;
	uint16_t height;
	uint8_t  bitperpel;
	uint8_t  description;
};
#pragma pack()

GLuint LoadTGA(std::vector<uint8_t> *image, int *width, int *height, int *internalFormat, int *format, const char *fileName)
{
	TGAHeader *header;
	uint8_t   *buffer;
	uint32_t  size;

	// попытаемся загрузить текстуру из файла
	if (!FilesIOLibrary::LoadFile(fileName, &buffer, &size))
		return -1;

	// если размер файла заведомо меньше заголовка TGA
	if (size <= sizeof(TGAHeader))
	{
		printf("Too small file '%s'\n", fileName);
		delete[] buffer;
		return -1;
	}

	header = (TGAHeader*)buffer;
	*width = header->width;
	*height = header->height;

	for (uint32_t i = sizeof(TGAHeader) + header->idlength; i < size; i++)
	{
		image->push_back(buffer[i]);
	}

	// проверим формат TGA-файла - несжатое RGB или RGBA изображение
	if (header->datatype != 2 || (header->bitperpel != 24 && header->bitperpel != 32))
	{
		printf("Wrong TGA format '%s'\n", fileName);
		delete[] buffer;
		return -1;
	}

	// получим формат текстуры
	*format = (header->bitperpel == 24 ? GL_BGR : GL_BGRA);
	*internalFormat = (*format == GL_BGR ? GL_RGB8 : GL_RGBA8);

	delete[] buffer;
	//delete header;
	return 0;
}
