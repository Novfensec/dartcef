#pragma once
#include "include/cef_render_handler.h"
#include <functional>

class RenderHandler : public CefRenderHandler
{
public:
    int width_ = 800;
    int height_ = 600;

    using PaintCallback = std::function<void(const void *, int, int)>;
    using AccelPaintCallback = std::function<void(void *)>;

    RenderHandler(PaintCallback cb, AccelPaintCallback acb);

    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;

    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
                 const RectList &dirtyRects, const void *buffer,
                 int width, int height) override;

    void OnAcceleratedPaint(CefRefPtr<CefBrowser> browser,
                            CefRenderHandler::PaintElementType type,
                            const CefRenderHandler::RectList &dirtyRects,
                            const CefAcceleratedPaintInfo &info) override;

    IMPLEMENT_REFCOUNTING(RenderHandler);

private:
    PaintCallback callback_;
    AccelPaintCallback accel_callback_;
};