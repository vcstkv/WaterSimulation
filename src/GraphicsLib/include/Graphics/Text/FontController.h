#pragma once
#include "TextFont.h"

class FontController
{
public:
	FontController();
	~FontController();
	uint32_t AddFont(const char *filePath);
private:
	std::vector<std::shared_ptr<TextFont>> fonts;
};

