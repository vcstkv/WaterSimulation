#pragma once
#include "TextFont.h"

class FontController
{
public:
	FontController();
	~FontController();
	uint32_t AddFont(char *filePath);
private:
	std::vector<TextFont*> *fonts;
};

