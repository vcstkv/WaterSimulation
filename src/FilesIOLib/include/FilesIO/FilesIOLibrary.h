#pragma once
#include <iostream>
#include <fstream>

#ifdef FILESIOLIBRARY_EXPORTS
#define FIOL_API __declspec(dllexport)
#else
#define FIOL_API __declspec(dllimport)
#endif

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

class FilesIOLibrary
{
public:
	template <typename T>
	FIOL_API static void WriteBinaryValue(std::ofstream *output, T value);
	template <typename T>
	FIOL_API static void ReadBinaryValue(uint8_t **buffer, T *value, uint32_t *offset);
	FIOL_API static bool LoadFile(const char *fileName, uint8_t **buffer, uint32_t *size);
};

