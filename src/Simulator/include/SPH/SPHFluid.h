#pragma once

#include <vector>

#include "SPHParticle.h"
#include "SPHSolver.h"
#include "SPHFluidParams.h"
#include "Graphics/Shapes/Sprites/CommonSprite.h"
#include "Graphics/Graphics.h"

typedef unsigned int uint;

class SPHFluid
{
public:
	SPHFluid(SPHFluidParams &params);
	~SPHFluid();
	void Update(double delta);
	void Render(Graphics *graphics, glm::mat4 *projection, glm::mat4 *view);
	void DragParticle(double x, double y, uint particleNum);
	void Init();
private:
	SPHFluidParams params;
	std::vector<SPHParticle> *particles;
};

