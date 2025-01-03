#include "ui_extension.h"

namespace cui::fonts {

std::optional<LOGFONT> get_client_log_font(GUID font_client_id)
{
    if (manager::ptr api; fb2k::std_api_try_get(api)) {
        LOGFONT log_font{};
        api->get_font(font_client_id, log_font);
        return log_font;
    }

    return {};
}

std::optional<LOGFONT> get_common_log_font(font_type_t common_font_type)
{
    if (manager::ptr api; fb2k::std_api_try_get(api)) {
        LOGFONT log_font{};
        api->get_font(common_font_type, log_font);
        return log_font;
    }

    return {};
}

LOGFONT get_client_log_font_with_fallback(GUID font_client_id)
{
    if (const auto log_font = get_client_log_font(font_client_id))
        return *log_font;

    LOGFONT log_font{};
    uGetIconFont(&log_font);
    return log_font;
}

LOGFONT get_common_log_font_with_fallback(font_type_t common_font_type)
{
    if (const auto log_font = get_common_log_font(common_font_type))
        return *log_font;

    LOGFONT log_font{};

    if (common_font_type == font_type_items)
        uGetIconFont(&log_font);
    else
        uGetMenuFont(&log_font);

    return log_font;
}

HFONT create_client_hfont_with_fallback(GUID font_client_id)
{
    const auto log_font = get_client_log_font_with_fallback(font_client_id);
    return CreateFontIndirect(&log_font);
}

HFONT create_common_hfont_with_fallback(font_type_t common_font_type)
{
    const auto log_font = get_common_log_font_with_fallback(common_font_type);
    return CreateFontIndirect(&log_font);
}

#ifndef CUI_SDK_DWRITE_DISABLED
font::ptr get_client_font(GUID font_client_id)
{
    if (manager_v3::ptr api; fb2k::std_api_try_get(api)) {
        return api->get_client_font(font_client_id);
    }

    return {};
}
#endif

} // namespace cui::fonts
