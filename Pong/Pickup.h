#pragma once

#include "../Basic/Shapes/Circle.h"
#include "../Engine/Physics/CollideAble.h"
#include "../Engine/Render/Able.h"
#include "../Engine/Update/FixedAble.h"
#include "../Lib/Delegates.h"
#include "../Lib/Types.h"

namespace Pong
{
    class Ball;
    class Stick;

    class Pickup final : public Engine::Render::Able,
                         public Engine::Physics::CollideAble,
                         public Engine::Update::FixedAble
    {
    public:
        void Compose(Ball* pBall, Stick* pFriendly, Stick* pEnemy);
        void Compose(Engine::Render::Pipeline* pPipeline) override;
        void Render(float delta) override;

        void FixedUpdate() override;
        void Collided(CollideAble* pOther) override;
        DXBox& GetBoundingBox() override { return boundingBox_; }

        Lib::MulticastDelegate<> PickedUpEvent;

    private:
        void Spawn();
        void Deactivate();
        void ApplyEffect();
        static float RandomFloat(float min, float max);

        Basic::Shapes::Circle circle_;
        DXBox     boundingBox_;
        DXBuffer* pCB_ = nullptr;

        Ball*  pBall_     = nullptr;
        Stick* pFriendly_ = nullptr;
        Stick* pEnemy_    = nullptr;
        bool   active_    = false;
        float  timer_     = 0.0f;

        static constexpr float FIXED_DT    = 0.01f;
        static constexpr float MIN_RADIUS  = 0.05f;
        static constexpr float MAX_RADIUS  = 0.30f;   // 15% of field (height = 2 NDC units)
        static constexpr float ASPECT_RATIO = 1200.0f / 800.0f;  // matches Game::Compose window size
    };
}
