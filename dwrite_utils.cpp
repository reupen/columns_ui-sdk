#include "ui_extension.h"

#if CUI_SDK_DWRITE_ENABLED

namespace cui::dwrite_utils {

HRESULT create_custom_rendering_params(IDWriteFactory* factory, HMONITOR monitor, DWRITE_RENDERING_MODE rendering_mode,
    bool force_greyscale_antialiasing, IDWriteRenderingParams** rendering_params)
{
    pfc::com_ptr_t<IDWriteRenderingParams> monitor_rendering_params;
    if (HRESULT hr{};
        FAILED(hr = factory->CreateMonitorRenderingParams(monitor, monitor_rendering_params.receive_ptr())))
        return hr;

    return factory->CreateCustomRenderingParams(monitor_rendering_params->GetGamma(),
        monitor_rendering_params->GetEnhancedContrast(), monitor_rendering_params->GetClearTypeLevel(),
        force_greyscale_antialiasing ? DWRITE_PIXEL_GEOMETRY_FLAT : monitor_rendering_params->GetPixelGeometry(),
        rendering_mode, rendering_params);
}

HMONITOR get_monitor_for_window(HWND wnd)
{
    const auto root_window = GetAncestor(wnd, GA_ROOT);

    if (!root_window)
        return nullptr;

    return MonitorFromWindow(root_window, MONITOR_DEFAULTTONEAREST);
}

} // namespace cui::dwrite_utils

#endif
