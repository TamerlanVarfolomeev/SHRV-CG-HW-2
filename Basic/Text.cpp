#include "Text.h"

#include <d3dcompiler.h>

#include "../Engine/Render/Pipeline.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace Basic;

// ---------------------------------------------------------------------------
// 5x5 pixel font
// Each row is a uint8_t: bit 4 = leftmost column, bit 0 = rightmost column
// ---------------------------------------------------------------------------
struct GlyphDef { char c; uint8_t rows[5]; };

static const GlyphDef GLYPHS[] =
{
    // Special
    {' ', {0x00, 0x00, 0x00, 0x00, 0x00}},

    // A-Z
    {'A', {0x0E, 0x11, 0x1F, 0x11, 0x11}},
    {'B', {0x1E, 0x11, 0x1E, 0x11, 0x1E}},
    {'C', {0x0E, 0x10, 0x10, 0x10, 0x0E}},
    {'D', {0x1E, 0x11, 0x11, 0x11, 0x1E}},
    {'E', {0x1F, 0x10, 0x1E, 0x10, 0x1F}},
    {'F', {0x1F, 0x10, 0x1E, 0x10, 0x10}},
    {'G', {0x0E, 0x10, 0x13, 0x11, 0x0E}},
    {'H', {0x11, 0x11, 0x1F, 0x11, 0x11}},
    {'I', {0x1F, 0x04, 0x04, 0x04, 0x1F}},
    {'J', {0x01, 0x01, 0x01, 0x11, 0x0E}},
    {'K', {0x11, 0x12, 0x1C, 0x12, 0x11}},
    {'L', {0x10, 0x10, 0x10, 0x10, 0x1F}},
    {'M', {0x11, 0x1B, 0x15, 0x11, 0x11}},
    {'N', {0x11, 0x19, 0x15, 0x13, 0x11}},
    {'O', {0x0E, 0x11, 0x11, 0x11, 0x0E}},
    {'P', {0x1E, 0x11, 0x1E, 0x10, 0x10}},
    {'Q', {0x0E, 0x11, 0x11, 0x13, 0x0F}},
    {'R', {0x1E, 0x11, 0x1E, 0x14, 0x13}},
    {'S', {0x0E, 0x10, 0x0E, 0x01, 0x1E}},
    {'T', {0x1F, 0x04, 0x04, 0x04, 0x04}},
    {'U', {0x11, 0x11, 0x11, 0x11, 0x0E}},
    {'V', {0x11, 0x11, 0x11, 0x0A, 0x04}},
    {'W', {0x11, 0x11, 0x15, 0x1B, 0x11}},
    {'X', {0x11, 0x0A, 0x04, 0x0A, 0x11}},
    {'Y', {0x11, 0x0A, 0x04, 0x04, 0x04}},
    {'Z', {0x1F, 0x02, 0x04, 0x08, 0x1F}},

    // 0-9
    {'0', {0x0E, 0x13, 0x15, 0x19, 0x0E}},
    {'1', {0x04, 0x0C, 0x04, 0x04, 0x0E}},
    {'2', {0x0E, 0x01, 0x0E, 0x10, 0x1F}},
    {'3', {0x1E, 0x01, 0x0E, 0x01, 0x1E}},
    {'4', {0x11, 0x11, 0x1F, 0x01, 0x01}},
    {'5', {0x1F, 0x10, 0x1E, 0x01, 0x1E}},
    {'6', {0x0E, 0x10, 0x1E, 0x11, 0x0E}},
    {'7', {0x1F, 0x01, 0x02, 0x04, 0x04}},
    {'8', {0x0E, 0x11, 0x0E, 0x11, 0x0E}},
    {'9', {0x0E, 0x11, 0x0F, 0x01, 0x0E}},
};

static const uint8_t* GetGlyph(char c)
{
    for (const auto& g : GLYPHS)
        if (g.c == c) return g.rows;
    return GLYPHS[0].rows; // fallback to space
}

// ---------------------------------------------------------------------------

void Text::BuildGeometry(const char* text, float x, float y, float pw, float ph)
{
    verts_.clear();
    inds_.clear();

    float cx = x;

    for (int ci = 0; text[ci]; ++ci)
    {
        const char ch = text[ci];
        if (ch == ' ') { cx += 3.0f * pw; continue; }

        const uint8_t* glyph = GetGlyph(ch);

        for (int row = 0; row < 5; ++row)
        {
            for (int col = 0; col < 5; ++col)
            {
                if (!(glyph[row] & (0x10 >> col))) continue;

                const float px = cx + col * pw;
                const float py = y  - row * ph;

                const int32 base = (int32)(verts_.size() / 2);

                // top-right, bottom-left, bottom-right, top-left  (matches Square layout)
                verts_.push_back(float4(px + pw, py,      0.5f, 1.0f)); verts_.push_back(float4(1,1,1,1));
                verts_.push_back(float4(px,      py - ph, 0.5f, 1.0f)); verts_.push_back(float4(1,1,1,1));
                verts_.push_back(float4(px + pw, py - ph, 0.5f, 1.0f)); verts_.push_back(float4(1,1,1,1));
                verts_.push_back(float4(px,      py,      0.5f, 1.0f)); verts_.push_back(float4(1,1,1,1));

                inds_.push_back(base);     inds_.push_back(base + 1); inds_.push_back(base + 2);
                inds_.push_back(base + 1); inds_.push_back(base);     inds_.push_back(base + 3);
            }
        }

        cx += 6.0f * pw; // 5 pixel columns + 1 pixel gap
    }
}

void Text::Compose(const char* text, float x, float y, float pw, float ph)
{
    BuildGeometry(text, x, y, pw, ph);
}

void Text::Compose(Engine::Render::Pipeline* pPipeline)
{
    pPipeline_ = pPipeline;

    if (verts_.empty()) return;

    auto* dev = pPipeline_->GetDevice();

    // Upload vertex buffer
    D3D11_BUFFER_DESC bd = {};
    D3D11_SUBRESOURCE_DATA sd = {};

    bd.Usage     = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.ByteWidth = (UINT)(sizeof(float4) * verts_.size());
    sd.pSysMem   = verts_.data();
    dev->CreateBuffer(&bd, &sd, &pVB_);

    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.ByteWidth = (UINT)(sizeof(int32) * inds_.size());
    sd.pSysMem   = inds_.data();
    dev->CreateBuffer(&bd, &sd, &pIB_);

    indexCount_ = (int32)inds_.size();

    // Free CPU-side geometry data
    verts_.clear();  verts_.shrink_to_fit();
    inds_.clear();   inds_.shrink_to_fit();

    // Compile vertex shader and create input layout
    DXBlob* pCode  = nullptr;
    DXBlob* pError = nullptr;

    D3DCompileFromFile(L"./Shaders/Shader.hlsl", nullptr, nullptr,
        "VSMain", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &pCode, &pError);

    dev->CreateVertexShader(pCode->GetBufferPointer(), pCode->GetBufferSize(), nullptr, &pVS_);

    constexpr D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
         0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
         D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    dev->CreateInputLayout(layoutDesc, 2,
        pCode->GetBufferPointer(), pCode->GetBufferSize(), &pLayout_);
    pCode->Release();

    // Compile pixel shader
    D3DCompileFromFile(L"./Shaders/Shader.hlsl", nullptr, nullptr,
        "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0, &pCode, &pError);
    dev->CreatePixelShader(pCode->GetBufferPointer(), pCode->GetBufferSize(), nullptr, &pPS_);
    pCode->Release();

    // Rasterizer
    CD3D11_RASTERIZER_DESC rsDesc = {};
    rsDesc.CullMode = D3D11_CULL_NONE;
    rsDesc.FillMode = D3D11_FILL_SOLID;
    dev->CreateRasterizerState(&rsDesc, &pRS_);

    // Constant buffer (offset + color, matches Shader.hlsl AdditionBuffer)
    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage          = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDesc.ByteWidth      = sizeof(float4) * 2; // offset + color
    dev->CreateBuffer(&cbDesc, nullptr, &pCB_);
}

void Text::Render(float delta)
{
    if (!pVB_ || indexCount_ == 0) return;

    // Update constant buffer: zero offset, current color
    const float4 cbData[2] = {float4(0.0f, 0.0f, 0.0f, 0.0f), color_};
    D3D11_MAPPED_SUBRESOURCE sr = {};
    auto* ctx = pPipeline_->GetDeviceContext();

    ctx->Map(pCB_, 0, D3D11_MAP_WRITE_DISCARD, 0, &sr);
    memcpy(sr.pData, cbData, sizeof(cbData));
    ctx->Unmap(pCB_, 0);

    constexpr uint32 stride = 32;
    constexpr uint32 offset = 0;

    ctx->VSSetConstantBuffers(0, 1, &pCB_);
    ctx->RSSetState(pRS_);
    ctx->IASetInputLayout(pLayout_);
    ctx->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ctx->IASetIndexBuffer(pIB_, DXGI_FORMAT_R32_UINT, 0);
    ctx->IASetVertexBuffers(0, 1, &pVB_, &stride, &offset);
    ctx->VSSetShader(pVS_, nullptr, 0);
    ctx->PSSetShader(pPS_, nullptr, 0);
    ctx->DrawIndexed(indexCount_, 0, 0);
}

void Text::SetColor(const float4& color)
{
    color_ = color;
}

void Text::SetText(const char* text, float x, float y, float pw, float ph)
{
    BuildGeometry(text, x, y, pw, ph);

    if (pVB_) { pVB_->Release(); pVB_ = nullptr; }
    if (pIB_) { pIB_->Release(); pIB_ = nullptr; }
    indexCount_ = 0;

    if (verts_.empty()) return;

    auto* dev = pPipeline_->GetDevice();

    D3D11_BUFFER_DESC bd = {};
    D3D11_SUBRESOURCE_DATA sd = {};

    bd.Usage     = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.ByteWidth = (UINT)(sizeof(float4) * verts_.size());
    sd.pSysMem   = verts_.data();
    dev->CreateBuffer(&bd, &sd, &pVB_);

    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.ByteWidth = (UINT)(sizeof(int32) * inds_.size());
    sd.pSysMem   = inds_.data();
    dev->CreateBuffer(&bd, &sd, &pIB_);

    indexCount_ = (int32)inds_.size();

    verts_.clear(); verts_.shrink_to_fit();
    inds_.clear();  inds_.shrink_to_fit();
}

float Text::CalcWidth(const char* text, float pw)
{
    float w = 0.0f;
    for (int i = 0; text[i]; ++i)
        w += (text[i] == ' ') ? 3.0f * pw : 6.0f * pw;

    // Trim the trailing inter-character gap from the last non-space character
    if (w > 0.0f)
    {
        int last = 0;
        for (int i = 0; text[i]; ++i) last = i;
        if (text[last] != ' ') w -= pw;
    }
    return w;
}
