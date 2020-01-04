#pragma once
#include <fstream>

#ifdef FILESIOLIBRARY_EXPORTS
#define FIOL_API __declspec(dllexport)
#else
#define FIOL_API __declspec(dllimport)
#endif

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

namespace FilesIOLibrary
{
	template <typename T>
	void WriteBinaryValue(std::ofstream &output, const T&& value)
	{
		output.write((char*)value, sizeof(T));
	}
	template <typename T>
	T ReadBinaryValue(uint8_t **buffer, T *value, uint32_t &offset)
	{
		*value = *(T*)(*buffer) + offset;
		offset += sizeof(T);
	}
	FIOL_API bool LoadFile(const char *fileName, uint8_t **buffer, uint32_t *size);
};

