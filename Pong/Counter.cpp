#include "Counter.h"

#include <cstdio>

#include "../Engine/Render/Pipeline.h"

using namespace Pong;

// Score display constants (NDC)
static constexpr float PW  = 0.022f;  // pixel width
static constexpr float PH  = 0.038f;  // pixel height
static constexpr float SCY = 0.88f;   // score y position (top of glyph)
static constexpr float GAP = 0.08f;   // horizontal gap from center line

void Counter::Compose(Wall* pLeft, Wall* pRight)
{
    pLeft->CollidedEvent.AddRaw(this, &Counter::OnCollidedEvent, 1);
    pRight->CollidedEvent.AddRaw(this, &Counter::OnCollidedEvent, 2);
}

void Counter::Compose(Engine::Render::Pipeline* pPipeline)
{
    // Player 1 score: right-aligned, right edge at x = -GAP
    float x1 = -GAP - Basic::Text::CalcWidth("0", PW);
    score1_.Compose("0", x1, SCY, PW, PH);
    score1_.Compose(pPipeline);

    // Player 2 score: left-aligned at x = +GAP
    score2_.Compose("0", GAP, SCY, PW, PH);
    score2_.Compose(pPipeline);
}

void Counter::Render(float delta)
{
    score1_.Render(delta);
    score2_.Render(delta);
}

void Counter::OnCollidedEvent(bool _, int32 n)
{
    if (n == 1) points1_++;
    else        points2_++;
    UpdateDisplay();
}

void Counter::UpdateDisplay()
{
    char buf[16];

    snprintf(buf, sizeof(buf), "%d", points1_);
    score1_.SetText(buf, -GAP - Basic::Text::CalcWidth(buf, PW), SCY, PW, PH);

    snprintf(buf, sizeof(buf), "%d", points2_);
    score2_.SetText(buf, GAP, SCY, PW, PH);
}
