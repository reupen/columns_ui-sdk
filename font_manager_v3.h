#pragma once

namespace cui::fonts {

PFC_DECLARE_EXCEPTION(exception_font_client_not_found, pfc::exception, "Font client not found");

#if NTDDI_VERSION >= 0x0A000004
static_assert(sizeof(DWRITE_FONT_AXIS_VALUE) == 8);
using AxisValue = DWRITE_FONT_AXIS_VALUE;
#else
struct AxisValue {
    uint32_t axis_tag;
    float value;
};
#endif

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
 * Defines text rendering options, as configured in Columns UI Colours and fonts preferences.
 *
 * \note Part of the preliminary DirectWrite-friendly manager_v3 interface.
 *       Subject to change before the final Columns UI 3.0.0 release.
 */
class NOVTABLE rendering_options : public service_base {
public:
    /**
     * Retrieves the rendering mode configured in Columns UI preferences.
     */
    [[nodiscard]] virtual DWRITE_RENDERING_MODE rendering_mode() noexcept = 0;

    /**
     * Retrieves the value of the 'Force greyscale anti-aliasing' setting in
     * Columns UI preferences.
     *
     * If enabled, `DWRITE_PIXEL_GEOMETRY_FLAT` and/or `D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE`
     * should be used.
     */
    [[nodiscard]] virtual bool force_greyscale_antialiasing() noexcept = 0;

    /**
     * Retrieves the value of the 'Use colour glyphs when available' setting in
     * Columns UI preferences.
     *
     * For `ID2D1RenderTarget::DrawTextLayout()` and similar methods, this refers to the
     * `D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT` flag.
     */
    [[nodiscard]] virtual bool use_colour_glyphs() noexcept = 0;

    FB2K_MAKE_SERVICE_INTERFACE(rendering_options, service_base)
};

/**
 * Defines a font, as configured in Columns UI Colours and fonts preferences.
 *
 * \note Part of the preliminary DirectWrite-friendly manager_v3 interface.
 *       Subject to change before the final Columns UI 3.0.0 release.
 *
 * The recommended methods to use are create_text_format() and rendering_options().
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
     * Note that the LOGFONT returned is the closest equivalent as GDI does
     * not support modern font features.
     */
    [[nodiscard]] virtual LOGFONT log_font() noexcept = 0;

    /**
     * Get a GDI-compatible LOGFONT for this font and a specific DPI.
     *
     * Note that the LOGFONT returned is the closest equivalent as GDI does
     * not support modern font features.
     */
    [[nodiscard]] virtual LOGFONT log_font_for_dpi(unsigned dpi) noexcept = 0;

    /**
     * Returns a font fallback object that should be applied to the text
     * format using `IDWriteTextFormat1::SetFontFallback()`.
     *
     * \note You do not need to use this method if the text format was created
     * using `create_text_format()`.
     *
     * Do not call `IDWriteTextFormat1::SetFontFallback()` if the returned
     * object pointer is not valid (i.e. the `is_valid()` method returns false).
     *
     * The custom font fallback object is used for the ‘Use alternative emoji font
     * selection logic’ option in Columns UI text rendering options.
     */
    [[nodiscard]] virtual pfc::com_ptr_t<
#ifdef DWRITE_2_H_INCLUDED
        IDWriteFontFallback
#else
        IUnknown
#endif
        >
    font_fallback() noexcept = 0;

    /**
     * Create a DirectWrite text format for this font.
     *
     * The font fallback object for the font will be automatically configured if needed.
     *
     * No further customisation of the text format is performed. You should configure text
     * alignment, paragraph alignment, word wrapping and other options according to your needs.
     */
    [[nodiscard]] virtual pfc::com_ptr_t<IDWriteTextFormat> create_text_format(
        const wchar_t* locale_name = L"") noexcept
        = 0;

    /**
     * Retrieves text rendering options.
     *
     * These options should be used when rendering text using the created text format.
     */
    [[nodiscard]] virtual rendering_options::ptr rendering_options() noexcept = 0;

#ifdef __WIL_COM_INCLUDED
    wil::com_ptr_t<IDWriteTextFormat> create_wil_text_format(const wchar_t* locale_name = L"")
    {
        wil::com_ptr_t<IDWriteTextFormat> text_format;
        text_format.attach(create_text_format(locale_name).detach());
        return text_format;
    }
#endif

    FB2K_MAKE_SERVICE_INTERFACE(font, service_base)
};

/**
 * Preliminary DirectWrite-friendly manager_v3 interface.
 *
 * \note Part of the preliminary DirectWrite-friendly manager_v3 interface.
 *       Subject to change before the final Columns UI 3.0.0 release.
 */
class NOVTABLE manager_v3 : public service_base {
public:
    /**
     * Get a font definition for a particular font client or a common font.
     *
     * \param id font client or common font GUID
     * \return service pointer for a font instance.
     * \throw exception_font_client_not_found thrown if an invalid GUID is supplied
     */
    [[nodiscard]] virtual font::ptr get_font(GUID id) const = 0;

    /**
     * Retrieves the default text rendering options.
     *
     * \remark ``font::rendering_options()`` should be used in preference to this method. This method is only provided
     * to allow access to the configured rendering options when the font being used is not provided by Columns UI.
     *
     * \see ``font::rendering_options()``
     */
    [[nodiscard]] virtual rendering_options::ptr get_default_rendering_options() noexcept = 0;

    /**
     * Get the default Columns UI font fallback object when it’s required.
     *
     * \remark ``font::font_fallback()`` should be used in preference to this method. This method is only provided to
     * allow access to the font fallback object when the font being used is not provided by Columns UI.
     *
     * \see See ``font::font_fallback()`` for more details about the %font fallback object returned.
     */
    [[nodiscard]] virtual pfc::com_ptr_t<
#ifdef DWRITE_2_H_INCLUDED
        IDWriteFontFallback
#else
        IUnknown
#endif
        >
    get_default_font_fallback() noexcept = 0;

    /**
     * Set the font size for a client font.
     *
     * \param id font client GUID
     * \param size new size in DIP units
     * \throw exception_font_client_not_found thrown if an invalid GUID is supplied
     */
    virtual void set_font_size(GUID id, float size) = 0;

    /**
     * Register a new callback for when a font has changed.
     * @param id client or common font GUID
     * @param callback callback function
     * @return callback token that should be reset when you no longer want the callback function to be called
     */
    [[nodiscard]] virtual callback_token::ptr on_font_changed(GUID id, basic_callback::ptr callback) = 0;

    /**
     * Register a new callback for when the default rendering options have changed.
     *
     * \remark This callback should only be used if ``get_default_rendering_options()`` is being used. Do not use it if
     * you’re using ``font::rendering_options()``.
     *
     * @param callback callback function
     * @return callback token that should be reset when you no longer want the callback function to be called
     */
    [[nodiscard]] virtual callback_token::ptr on_default_rendering_options_changed(basic_callback::ptr callback) = 0;

    /**
     * Register a new callback for when the font fallback object has changed.
     *
     * \remark This callback should only be used if ``get_default_font_fallback()`` is being used. Do not use it if
     * you’re using ``font::font_fallback()``.
     *
     * @param callback callback function
     * @return callback token that should be reset when you no longer want the callback function to be called
     */
    [[nodiscard]] virtual callback_token::ptr on_default_font_fallback_changed(basic_callback::ptr callback) = 0;

    FB2K_MAKE_SERVICE_INTERFACE_ENTRYPOINT(manager_v3)
};

} // namespace cui::fonts
