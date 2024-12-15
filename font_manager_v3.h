#pragma once

namespace cui::fonts {

#if NTDDI_VERSION >= 0x0A000004
static_assert(sizeof(DWRITE_FONT_AXIS_VALUE) == 8);
using AxisValue = DWRITE_FONT_AXIS_VALUE;
#else
struct AxisValue {
    uint32_t axis_tag;
    float value;
};
#endif

struct rendering_options {
    DWRITE_RENDERING_MODE rendering_mode{DWRITE_RENDERING_MODE_DEFAULT};
    bool force_greyscale_antialiasing{};
};

enum class FontFamilyModel {
    /**
     * Returns the typographic font family name if known,
     * otherwise the weight-stretch-style family name.
     */
    Automatic = 0,
    /**
     * Requests the weight-stretch-style family name.
     * May return an empty string if not known.
     */
    WeightStretchStyle = 1,
    /**
     * Requests the typographic family name.
     * May return an empty string if not known.
     */
    Typographic = 2,
};

/**
 * Defines a font, as configured in Columns UI Colours and fonts preferences.
 *
 * \note Part of the preliminary DirectWrite-friendly manager_v3 interface.
 *       Subject to change before the final Columns UI 3.0.0 release.
 *
 * The recommended methods to use are create_text_format() and get_rendering_options().
 * However, access to the underlying data is also provided for less common use cases.
 */
class NOVTABLE font : public service_base {
public:
    [[nodiscard]] virtual const wchar_t* family_name(
        FontFamilyModel font_family_model = FontFamilyModel::Automatic) noexcept
        = 0;
    [[nodiscard]] virtual DWRITE_FONT_WEIGHT weight() noexcept = 0;
    [[nodiscard]] virtual DWRITE_FONT_STRETCH stretch() noexcept = 0;
    [[nodiscard]] virtual DWRITE_FONT_STYLE style() noexcept = 0;

    /**
     * Get the font size, in DIP units.
     */
    [[nodiscard]] virtual float size() noexcept = 0;

    /**
     * Get the number of configured font axes. May be zero, in which case
     * the weight-stretch-style values should be used.
     */
    [[nodiscard]] virtual size_t axis_count() const noexcept = 0;

    /**
     * Get an axis value.
     *
     * \param index Axis index. Must be in the range 0 <= index < axis_count().
     * \throws std::out_of_range on invalid index
     */
    [[nodiscard]] virtual AxisValue axis_value(size_t index) const = 0;

    /**
     * Get a GDI-compatible LOGFONT for this font.
     *
     * Note that this is the closest equivalent as GDI does not support
     * modern font features.
     */
    [[nodiscard]] virtual LOGFONT log_font() noexcept = 0;

    /**
     * Create a DirectWrite text format for this font.
     */
    [[nodiscard]] virtual pfc::com_ptr_t<IDWriteTextFormat> create_text_format(
        const wchar_t* locale_name = L"") noexcept
        = 0;

    /**
     * Retrieves the rendering mode configured in Columns UI preferences.
     */
    [[nodiscard]] virtual DWRITE_RENDERING_MODE rendering_mode() noexcept = 0;

    /**
     * Retrieves the value of the 'Force greyscale antialiasing' setting in
     * Columns UI preferences.
     */
    [[nodiscard]] virtual bool force_greyscale_antialiasing() noexcept = 0;

    rendering_options get_rendering_options() { return {rendering_mode(), force_greyscale_antialiasing()}; }

#ifdef __WIL_COM_INCLUDED
    wil::com_ptr_t<IDWriteTextFormat> create_wil_text_format(const wchar_t* locale_name = L"")
    {
        wil::com_ptr_t<IDWriteTextFormat> text_format;
        text_format.attach(create_text_format(locale_name).detach());
        return text_format;
    }
#endif

    FB2K_MAKE_SERVICE_INTERFACE_ENTRYPOINT(font);
};

/**
 * Preliminary DirectWrite-friendly manager_v3 interface.
 *
 * \note Part of the preliminary DirectWrite-friendly manager_v3 interface.
 *       Subject to change before the final Columns UI 3.0.0 release.
 */
class NOVTABLE manager_v3 : public service_base {
public:
    [[nodiscard]] virtual font::ptr get_client_font(GUID id) const = 0;
    virtual void set_client_font_size(GUID id, float size) = 0;

    FB2K_MAKE_SERVICE_INTERFACE_ENTRYPOINT(manager_v3);
};

} // namespace cui::fonts
