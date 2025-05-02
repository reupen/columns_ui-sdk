#pragma once

namespace cui::dwrite_utils {
/**
 * Get the HMONITOR to use with `create_custom_rendering_params()` for an HWND.
 *
 * @param wnd The HWND that is rendering text using DirectWrite
 * @return HMONITOR of the root window associated with the HWND, or `nullptr` on failure
 */
HMONITOR get_monitor_for_window(HWND wnd);

/**
 * Create DirectWrite rendering params for a monitor and specific rendering options.
 *
 * @param factory Pre-created DirectWrite factory
 * @param monitor Monitor handle returned by `get_monitor_for_window()`
 * @param rendering_mode Rendering mode returned by `cui::fonts::rendering_options::rendering_mode()`
 * @param force_greyscale_antialiasing Flag returned by `cui::fonts::rendering_options::force_greyscale_antialiasing()`
 * @param rendering_params Receives a pointer to the created IDWriteRenderingParams on success
 * @return HRESULT indicating success or failure code
 */
HRESULT create_custom_rendering_params(IDWriteFactory* factory, HMONITOR monitor, DWRITE_RENDERING_MODE rendering_mode,
    bool force_greyscale_antialiasing, IDWriteRenderingParams** rendering_params);

} // namespace cui::dwrite_utils
