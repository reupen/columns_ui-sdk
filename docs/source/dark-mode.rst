Dark mode
=========

Columns UI 2.0.0 and newer feature an optional dark mode on Windows 10 version
2004 and newer.

If dark mode is active, panels should render system UI elements, such as common
controls and scroll bars, with a dark theme.

The following code sample shows how to enable dark scroll bars for a window
depending on whether dark mode is enabled::

  const auto is_dark = cui::colours::is_dark_mode_active()
  SetWindowTheme(hwnd, is_dark ? L"DarkMode_Explorer" : nullptr, nullptr);

Some common controls have a native dark mode that can also be activated using
``SetWindowTheme()`` and one of the following themes:

- ``DarkMode``
- ``DarkMode_Explorer``
- ``DarkMode_CFD``

Handling dynamic dark mode changes
----------------------------------

If you have an existing :class:`cui::colours::client` implementation, you should:

1. Override :func:`cui::colours::client::get_supported_bools()` to return the ``cui::colours::bool_flag_dark_mode_enabled`` flag (use ``|`` to combine multiple flags). For example::

    uint32_t get_supported_bools() const override
    {
        return colours::bool_flag_use_custom_active_item_frame | colours::bool_flag_dark_mode_enabled;
    }

2. Override :func:`cui::colours::client::on_bool_changed()` to handle dynamic dark mode changes. For example::

    void on_bool_changed(uint32_t changed_items_mask) const override
    {
        if (changed_items_mask & colours::bool_flag_dark_mode_enabled) {
            const auto is_dark = cui::colours::is_dark_mode_active();
            for (auto hwnd : hwnds) {
                SetWindowTheme(hwnd, is_dark ? L"DarkMode_Explorer" : nullptr, nullptr);
            }
        }
    }

If you don’t have an existing :class:`cui::colours::client` implementation, you can use :class:`cui::colours::dark_mode_notifier` to react to dark mode status changes. For example::

    // Member variable
    std::unique_ptr<cui::colours::dark_mode_notifier> m_dark_mode_notifier;

    // Window procedure
    case WM_CREATE:
        // ...
        SetWindowTheme(hwnd, is_dark ? L"DarkMode_Explorer" : nullptr, nullptr);
        m_dark_mode_notifier = std::make_unique<cui::colours::dark_mode_notifier>([hwnd] {
            SetWindowTheme(hwnd, is_dark ? L"DarkMode_Explorer" : nullptr, nullptr);
        });
        return 0;
    case WM_DESTROY:
        m_dark_mode_notifier.reset();
        return 0;
