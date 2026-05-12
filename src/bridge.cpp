#include "main_client.h"
#include "include/cef_app.h"
#include "common/gpu_mapper.h"
#include "platform_utils.h"
#include <string>
#include <cstdint>

#if defined(_WIN32)
#define DART_EXPORT extern "C" __declspec(dllexport)
#else
#define DART_EXPORT extern "C" __attribute__((visibility("default"))) __attribute__((used))
#endif

CefRefPtr<CefBrowser> g_browser;
CefRefPtr<MainClient> g_client;

DART_EXPORT void initialize(const char *sub_path, const char *res_path)
{
    platform_initialize_cef(
        sub_path ? std::string(sub_path) : "",
        res_path ? std::string(res_path) : "");
}

typedef void (*DartPaintCallback)(const void *buffer, int width, int height);
typedef void (*DartAccelPaintCallback)(void *shared_handle);

DART_EXPORT void create_browser(const char *url,
                                DartPaintCallback cb,
                                DartAccelPaintCallback acb,
                                bool shared_texture_enabled,
                                int fps)
{
    CefWindowInfo window_info;
    window_info.SetAsWindowless(0);
    window_info.shared_texture_enabled = shared_texture_enabled;

    CefBrowserSettings settings;
    settings.windowless_frame_rate = fps;

    auto paint_wrapper = [cb](const void *buffer, int width, int height)
    {
        if (cb)
            cb(buffer, width, height);
    };
    auto accel_wrapper = [acb](void *handle)
    {
        if (acb)
            acb(handle);
    };

    CefRefPtr<RenderHandler> render_handler = new RenderHandler(paint_wrapper, accel_wrapper);
    g_client = new MainClient(render_handler);

    CefBrowserHost::CreateBrowser(window_info, g_client, url ? std::string(url) : "", settings, nullptr, nullptr);
}

DART_EXPORT void load_url(const char *url)
{
    if (url && g_browser && g_browser->GetMainFrame())
    {
        g_browser->GetMainFrame()->LoadURL(CefString(url));
    }
}

DART_EXPORT void resize(int w, int h)
{
    if (g_client && g_browser)
    {
        g_client->GetCustomRenderHandler()->width_ = w;
        g_client->GetCustomRenderHandler()->height_ = h;
        g_browser->GetHost()->WasResized();
        g_browser->GetHost()->Invalidate(PET_VIEW);
    }
}

DART_EXPORT void do_work()
{
    CefDoMessageLoopWork();
}

DART_EXPORT void shutdown_cef()
{
    if (g_browser && g_browser->GetHost())
    {
        g_browser->GetHost()->CloseBrowser(true);
    }
    g_browser = nullptr;
    g_client = nullptr;
    CefShutdown();
}

DART_EXPORT void set_focus(bool focused)
{
    if (!g_browser || !g_browser->GetHost())
        return;
    g_browser->GetHost()->SetFocus(focused);
}

DART_EXPORT void send_mouse_event(int x, int y, int event_type, bool is_up, int button_type)
{
    if (!g_browser || !g_browser->GetHost())
        return;

    CefMouseEvent mouse_event;
    mouse_event.x = x;
    mouse_event.y = y;
    mouse_event.modifiers = 0;

    if (event_type == 0)
    {
        g_browser->GetHost()->SendMouseMoveEvent(mouse_event, false);
    }
    else
    {
        cef_mouse_button_type_t cef_button = static_cast<cef_mouse_button_type_t>(button_type);
        g_browser->GetHost()->SendMouseClickEvent(mouse_event, cef_button, is_up, 1);
    }
}

DART_EXPORT void send_key_event(int key_code, int native_key_code, uint32_t modifiers, int type)
{
    if (!g_browser || !g_browser->GetHost())
        return;

    CefKeyEvent event;
    event.windows_key_code = key_code;
    event.native_key_code = native_key_code;
    event.modifiers = modifiers;

    if (type == 0)
        event.type = KEYEVENT_RAWKEYDOWN;
    else if (type == 1)
        event.type = KEYEVENT_KEYUP;
    else if (type == 2)
        event.type = KEYEVENT_CHAR;

    g_browser->GetHost()->SendKeyEvent(event);
}

DART_EXPORT void send_mouse_wheel(int x, int y, int delta_x, int delta_y)
{
    if (g_browser && g_browser->GetHost())
    {
        CefMouseEvent mouse_event;
        mouse_event.x = x;
        mouse_event.y = y;
        mouse_event.modifiers = 0;

        g_browser->GetHost()->SendMouseWheelEvent(mouse_event, delta_x, delta_y);
    }
}

DART_EXPORT void go_back()
{
    if (g_browser && g_browser->CanGoBack())
        g_browser->GoBack();
}

DART_EXPORT void go_forward()
{
    if (g_browser && g_browser->CanGoForward())
        g_browser->GoForward();
}

DART_EXPORT void reload()
{
    if (g_browser)
        g_browser->Reload();
}