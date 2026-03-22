#pragma once

#include "Wall.h"
#include "../Basic/Text.h"
#include "../Engine/Render/Able.h"
#include "../Lib/Types.h"

namespace Pong
{
    class Counter final : public Engine::Render::Able
    {
    public:
        void Compose(Wall* pLeft, Wall* pRight);
        void Compose(Engine::Render::Pipeline* pPipeline) override;
        void Render(float delta) override;

    private:
        void OnCollidedEvent(bool _, int32 n);
        void UpdateDisplay();

        Basic::Text score1_;
        Basic::Text score2_;

        int32 points1_ = 0;
        int32 points2_ = 0;
    };
}
