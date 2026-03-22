#pragma once

#include <vector>

#include "../Basic/Shapes/Square.h"
#include "../Engine/Render/Able.h"
#include "../Lib/Types.h"

namespace Pong
{
    class DashedLine final : public Engine::Render::Able
    {
    public:
        void Compose(int32 dashCount, float dashHalfWidth, float dashHalfHeight);
        void Compose(Engine::Render::Pipeline* pPipeline) override;
        void Render(float delta) override;

    private:
        std::vector<Basic::Shapes::Square> squares_;
        DXBuffer* pConstantBuffer_ = nullptr;
        Engine::Render::Pipeline* pPipeline_ = nullptr;
    };
}
