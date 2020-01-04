#define GRAPHICS_EXPORTS
#include "Graphics/Text/FontController.h"



FontController::FontController()
{
}


FontController::~FontController()
{
}

uint32_t FontController::AddFont(const char *filePath)
{
	for (uint32_t i = 0; i < fonts.size(); i++)
	{
		if (!fonts[i]->GetFilePath().compare(filePath))
		{
			return i;
		}
	}

	fonts.emplace_back(std::make_shared<TextFont>(filePath));

	return fonts.size() - 1;
}
