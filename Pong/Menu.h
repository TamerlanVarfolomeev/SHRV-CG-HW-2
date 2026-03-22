#pragma once

#include "../Basic/Text.h"
#include "../Engine/Input/Device.h"
#include "../Engine/Render/Able.h"

namespace Pong
{
    class Menu final : public Engine::Render::Able
    {
    public:
        void Compose(Engine::Render::Pipeline* pPipeline) override;
        void Render(float delta) override;

        // Returns -1 while waiting, 0 = single player, 1 = two players
        int Update(Engine::Input::Device* pDevice);

    private:
        Basic::Text title_;
        Basic::Text optionSingle_;
        Basic::Text optionTwo_;

        int  selection_ = 0;
        bool active_    = true;

        bool prevUp_    = false;
        bool prevDown_  = false;
        bool prevEnter_ = false;
    };
}
