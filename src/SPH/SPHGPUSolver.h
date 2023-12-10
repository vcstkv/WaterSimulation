#pragma once
#include <vector>

#include "Graphics/Shaders/ShaderProgram.h"
#include "SPH/SPHFluidParams.h"
#include "SPH/SPHParticle.h"

class SPHGPUSolver {
public:
    SPHGPUSolver(SPHFluidParams& params, BoundaryBox& box, std::vector<SPHParticle>& particles);
    ~SPHGPUSolver();
    void UpdateParams(SPHFluidParams& params, BoundaryBox& box);
    void Advect(SPHFluidParams& params, std::vector<SPHParticle>& particles);
    void Render(const glm::mat4& projection, const glm::mat4& view);
    void UseBoundary(BoundaryBox& box, std::vector<SPHParticle>& p);
    void SortParticles();

private:
    ShaderProgram accumulateForces;
    ShaderProgram densityAndPressure;
    ShaderProgram indexClear;
    ShaderProgram indexUpdate;
    ShaderProgram leapfrogIntegration;
    ShaderProgram boundaries;
    ShaderProgram render;
    //-----------Compute shaders for gpu radix sort--------
    ShaderProgram origDataToInterData;
    ShaderProgram bitForPrefixScan;
    ShaderProgram parallelPrefixScan;
    ShaderProgram sortInterData;
    ShaderProgram sortOrigData;
    //-----------------------------------------------------
    static constexpr std::string_view renderVertexShaderPath = "sph/ParticleRender.vs";
    static constexpr std::string_view renderGeometryShaderPath = "sph/ParticleRender.gs";
    static constexpr std::string_view renderFragmentShaderPath = "sph/ParticleRender.fs";
    static constexpr std::string_view accumulateForcesPath = "sph/accumulate_forces.cs";
    static constexpr std::string_view densityAndPressurePath = "sph/density_and_pressure.cs";
    static constexpr std::string_view indexClearPath = "sph/index_clear.cs";
    static constexpr std::string_view indexUpdatePath = "sph/index_update.cs";
    static constexpr std::string_view leapfrogIntegrationPath = "sph/leapfrog_integration.cs";
    static constexpr std::string_view boundariesPath = "sph/reflect_boundaries.cs";

    uint32_t particlesCount;
    GLuint particlesBuffer;
    GLuint indexBuffer;
    GLuint vao;
    GLuint prefixSumBuffer;
    GLuint origDataCopyBuffer;
    GLuint interDataBuffer;
    uint32_t prefixSumBufferSize;
};
