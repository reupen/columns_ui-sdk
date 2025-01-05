Using fonts
===========

Columns UI features centralised configuration of fonts on its Font preferences
page. This allows fonts for different parts of the UI to be configured in one
place.

Third-party panel components can add their own entries to the Font preferences
page, and query fonts that have configured.

Adding a new entry
------------------

To add a new element to the Columns UI Fonts preferences page, you need to
implement a :class:`cui::fonts::client` service and register it using the
:class:`cui::fonts::client::factory` factory.

For an example of this, see `the Console panel source`_.

A single component can add multiple entries by implementing
:class:`cui::fonts::client` multiple times, each with a different client GUID.

Querying fonts
--------------

:func:`cui::fonts::create_client_hfont_with_fallback()` can be used to create to
an ``HFONT`` for a particular font client, while
:func:`cui::fonts::get_client_log_font_with_fallback()` and can be used to get
the ``LOGFONT`` structure for a particular font client. There are also various
other utility functions for other scenarios in the :cpp:type:`cui::fonts`
namespace.

It’s also possible to create an ``HFONT`` for a common font using
:func:`cui::fonts::create_common_hfont_with_fallback()`. However, given the ease
of implementing a font client to add a custom entry, it’s not generally
recommended to use the common fonts directly.

Panels using DirectWrite can use the experimental
:func:`cui::fonts::get_client_font()` function to retrieve a
:class:`cui::fonts::font` instance and create a ``IDWriteTextFormat`` object.
Note that :func:`cui::fonts::get_client_font()` requires Columns UI 3.0.0 alpha
1 or later, and compatibility may be broken before the final Columns UI 3.0.0
release. If a compatible version of Columns UI isn’t installed, the function
will return an empty ``std::optional``. Fallback logic should be implemented for
scenario.

.. _the console panel source: https://github.com/reupen/console_panel/blob/38983f68cea0bb6843ce8401f8601bb0651bc8c4/foo_uie_console/main.cpp#L624-L659
