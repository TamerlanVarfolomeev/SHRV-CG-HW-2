#include "Circle.h"

#include <cmath>

#include "../../Engine/Render/Pipeline.h"

using namespace Basic::Shapes;

static constexpr float PI = 3.14159265358979f;

void Circle::BuildPoints(const float2& center, float radius, float aspectRatio)
{
    // Center vertex
    points_[0] = float4(center.x, center.y, 0.5f, 1.0f);
    points_[1] = float4(1, 1, 1, 1);

    // Perimeter vertices — divide X by aspectRatio so circle looks round on screen
    for (int i = 0; i < SEGMENTS; ++i)
    {
        const float angle = i * 2.0f * PI / SEGMENTS;
        points_[(i + 1) * 2]     = float4(center.x + radius * cosf(angle) / aspectRatio,
                                           center.y + radius * sinf(angle),
                                           0.5f, 1.0f);
        points_[(i + 1) * 2 + 1] = float4(1, 1, 1, 1);
    }

    // Triangle fan indices: [0, i+1, (i+1)%SEGMENTS + 1]
    for (int i = 0; i < SEGMENTS; ++i)
    {
        indices_[i * 3]     = 0;
        indices_[i * 3 + 1] = i + 1;
        indices_[i * 3 + 2] = (i + 1) % SEGMENTS + 1;
    }

    pPoints_       = points_;
    pIndices_      = indices_;
    indicesAmount_ = SEGMENTS * 3;
}

void Circle::Compose(const float2& center, float radius, float aspectRatio)
{
    BuildPoints(center, radius, aspectRatio);
}

void Circle::Compose(Engine::Render::Pipeline* pPipeline)
{
    Rendering::Compose(pPipeline);
    CreateVertexShader(L"./Shaders/Shader.hlsl", nullptr, nullptr);
    CreatePixelShader(L"./Shaders/Shader.hlsl", nullptr, nullptr);
    CreateLayout();
    CreateVertexBuffer((SEGMENTS + 1) * 2);
    CreateIndexBuffer(SEGMENTS * 3);
    CreateRasterizerState();
}

void Circle::SetGeometry(const float2& center, float radius, float aspectRatio)
{
    BuildPoints(center, radius, aspectRatio);

    if (pVertexBuffer_) { pVertexBuffer_->Release(); pVertexBuffer_ = nullptr; }
    if (pIndexBuffer_)  { pIndexBuffer_->Release();  pIndexBuffer_  = nullptr; }

    CreateVertexBuffer((SEGMENTS + 1) * 2);
    CreateIndexBuffer(SEGMENTS * 3);
}
