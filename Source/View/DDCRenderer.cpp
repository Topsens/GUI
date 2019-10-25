#include "DDCRenderer.h"

bool DDCRenderer::BeginPaint(HDC hdc)
{
    if (!this->CreateRenderTarget(hdc))
    {
        return false;
    }

    this->target->BeginDraw();

    return true;
}

bool DDCRenderer::CreateRenderTarget(HDC hdc)
{
    if (!hdc)
    {
        return false;
    }

    BITMAP bmp = {};
    GetObjectA(GetCurrentObject(hdc, OBJ_BITMAP), sizeof(bmp), &bmp);

    if (bmp.bmWidth <= 0 || bmp.bmHeight <= 0)
    {
        return false;
    }

    if (!this->target)
    {
        auto prop = D2D1::RenderTargetProperties();
        prop.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
        prop.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
        prop.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
        prop.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);

        ID2D1DCRenderTarget* target = nullptr;
        if (SUCCEEDED(d2d1Factory->CreateDCRenderTarget(&prop, &target)))
        {
            RECT rect{ 0, 0, bmp.bmWidth, bmp.bmHeight };
            if (SUCCEEDED(target->BindDC(hdc, &rect)))
            {
                this->target = target;
                return true;
            }

            target->Release();
        }
    }

    return false;
}