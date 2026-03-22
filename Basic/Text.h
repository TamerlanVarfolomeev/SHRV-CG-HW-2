#pragma once

#include <vector>

#include "../Engine/Render/Able.h"
#include "../Lib/Types.h"

namespace Basic
{
    class Text final : public Engine::Render::Able
    {
    public:
        // Call first: build geometry from string
        // x, y  – top-left corner in NDC; pw, ph – pixel size in NDC
        void Compose(const char* text, float x, float y, float pw, float ph);

        // Called by Pipeline::Add (or manually): upload geometry, compile shaders
        void Compose(Engine::Render::Pipeline* pPipeline) override;

        void Render(float delta) override;

        void SetColor(const float4& color);

        // Rebuild geometry and re-upload VB/IB (shaders/layout/CB stay intact)
        void SetText(const char* text, float x, float y, float pw, float ph);

        // Total visible width of a string in NDC units
        static float CalcWidth(const char* text, float pw);

    private:
        void BuildGeometry(const char* text, float x, float y, float pw, float ph);

        std::vector<float4> verts_;
        std::vector<int32>  inds_;

        Engine::Render::Pipeline* pPipeline_ = nullptr;
        DXVertexShader*   pVS_     = nullptr;
        DXPixelShader*    pPS_     = nullptr;
        DXInputLayout*    pLayout_ = nullptr;
        DXRasterizerState* pRS_    = nullptr;
        DXBuffer*         pVB_     = nullptr;
        DXBuffer*         pIB_     = nullptr;
        DXBuffer*         pCB_     = nullptr;

        int32  indexCount_ = 0;
        float4 color_      = float4(1.0f, 1.0f, 1.0f, 1.0f);
    };
}
