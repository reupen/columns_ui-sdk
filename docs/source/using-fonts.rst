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

GDI
~~~

:func:`cui::fonts::create_hfont_with_fallback()` can be used to create to an
``HFONT`` for a particular font client, while
:func:`cui::fonts::get_log_font_with_fallback()` and can be used to get the
``LOGFONT`` structure for a particular font client. There are also various other
utility functions for other scenarios in the :cpp:type:`cui::fonts` namespace.

DirectWrite
~~~~~~~~~~~

Panels using DirectWrite can use the experimental :func:`cui::fonts::get_font()`
function to retrieve a :class:`cui::fonts::font` instance. The
:func:`cui::fonts::font::create_text_format()` member function can then be
called to create an ``IDWriteTextFormat`` object.

You should also apply the rendering options returned by
:func:`cui::fonts::font::rendering_options()` when rendering text using the text
format.

Note that :func:`cui::fonts::get_font()` requires Columns UI 3.0.0 beta 1 or
later, and compatibility may be broken before the final Columns UI 3.0.0
release.

If a compatible version of Columns UI isn’t installed,
:func:`cui::fonts::get_font()` will return an empty ``std::optional``. Fallback
logic should be implemented for this scenario.

Common fonts
~~~~~~~~~~~~

It’s also possible to pass the ID of a common font (i.e.
``cui::fonts::items_font_id`` or ``cui::fonts::labels_font_id``) rather than the
ID of a font client to the functions mentioned in the previous two sections.
However, given the ease of implementing a font client to add a custom entry,
it’s not generally recommended to use the common fonts directly.

.. _the console panel source: https://github.com/reupen/console_panel/blob/38983f68cea0bb6843ce8401f8601bb0651bc8c4/foo_uie_console/main.cpp#L624-L659
