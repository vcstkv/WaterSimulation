#include "Graphics/Text/FontController.h"



FontController::FontController()
{
	fonts = new std::vector<TextFont*>;
}


FontController::~FontController()
{
	delete fonts;
}

uint32_t FontController::AddFont(char *filePath)
{
	for (uint32_t i = 0; i < fonts->size(); i++)
		if (strcmp((*fonts)[i]->GetFilePath(), filePath))
			return i;

	fonts->push_back(new TextFont(filePath));

	return fonts->size()-1;
}
