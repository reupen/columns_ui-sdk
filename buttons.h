#ifndef _UI_EXTENSION_BUTTONS_H_
#define _UI_EXTENSION_BUTTONS_H_

/**
 * \file buttons.h
 * \brief Button API
 */

namespace uie {

enum t_mask {
    /** \brief No transparency mask is used */
    MASK_NONE,
    /** \brief 1bpp bitmap transparency mask is used */
    MASK_BITMAP,
    /** \brief Pixels with specified colour are transparent */
    MASK_COLOUR,
};

/** \brief Identifies the type of GUID. */
enum t_button_guid {
    /** GUID identifies a button command */
    BUTTON_GUID_BUTTON,
    /** GUID identifies a context menu command */
    BUTTON_GUID_MENU_ITEM_CONTEXT,
    /** GUID identifies a main menu command */
    BUTTON_GUID_MENU_ITEM_MAIN,
};

/**
 * \brief Identifies the type of button.
 */
enum t_button_type {
    /** The button acts as a standard click button */
    BUTTON_TYPE_NORMAL,
    /** The button displays a drop-down menu when pressed */
    BUTTON_TYPE_DROPDOWN,
    /** The button displays an arrow which displays a drop-down menu */
    BUTTON_TYPE_DROPDOWN_ARROW
};

/**
 * \brief Identifies the state of a button.
 *
 * Combine multiple flags using bitwise or.
 *
 * \see button::get_button_state
 */
enum t_button_state {
    /** The button is enabled */
    BUTTON_STATE_ENABLED = (1 << 0),
    /** The button is in an active state */
    BUTTON_STATE_PRESSED = (1 << 1),
    /** The button displays a ToolTip */
    BUTTON_STATE_SHOW_TOOLTIP = (1 << 2),
    /** The default button state */
    BUTTON_STATE_DEFAULT = BUTTON_STATE_ENABLED | BUTTON_STATE_SHOW_TOOLTIP,
};

/** \brief Class implemented by button hosts to receive notification of button events */
class NOVTABLE button_callback {
public:
    /**
     * Called when the state of the button changed
     *
     * \param [in]    p_new_state    Combination of uie::t_button_state
     */
    virtual void on_button_state_change(unsigned p_new_state) = 0; // see t_button_state

    /**
     * Called when the state of the command changed
     *
     * \param [in]    p_new_state    Index of new command state
     *
     * \see button::get_command_state_index, button::get_command_state_count
     */
    virtual void on_command_state_change(unsigned p_new_state) = 0; // NCI
};

/** \brief Service that provides buttons for a toolbar */
class NOVTABLE button : public service_base {
public:
    /**
     * \brief Get the identifier of the button.
     *
     * Use get_type_guid() to determine what the GUID repreesnts.
     *
     * \return GUID identifying the command represented by the class
     */
    virtual const GUID& get_item_guid() const = 0;

    /**
     * \brief Get whether get_item_guid() specifies a main menu item,
     * a context menu, or a custom button command.
     *
     * \Note Only recommended use of button-only buttons are dropdown-only buttons
     *
     * \return type of command represnted by this class
     *
     * \see t_button_guid
     */
    virtual t_button_guid get_guid_type() const { return BUTTON_GUID_MENU_ITEM_MAIN; }

    /**
     * \deprecated Use button_v2::get_item_bitmap() instead.
     *
     * \brief Get a handle to a bitmap and its transparency mask of the menu item.
     *
     * Caller presumes ownership of bitmap.
     *
     * \param [in]    cr_btntext    Colour to use for text/foreground
     * \param [out]    bm_mask        HBITMAP of transparency mask. This is a monochrome bitmap.
     *
     * \note In the toolbar control, transparency masks are supported on all versions of windows;
     * where as 32 bpp bitmaps with 8bpp alpha channel are supported only under
     * common controls version 6.
     *
     * \note Ensure you do not create a mask bitmap if you fail to create main bitmap
     *
     * \remark    Masks generated from a colour are only supported on bitmaps with
     *            a colour depth less than or equal to 8bpp.
     *
     * \return HBITMAP of menu item
     */
    virtual HBITMAP get_item_bitmap(unsigned command_state_index, COLORREF cr_btntext, t_mask& p_mask_type,
        COLORREF& cr_mask, HBITMAP& bm_mask) const
        = 0;

    /**
     * \brief Get the type of the button.
     *
     * \return Type of button
     *
     * \see t_button_type
     */
    virtual t_button_type get_button_type() const { return BUTTON_TYPE_NORMAL; }
    /**
     * \brief Gets menu items for drop-down buttons.
     *
     * \param [out]    p_out        Receives menu items
     */
    virtual void get_menu_items(menu_hook_t& p_out) {}

    /**
     * \brief Get the current button state.
     *
     * \return Button state
     *
     * \see t_button_state
     */
    virtual unsigned get_button_state() const { return BUTTON_STATE_DEFAULT; }

    /**
     * \brief Get the current state of the command. For example, in a "Play or pause"
     * command this would indicate the play or pause state.
     *
     * \warning This method is not currently used.
     *
     * \return Index of current command state
     */
    virtual unsigned get_command_state_index() const { return 0; }

    /**
     * \brief Get the total number of possible command states.
     *
     * \warning This method is not currently used.
     *
     * \return Total count of possible command states
     */
    virtual unsigned get_command_state_count() const { return 1; }

    /**
     * \brief Get the name of specified command state.
     *
     * \warning This method is not currently used.
     *
     * \param [in]    index        Index of command state's name to retrieve
     * \param [out]   p_out        Receives command state name
     */
    virtual void get_command_state_name(unsigned index, pfc::string_base& p_out) const { p_out = "Default"; }

    /**
     * \brief Register a button_callback class to receive callbacks.
     *
     * \param [in]    p_callback    Reference to callback object requesting callbacks
     */
    virtual void register_callback(button_callback& p_callback) {}

    /**
     * \brief Deregister a button_callback class to stop receiving callbacks.
     *
     * \param [in]    p_callback    Reference to callback object being deregistered.
     *
     * The object implementing this method must not keep any references to the specified
     * callback object after this method returns
     */
    virtual void deregister_callback(button_callback& p_callback) {}

    FB2K_MAKE_SERVICE_INTERFACE_ENTRYPOINT(button);
};

/**
 * \brief Extension of button interface; allows icons to be used as default button images.
 *
 * New in SDK version 6.5.
 */
class NOVTABLE button_v2 : public button {
public:
    enum handle_type_t {
        handle_type_bitmap = 0, /**< HBITMAP */
        handle_type_icon = 1, /**< HICON */
    };
    /**
     * \brief Get a handle to a image of the menu item.
     *
     * Caller presumes ownership of bitmap.
     *
     * \param [in]    command_state_index        Not used.
     * \param [in]    cr_btntext                 Colour to use for text/foreground
     * \param [in]    cx_hint                    Displayed bitmap width
     * \param [in]    cy_hint                    Displayed bitmap height
     * \param [out]   handle_type                Receives the type of handle returned (icon or bitmap)
     *
     * \note Use alpha channel for transparency.
     *
     * \note You can vary the returned image depending on whether dark mode is active by using
     *       cui::colours::is_dark_mode_active(). All button images are flushed when the dark
     *       mode status changes.
     *
     * \return Handle of image
     */
    virtual HANDLE get_item_bitmap(unsigned command_state_index, COLORREF cr_btntext, unsigned cx_hint,
        unsigned cy_hint, unsigned& handle_type) const
        = 0;

    /**
     * \brief Deprecated uie::button method, not used for uie::button_v2.
     */
    HBITMAP get_item_bitmap(unsigned command_state_index, COLORREF cr_btntext, t_mask& p_mask_type, COLORREF& cr_mask,
        HBITMAP& bm_mask) const override
    {
        return nullptr;
    }

    FB2K_MAKE_SERVICE_INTERFACE(button_v2, button);
};

/** \brief Subclass of uie::button, for buttons based upon a context menu item */
class NOVTABLE menu_button : public button {
public:
    /**
     * \brief Set the subcommand that subsequent function calls will refer to.
     *
     * Called after instantiation and before other command-related methods.
     *
     * \param [in]    p_subcommand    Specifies the subcommand that this object wll represent
     */
    virtual void select_subcommand(const GUID& p_subcommand) = 0; // NCI

    FB2K_MAKE_SERVICE_INTERFACE(menu_button, button);
};

/** \brief Subclass of uie::button, for buttons that implement their own command */
class NOVTABLE custom_button : public button {
public:
    t_button_guid get_guid_type() const override { return BUTTON_GUID_BUTTON; }

    /**
     * \brief Execute the custom button's command.
     *
     * \param [in]    p_items        Items to perform the command on
     */
    virtual void execute(const pfc::list_base_const_t<metadb_handle_ptr>& p_items) = 0;

    /**
     * \brief Get the name of the custom button.
     *
     * \param [out]    p_out        Receives the name of the button, UTF-8 encoded
     */
    virtual void get_name(pfc::string_base& p_out) const = 0;

    /**
     * \brief Get the description of the custom button.
     *
     * \param [out]    p_out      Receives the description of the button, UTF-8 encoded
     * \return                    whether p_out was set to a non-empty value
     */
    virtual bool get_description(pfc::string_base& p_out) const { return false; }

    static bool g_button_get_name(const GUID& p_guid, pfc::string_base& p_out)
    {
        service_enum_t<uie::button> e;
        service_ptr_t<uie::button> ptr;
        service_ptr_t<uie::custom_button> p_button;
        while (e.next(ptr)) {
            if (ptr->get_guid_type() == BUTTON_GUID_BUTTON && ptr->service_query_t(p_button)
                && ptr->get_item_guid() == p_guid) {
                p_button->get_name(p_out);
                return true;
            }
        }
        return false;
    }

    FB2K_MAKE_SERVICE_INTERFACE(custom_button, button);
};

/** \brief Service factory for buttons. */
template <class T>
class button_factory : public service_factory_t<T> {};

}; // namespace uie

#endif
