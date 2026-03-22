#include "Menu.h"

#include "../Engine/Input/Keyboard.h"
#include "../Engine/Render/Pipeline.h"

using namespace Pong;
using namespace Engine::Input;

void Menu::Compose(Engine::Render::Pipeline* pPipeline)
{
    // Title "PONG" – larger pixels, centered
    constexpr float tpw = 0.028f, tph = 0.048f;
    title_.Compose("PONG", -0.336f, 0.65f, tpw, tph);
    title_.Compose(pPipeline);

    // Menu options – smaller pixels
    constexpr float pw = 0.012f, ph = 0.022f;
    optionSingle_.Compose("SINGLE PLAYER", -0.450f,  0.15f, pw, ph);
    optionSingle_.Compose(pPipeline);

    optionTwo_.Compose("TWO PLAYERS", -0.378f, -0.10f, pw, ph);
    optionTwo_.Compose(pPipeline);
}

void Menu::Render(float delta)
{
    if (!active_) return;

    constexpr float4 white{1.00f, 1.00f, 1.00f, 1.0f};
    constexpr float4 gray {0.35f, 0.35f, 0.35f, 1.0f};
    constexpr float4 gold {0.95f, 0.75f, 0.20f, 1.0f};

    title_.SetColor(gold);
    title_.Render(delta);

    optionSingle_.SetColor(selection_ == 0 ? white : gray);
    optionSingle_.Render(delta);

    optionTwo_.SetColor(selection_ == 1 ? white : gray);
    optionTwo_.Render(delta);
}

int Menu::Update(Engine::Input::Device* pDevice)
{
    if (!active_) return -1;

    const bool upNow    = pDevice->IsKeyDown(Keyboard::Keys::W)
                       || pDevice->IsKeyDown(Keyboard::Keys::UpArrow);
    const bool downNow  = pDevice->IsKeyDown(Keyboard::Keys::S)
                       || pDevice->IsKeyDown(Keyboard::Keys::DownArrow);
    const bool enterNow = pDevice->IsKeyDown(Keyboard::Keys::Enter);

    if (upNow   && !prevUp_   && selection_ > 0) --selection_;
    if (downNow && !prevDown_ && selection_ < 1) ++selection_;

    prevUp_   = upNow;
    prevDown_ = downNow;

    if (enterNow && !prevEnter_)
    {
        active_ = false;
        return selection_;
    }
    prevEnter_ = enterNow;

    return -1;
}
