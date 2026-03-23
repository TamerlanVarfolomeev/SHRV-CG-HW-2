#include "Pickup.h"

#include <cstdlib>
#include <ctime>

#include "Ball.h"
#include "Stick.h"
#include "ShaderData.h"
#include "../Engine/Render/Pipeline.h"

using namespace Pong;

void Pickup::Compose(Ball* pBall, Stick* pFriendly, Stick* pEnemy)
{
    pBall_     = pBall;
    pFriendly_ = pFriendly;
    pEnemy_    = pEnemy;
    std::srand((unsigned)std::time(nullptr));
    timer_ = RandomFloat(3.0f, 8.0f);

    // Degenerate box off-screen so nothing intersects while inactive
    boundingBox_.Center  = float3(0, 10, 0);
    boundingBox_.Extents = float3(0, 0, 0);

    circle_.Compose(float2(0, 0), MIN_RADIUS, ASPECT_RATIO);
}

void Pickup::Compose(Engine::Render::Pipeline* pPipeline)
{
    circle_.Compose(pPipeline);

    D3D11_BUFFER_DESC bd{};
    bd.Usage          = D3D11_USAGE_DYNAMIC;
    bd.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.ByteWidth      = sizeof(AdditionData);
    circle_.GetPipeline()->GetDevice()->CreateBuffer(&bd, nullptr, &pCB_);
}

void Pickup::Render(float delta)
{
    if (!active_) return;

    const AdditionData cb{ float4(0, 0, 0, 0), float4(0, 1, 0, 1) };  // green, no offset
    D3D11_MAPPED_SUBRESOURCE sr{};
    auto* ctx = circle_.GetPipeline()->GetDeviceContext();
    ctx->Map(pCB_, 0, D3D11_MAP_WRITE_DISCARD, 0, &sr);
    memcpy(sr.pData, &cb, sizeof(cb));
    ctx->Unmap(pCB_, 0);
    ctx->VSSetConstantBuffers(0, 1, &pCB_);

    circle_.Render(delta);
}

void Pickup::FixedUpdate()
{
    if (active_) return;
    timer_ -= FIXED_DT;
    if (timer_ <= 0.0f)
        Spawn();
}

void Pickup::Collided(CollideAble* pOther)
{
    if (!active_) return;
    if (dynamic_cast<Ball*>(pOther) == nullptr) return;
    ApplyEffect();
    PickedUpEvent.Broadcast();
    Deactivate();
}

void Pickup::Spawn()
{
    const float r = RandomFloat(MIN_RADIUS, MAX_RADIUS);
    const float x = RandomFloat(-1.0f + r, 1.0f - r);
    const float y = RandomFloat(-1.0f + r, 1.0f - r);

    circle_.SetGeometry(float2(x, y), r, ASPECT_RATIO);
    boundingBox_.Center  = float3(x, y, 0);
    boundingBox_.Extents = float3(r, r, 0.1f);
    active_ = true;
}

void Pickup::Deactivate()
{
    active_ = false;
    boundingBox_.Center  = float3(0, 10, 0);
    boundingBox_.Extents = float3(0, 0, 0);
    timer_ = RandomFloat(3.0f, 8.0f);
}

void Pickup::ApplyEffect()
{
    if (rand() % 2 == 0)
    {
        pBall_->ApplySpeedBoost(1.30f, 5.0f);
    }
    else
    {
        pEnemy_->Resize(0.90f);
        pFriendly_->Resize(1.15f);
    }
}

float Pickup::RandomFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}
