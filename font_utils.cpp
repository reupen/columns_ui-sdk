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

namespace {

class common_callback_impl : public common_callback {
public:
    explicit common_callback_impl(std::function<void(uint32_t changed_items_mask)> callback_func)
        : m_callback_func(std::move(callback_func))
    {
    }

    common_callback_impl(common_callback_impl&&) = default;
    common_callback_impl& operator=(common_callback_impl&&) = default;

    void on_font_changed(uint32_t changed_items_mask) const override { m_callback_func(changed_items_mask); }

private:
    std::function<void(uint32_t changed_items_mask)> m_callback_func;
};

} // namespace

std::unique_ptr<callback_token> on_common_font_changed(std::function<void(uint32_t changed_items_mask)> callback_func)
{
    manager::ptr api;

    if (!fb2k::std_api_try_get(api))
        return {};

    auto callback = std::make_shared<common_callback_impl>(std::move(callback_func));

    api->register_common_callback(&*callback);

    auto deregister = [callback{std::move(callback)}, api]() mutable { api->deregister_common_callback(&*callback); };

    return std::make_unique<callback_token_lambda>(std::move(deregister));
}

} // namespace cui::fonts
