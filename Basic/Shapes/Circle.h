#pragma once

#include "../Components/Rendering.h"

namespace Basic
{
    namespace Shapes
    {
        class Circle : public Components::Rendering
        {
        public:
            void Compose(const float2& center, float radius, float aspectRatio = 1.0f);
            void Compose(Engine::Render::Pipeline* pPipeline) override;

            // Rebuild and re-upload VB/IB with new geometry (call after Compose(Pipeline*))
            void SetGeometry(const float2& center, float radius, float aspectRatio = 1.0f);

        private:
            static constexpr int SEGMENTS = 16;

            void BuildPoints(const float2& center, float radius, float aspectRatio);

            float4 points_[(SEGMENTS + 1) * 2]{};
            int32  indices_[SEGMENTS * 3]{};
        };
    }
}
