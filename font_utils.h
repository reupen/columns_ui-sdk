#pragma once

namespace cui::fonts {

/**
 * Get a LOGFONT for a particular font client.
 *
 * \param font_client_id GUID of the font client
 * \return populated LOGFONT structure, or empty optional if Columns UI is not installed
 */
[[nodiscard]] std::optional<LOGFONT> get_client_log_font(GUID font_client_id);

/**
 * Get a LOGFONT for a common font.
 *
 * \param common_font_type common font type
 * \return populated LOGFONT structure, or empty optional if Columns UI is not installed
 */
[[nodiscard]] std::optional<LOGFONT> get_common_log_font(font_type_t common_font_type);

/**
 * Get a LOGFONT for a particular font client, falling back to the system icon font
 * if Columns UI is not installed.
 *
 * \param font_client_id GUID of the font client
 * \return populated LOGFONT structure
 */
[[nodiscard]] LOGFONT get_client_log_font_with_fallback(GUID font_client_id);

/**
 * Get a LOGFONT for a common font, falling back to the system icon font if Columns UI
 * is not installed.
 *
 * \param common_font_type common font type
 * \return populated LOGFONT structure
 */
[[nodiscard]] LOGFONT get_common_log_font_with_fallback(font_type_t common_font_type);

/**
 * Get an HFONT for a particular font client, falling back to the system icon font
 * if Columns UI is not installed.
 *
 * \param font_client_id GUID of the font client
 * \return HFONT handle. The caller must delete it using DeleteObject when it’s no longer required.
 */
[[nodiscard]] HFONT create_client_hfont_with_fallback(GUID font_client_id);

/**
 * Get an HFONT for a common font, falling back to the system icon font
 * if Columns UI is not installed.
 *
 * \param common_font_type common font type
 * \return HFONT handle. The caller must delete it using DeleteObject when it’s no longer required.
 */
[[nodiscard]] HFONT create_common_hfont_with_fallback(font_type_t common_font_type);

#if CUI_SDK_DWRITE_ENABLED

/**
 * Get a font::ptr for a particular font. This is mainly intended for DirectWrite usage.
 *
 * \note Requires Columns UI 3.0.0 alpha 1 or newer. You must check if the returned service pointer
 *       is valid.
 *
 * \param font_id font client or common font GUID
 * \return font::ptr for the specified font client. Will be empty if a compatible Columns UI version
 *         is not installed.
 * \throw exception_font_client_not_found thrown if an invalid GUID is supplied
 */
[[nodiscard]] font::ptr get_font(GUID font_id);

#endif

/**
 * Register a function to be called when a common font has changed.
 *
 * This is a simple wrapper around manager::register_common_callback() and
 * manager::deregister_common_callback().
 *
 * \note Generally it's recommended to implement a client object rather than use common fonts, so that
 *       fonts can be customised specifically for your panel. This callback is only useful if you do
 *       not do that and use common fonts instead.
 *
 * @param callback_func function to be called when a common font has changed
 * @return callback token that should be reset when you no longer want the callback function to be called
 */
[[nodiscard]] callback_token::ptr on_common_font_changed(
    std::function<void(uint32_t changed_items_mask)> callback_func);

} // namespace cui::fonts
