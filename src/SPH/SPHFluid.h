#pragma once

#include <vector>

#include "Graphics/Graphics.h"
#include "SPHFluidParams.h"
#include "SPHGPUSolver.h"
#include "SPHParticle.h"

class SPHFluid {
public:
    SPHFluid(SPHFluidParams& params, BoundaryBox& box);
    ~SPHFluid();
    void Update(float delta);
    void Render(const Graphics& graphics, const glm::mat4& projection, const glm::mat4& view);
    void DragParticle(float x, float y, uint32_t particleNum);
    void SetBoundaryBox(BoundaryBox& box);
    void AdjustParams(SPHFluidParams& params);
    void Init();
    void AddParticle(float x, float y, float z);

private:
    SPHFluidParams params{};
    SPHGPUSolver* solver{};
    BoundaryBox boundaryBox{};
    std::vector<SPHParticle>* particles{};
};
