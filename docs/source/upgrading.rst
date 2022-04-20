Upgrading the SDK
=================

Version 7.0-beta.1
------------------

This version of the Columns UI SDK requires Visual Studio 2022.

The project file was also renamed ``columns_ui-sdk-public.vcxproj``.

New in this version
~~~~~~~~~~~~~~~~~~~

This version adds:

- `support for the Columns UI dark mode`_
- preliminary support for compiling x64 panels
- :class:`uie::container_uie_window_v3_t`
- :class:`uie::container_window_v3`
- :class:`uie::simple_command_menu_node`
- :class:`uie::splitter_item_full_v2_t`
- :class:`uie::splitter_item_full_v3_t`
- :class:`uie::splitter_item_full_v3_impl_t`
- :member:`uie::splitter_window::size_and_dpi`
- :class:`cui::fcl::dataset_v2`
- :class:`cui::fonts::manager_v2`
- :func:`cui::config_objects::get_locked_panel_resizing_allowed()`
- :func:`uie::win32::paint_background_using_parent()`

Renamed namespaces
~~~~~~~~~~~~~~~~~~

- The ``ui_extension`` namespace was renamed ``uie``
- The ``columns_ui`` namespace was renamed ``cui``

Aliases exist for the old names for backwards compatibility.

Deprecated in this version
~~~~~~~~~~~~~~~~~~~~~~~~~~

The following classes were deprecated:

- :class:`uie::container_ui_extension_t`
- :class:`uie::container_ui_extension`
- :class:`uie::container_menu_ui_extension`
- :class:`uie::container_uie_window_t`
- :class:`uie::container_window_autorelease_t`
- :class:`uie::container_window_release_t`
- :class:`uie::container_window`

The following functions were deprecated:

- :func:`uHeader_InsertItem()`
- :func:`uHeader_SetItemText()`
- :func:`uHeader_SetItemWidth()`
- :func:`uToolTip_AddTool()`
- :func:`uComboBox_SelectString()`
- :func:`win32_helpers::send_message_to_all_children()`
- :func:`win32_helpers::tooltip_add_tool()`

Removed in this version
~~~~~~~~~~~~~~~~~~~~~~~

Example components are no longer bundled with the SDK. These are now published on GitHub:

- `Example panel`_
- `Console panel`_

The following classes were removed:

- :class:`uie::window_base_t`
- :class:`logfont_os_menu`
- :class:`logfont_os_icon`
- :class:`logfont_os_from_utf8`
- :class:`logfont_utf8_from_os`

The following functions were removed:

- :func:`uGetClassLong()`
- :func:`uSetClassLong()`
- :func:`convert_logfont_utf8_to_os()`
- :func:`convert_logfont_os_to_utf8()`

The following macros were removed:

- :c:macro:`uT()`
- :c:macro:`uTS()`
- :c:macro:`Tu()`
- :c:macro:`TSu()`

.. _support for the Columns UI dark mode: Dark mode
.. _Example panel: https://github.com/reupen/example_panel
.. _Console panel: https://github.com/reupen/console_panel
