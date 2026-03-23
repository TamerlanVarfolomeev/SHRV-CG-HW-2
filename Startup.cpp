#include "Game.h"
#include "Pong/Ball.h"
#include "Pong/Counter.h"
#include "Pong/DashedLine.h"
#include "Pong/Menu.h"
#include "Pong/Pickup.h"
#include "Pong/Stick.h"
#include "Pong/StickAI.h"
#include "Pong/StickPlayer.h"
#include "Pong/Wall.h"

int main()
{
    auto* game = Game::Instance();
    game->Compose(L"Pong", 0.01f);
    auto* menu = new Pong::Menu();
    game->GetRenderPipeline()->Add(menu);

    int mode = -1;
    while (mode == -1)
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT) return 0;

        mode = menu->Update(game->GetInputDevice());
        game->GetRenderPipeline()->Render(0.0f);
    }

    auto* ball = new Pong::Ball();
    ball->Compose(float2(0, 0), float2(0.025f * 2 / 3, 0.025f), 1, 0.02f);
    auto* stick1 = new Pong::StickPlayer();
    stick1->Compose(float2(-0.9f, 0), float2(0.01f, 0.2f), Pong::Side::Left, 1, game->GetInputDevice());

    Pong::Stick* stick2 = nullptr;
    if (mode == 1)
    {
        auto* p = new Pong::StickPlayer();
        p->Compose(float2(0.9f, 0), float2(0.01f, 0.2f), Pong::Side::Right, 1, game->GetInputDevice());
        stick2 = p;
    }
    else
    {
        auto* ai = new Pong::StickAI();
        ai->Compose(float2(0.9f, 0), float2(0.01f, 0.2f), Pong::Side::Right, 1, ball);
        stick2 = ai;
    }

    auto* wallUp = new Pong::Wall();
    wallUp->Compose(float2(0, 2), float2(2, 1));
    auto* wallDown = new Pong::Wall();
    wallDown->Compose(float2(0, -2), float2(2, 1));
    auto* wallLeft = new Pong::Wall();
    wallLeft->Compose(float2(-2.1f, 0), float2(1, 2));
    auto* wallRight = new Pong::Wall();
    wallRight->Compose(float2(2.1f, 0), float2(1, 2));
    auto* counter = new Pong::Counter();
    counter->Compose(wallLeft, wallRight);

    auto* dashedLine = new Pong::DashedLine();
    dashedLine->Compose(13, 0.01f, 0.05f);

    auto* pickup = new Pong::Pickup();
    pickup->Compose(ball, stick1, stick2);

    game->GetRenderPipeline()->Add(dashedLine);
    game->GetRenderPipeline()->Add(counter);
    game->GetRenderPipeline()->Add(stick1);
    game->GetRenderPipeline()->Add(stick2);
    game->GetRenderPipeline()->Add(ball);
    game->GetRenderPipeline()->Add(pickup);
    game->GetFixedUpdate()->Add(stick1);
    game->GetFixedUpdate()->Add(stick2);
    game->GetFixedUpdate()->Add(ball);
    game->GetFixedUpdate()->Add(pickup);
    game->GetPhysicsCollide()->Add(stick1);
    game->GetPhysicsCollide()->Add(stick2);
    game->GetPhysicsCollide()->Add(ball);
    game->GetPhysicsCollide()->Add(wallUp);
    game->GetPhysicsCollide()->Add(wallDown);
    game->GetPhysicsCollide()->Add(wallLeft);
    game->GetPhysicsCollide()->Add(wallRight);
    game->GetPhysicsCollide()->Add(pickup);
    game->GetPhysicsMove()->Add(stick1);
    game->GetPhysicsMove()->Add(stick2);
    game->GetPhysicsMove()->Add(ball);
    game->Run();
}
