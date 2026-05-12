#include "handlers/include/render_handler.h"
#include "common/identifiers.h"

RenderHandler::RenderHandler(PaintCallback cb, AccelPaintCallback acb)
    : callback_(cb), accel_callback_(acb) {}

void RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
    rect = CefRect(0, 0, width_, height_);
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
                            const RectList &dirtyRects, const void *buffer,
                            int width, int height)
{
    if (callback_ && buffer)
    {
        callback_(buffer, width, height);
    }
}

void RenderHandler::OnAcceleratedPaint(CefRefPtr<CefBrowser> browser,
                                       CefRenderHandler::PaintElementType type,
                                       const CefRenderHandler::RectList &dirtyRects,
                                       const CefAcceleratedPaintInfo &info)
{
    if (accel_callback_)
    {
        void *shared_handle = nullptr;

#if defined(_WIN32)
        shared_handle = info.shared_texture_handle;
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
        shared_handle = info.shared_texture_io_surface;
#else
        shared_handle = info.shared_texture_io_surface;
#endif
#elif defined(__ANDROID__)
        shared_handle = info.shared_texture_handle;
#elif defined(__linux__)
        shared_handle = reinterpret_cast<void *>(static_cast<intptr_t>(info.planes[0].fd));
#endif

        accel_callback_(shared_handle);
    }
}