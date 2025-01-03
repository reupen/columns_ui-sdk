#pragma once

namespace cui::fonts {

enum font_mode_t {
    font_mode_common_items,
    font_mode_common_labels,
    font_mode_custom,
    font_mode_system,
};

enum font_type_t {
    font_type_items,
    font_type_labels,
};
enum font_type_flag_t {
    font_type_flag_items = (1 << 0),
    font_type_flag_labels = (1 << 1),
};

/**
 * Use this class if you wish to use the common fonts rather than implementing client
 */
class NOVTABLE common_callback {
public:
    virtual void on_font_changed(uint32_t changed_items_mask) const = 0;
};

/** One implementation in Columns UI - do not reimplement! */
class NOVTABLE manager : public service_base {
public:
    /** \brief Retrieves the font for the given client */
    virtual void get_font(const GUID& p_guid, LOGFONT& p_out) const = 0;

    /** \brief Retrieves common fonts. */
    virtual void get_font(const font_type_t p_type, LOGFONT& p_out) const = 0;
    /** \brief Sets your font as 'Custom' and to p_font */
    virtual void set_font(const GUID& p_guid, const LOGFONT& p_font) = 0;

    virtual void register_common_callback(common_callback* p_callback) = 0;
    virtual void deregister_common_callback(common_callback* p_callback) = 0;

    /** Helper */
    HFONT get_font(const GUID& p_guid) const
    {
        LOGFONT lf;
        memset(&lf, 0, sizeof(LOGFONT));
        get_font(p_guid, lf);
        return CreateFontIndirect(&lf);
    }

    /** Helper */
    HFONT get_font(const font_type_t p_type) const
    {
        LOGFONT lf;
        memset(&lf, 0, sizeof(LOGFONT));
        get_font(p_type, lf);
        return CreateFontIndirect(&lf);
    }

    FB2K_MAKE_SERVICE_INTERFACE_ENTRYPOINT(manager);
};

/**
 * Experimental version of the font management API with custom DPI support.
 *
 * One implementation in Columns UI - do not reimplement!
 */
class NOVTABLE manager_v2 : public service_base {
public:
    /** \brief Retrieve the font for the given client */
    virtual LOGFONT get_client_font(GUID guid, unsigned dpi = USER_DEFAULT_SCREEN_DPI) const = 0;

    /** \brief Retrieve a common font. */
    virtual LOGFONT get_common_font(font_type_t type, unsigned dpi = USER_DEFAULT_SCREEN_DPI) const = 0;

    /** \brief Set your font as 'Custom' and to the specified font. */
    virtual void set_client_font(GUID guid, const LOGFONT& font, int point_size_tenths) = 0;

    virtual void register_common_callback(common_callback* callback) = 0;
    virtual void deregister_common_callback(common_callback* callback) = 0;

    HFONT get_client_font_handle(GUID guid, unsigned dpi = USER_DEFAULT_SCREEN_DPI) const
    {
        LOGFONT lf = get_client_font(guid, dpi);
        return CreateFontIndirect(&lf);
    }

    HFONT get_common_font_handle(const font_type_t type, unsigned dpi = USER_DEFAULT_SCREEN_DPI) const
    {
        LOGFONT lf = get_common_font(type, dpi);
        return CreateFontIndirect(&lf);
    }

    FB2K_MAKE_SERVICE_INTERFACE_ENTRYPOINT(manager_v2);
};

/** Helper to simplify retrieving the font for a specific client. */
class helper {
public:
    void get_font(LOGFONT& p_out) const
    {
        if (m_api.is_valid()) {
            m_api->get_font(m_guid, p_out);
        } else
            uGetIconFont(&p_out);
    }
    HFONT get_font() const
    {
        if (m_api.is_valid()) {
            return m_api->get_font(m_guid);
        }
        return uCreateIconFont();
    }
    helper(GUID p_guid) : m_guid(p_guid)
    {
        try {
            standard_api_create_t(m_api);
        } catch (const exception_service_not_found&) {
        };
    }

private:
    service_ptr_t<manager> m_api;
    GUID m_guid;
};
class NOVTABLE client : public service_base {
public:
    virtual const GUID& get_client_guid() const = 0;
    virtual void get_name(pfc::string_base& p_out) const = 0;

    virtual font_type_t get_default_font_type() const = 0;

    virtual void on_font_changed() const = 0;

    template <class tClass>
    class factory : public service_factory_t<tClass> {};

    FB2K_MAKE_SERVICE_INTERFACE_ENTRYPOINT(client);

public:
    static bool create_by_guid(const GUID& p_guid, ptr& p_out);
};

}; // namespace cui::fonts
