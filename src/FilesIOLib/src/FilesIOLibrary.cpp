#define FILESIOLIBRARY_EXPORTS
#include "FilesIO/FilesIOLibrary.h"

template <typename T>
void FilesIOLibrary::WriteBinaryValue(std::ofstream *output, T value)
{
	for (int i = 0, pow = 1; i < sizeof(T); i++)
	{
		*output << (uint8_t)((value & (0xff * pow)) >> (8 * i));
		pow *= 256;
	}
}
template FIOL_API void FilesIOLibrary::WriteBinaryValue(std::ofstream *output, int value);
template FIOL_API void FilesIOLibrary::WriteBinaryValue(std::ofstream *output, char value);
template FIOL_API void FilesIOLibrary::WriteBinaryValue(std::ofstream *output, short value);
template FIOL_API void FilesIOLibrary::WriteBinaryValue(std::ofstream *output, long value);
template FIOL_API void FilesIOLibrary::WriteBinaryValue(std::ofstream *output, long long value);
template FIOL_API void FilesIOLibrary::WriteBinaryValue(std::ofstream *output, unsigned int value);
template FIOL_API void FilesIOLibrary::WriteBinaryValue(std::ofstream *output, unsigned char value);
template FIOL_API void FilesIOLibrary::WriteBinaryValue(std::ofstream *output, unsigned short value);
template FIOL_API void FilesIOLibrary::WriteBinaryValue(std::ofstream *output, unsigned long value);
template FIOL_API void FilesIOLibrary::WriteBinaryValue(std::ofstream *output, unsigned long long value);

template <typename T>
void FilesIOLibrary::ReadBinaryValue(uint8_t **buffer, T *value, uint32_t *offset)
{
	*value = *(T*)&(*buffer)[*offset];
	*offset += sizeof(T);
	//std::cout << std::endl << +*value;
}
template FIOL_API void FilesIOLibrary::ReadBinaryValue(uint8_t **buffer, int *value, uint32_t *offset);
template FIOL_API void FilesIOLibrary::ReadBinaryValue(uint8_t **buffer, char *value, uint32_t *offset);
template FIOL_API void FilesIOLibrary::ReadBinaryValue(uint8_t **buffer, short *value, uint32_t *offset);
template FIOL_API void FilesIOLibrary::ReadBinaryValue(uint8_t **buffer, long *value, uint32_t *offset);
template FIOL_API void FilesIOLibrary::ReadBinaryValue(uint8_t **buffer, long long *value, uint32_t *offset);
template FIOL_API void FilesIOLibrary::ReadBinaryValue(uint8_t **buffer, float *value, uint32_t *offset);
template FIOL_API void FilesIOLibrary::ReadBinaryValue(uint8_t **buffer, double *value, uint32_t *offset);
template FIOL_API void FilesIOLibrary::ReadBinaryValue(uint8_t **buffer, unsigned int *value, uint32_t *offset);
template FIOL_API void FilesIOLibrary::ReadBinaryValue(uint8_t **buffer, unsigned char *value, uint32_t *offset);
template FIOL_API void FilesIOLibrary::ReadBinaryValue(uint8_t **buffer, unsigned short *value, uint32_t *offset);
template FIOL_API void FilesIOLibrary::ReadBinaryValue(uint8_t **buffer, unsigned long *value, uint32_t *offset);
template FIOL_API void FilesIOLibrary::ReadBinaryValue(uint8_t **buffer, unsigned long long *value, uint32_t *offset);

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
