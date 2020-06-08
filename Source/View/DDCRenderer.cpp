#include "DDCRenderer.h"
#include "Cleanup.h"

DDCRenderer DDCRenderer::Create(HDC hdc)
{
    if (!hdc)
    {
        return DDCRenderer();
    }

    BITMAP bmp = {0};
    GetObjectA(GetCurrentObject(hdc, OBJ_BITMAP), sizeof(bmp), &bmp);

    if (bmp.bmWidth <= 0 || bmp.bmHeight <= 0)
    {
        return DDCRenderer();
    }

    if (!AddRefFactories())
    {
        return DDCRenderer();
    }
    ONCLEANUP(factories, []{ ReleaseFactories(); });

    auto prop = D2D1::RenderTargetProperties();
    prop.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    prop.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
    prop.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
    prop.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);

    ID2D1DCRenderTarget* target;
    if (FAILED(DDCRenderer::d2d1Factory->CreateDCRenderTarget(&prop, &target)))
    {
        return DDCRenderer();
    }
    ONCLEANUP(target, [target]{ target->Release(); });

    RECT rect{ 0, 0, bmp.bmWidth, bmp.bmHeight };
    if (FAILED(target->BindDC(hdc, &rect)))
    {
        return DDCRenderer();
    }

    return DDCRenderer(target);
}

DDCRenderer::DDCRenderer() : D2DRenderer()
{
}

DDCRenderer::DDCRenderer(DDCRenderer&& other)
{
    *this = std::move(other);
}

DDCRenderer::DDCRenderer(ID2D1RenderTarget* target) : D2DRenderer(target)
{
}

DDCRenderer& DDCRenderer::operator=(DDCRenderer&& other)
{
    *(D2DRenderer*)this = (D2DRenderer&&)other;
    return *this;
}