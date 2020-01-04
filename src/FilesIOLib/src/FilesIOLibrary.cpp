#define FILESIOLIBRARY_EXPORTS
#include "FilesIO/FilesIOLibrary.h"
#include <iostream>

bool FilesIOLibrary::LoadFile(const char *fileName, uint8_t **buffer, uint32_t *size)
{
	FILE     *input;
	uint32_t fileSize, readed = 0;

	if ((input = fopen(fileName, "rb")) == NULL)
	{
		std::cout << "\nCan\'t open file " << fileName;
		return false;
	}

	fseek(input, 0, SEEK_END);
	fileSize = (uint32_t)ftell(input);
	rewind(input);

	if (fileSize == 0)
	{
		fclose(input);
		std::cout << "\nFile size = 0";
		return false;
	}

	*buffer = new uint8_t[fileSize];

	readed = fread(*buffer, 1, fileSize, input);

	fclose(input);

	if (readed != fileSize)
	{
		std::cout << "\nCan't read all file: \n" << *buffer;
		delete[] *buffer;
		return false;
	}
	
	*size = fileSize;

	return true;
}
