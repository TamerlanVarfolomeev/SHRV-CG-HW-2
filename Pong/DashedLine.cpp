#include "DashedLine.h"

#include "ShaderData.h"
#include "../Engine/Render/Pipeline.h"

using namespace Pong;

void DashedLine::Compose(int32 dashCount, float dashHalfWidth, float dashHalfHeight)
{
    squares_.resize(dashCount);

    const float top = 1.0f - dashHalfHeight;
    const float step = dashCount > 1 ? 2.0f * top / (dashCount - 1) : 0.0f;

    for (int32 i = 0; i < dashCount; ++i)
    {
        const float y = -top + i * step;
        squares_[i].Compose(float2(0.0f, y), float2(dashHalfWidth, dashHalfHeight));
    }
}

void DashedLine::Compose(Engine::Render::Pipeline* pPipeline)
{
    pPipeline_ = pPipeline;

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    bufferDesc.ByteWidth = sizeof(AdditionData);
    pPipeline_->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &pConstantBuffer_);

    for (auto& square : squares_)
        square.Compose(pPipeline);
}

void DashedLine::Render(float delta)
{
    const AdditionData data{float4(0.0f, 0.0f, 0.0f, 0.0f), float4(0.5f, 0.5f, 0.5f, 1.0f)};

    D3D11_MAPPED_SUBRESOURCE subresource = {};
    pPipeline_->GetDeviceContext()->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
    memcpy(subresource.pData, &data, sizeof(AdditionData));
    pPipeline_->GetDeviceContext()->Unmap(pConstantBuffer_, 0);
    pPipeline_->GetDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer_);

    for (auto& square : squares_)
        square.Render(delta);
}
