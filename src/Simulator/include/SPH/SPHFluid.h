#pragma once

#include <vector>

#include "SPHParticle.h"
#include "SPHGPUSolver.h"
#include "SPHFluidParams.h"
#include "Graphics/Shapes/Sprites/CommonSprite.h"
#include "Graphics/Graphics.h"

typedef unsigned int uint;

class SPHFluid
{
public:
	SPHFluid(SPHFluidParams &params, BoundaryBox &box);
	~SPHFluid();
	void Update(float delta);
	void Render(const std::shared_ptr<const Graphics> graphics, const glm::mat4 &projection, const glm::mat4 &view);
	void DragParticle(float x, float y, uint particleNum);
	void SetBoundaryBox(BoundaryBox &box);
	void AdjustParams(SPHFluidParams& params);
	void Init();
	void AddParticle(float x, float y, float z);
private:
	SPHFluidParams params;
	SPHGPUSolver *solver;
	BoundaryBox boundaryBox;
	std::vector<SPHParticle> *particles;
};

