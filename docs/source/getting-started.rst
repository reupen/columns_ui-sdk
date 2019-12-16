Getting started
===============

The Columns UI SDK provides interfaces you can use to:

-  Create windows controlled by a host and embedded in the host’s window
-  Provide information about commands to be used as a toolbar button

Installation
------------

You’ll need:

-  `Microsoft Visual Studio 2019`_
-  `foobar2000 SDK`_

To install, extract the columns_ui-sdk.7z archive to the foobar2000
subdirectory of your foobar2000 SDK folder.

Usage
-----

Insert the columns_ui-sdk project into your solution, and add it
as a dependency for your project. Then
``#include "columns_ui-sdk/ui_extension.h"`` in your project as
needed.

Examples
--------

Two examples are included in the SDK:

- foo_uie_example – a simple panel that displays some text and implements
  a context menu item
- foo_uie_console – a console viewer


Panel APIs
----------

APIs
~~~~

Clients should implement :class:`uie::window`. Specific sub-classes exist for

-  Menus: :class:`uie::menu_window`
-  Playlists: :class:`uie::playlist_window`
-  Splitter panels: :class:`uie::splitter_window`

Hosts should implement :class:`uie::window_host`. Hosts wishing to expose
external control methods can implement :class:`uie::window_host_with_control`
instead.

Helpers
~~~~~~~

The preferred method of implementing the window class is to derive from
:type:`uie::container_ui_extension` (although this may not be suitable for
single-instance panels or dialog-based panels).

Button APIs
-----------

APIs
~~~~

The base class for buttons is :class:`uie::button`.

If you wish to provide default bitmaps and additional information for your menu
items, derive from :class:`uie::menu_button`. If you wish to implement a custom
button not based upon a menu item, derive from :class:`uie::custom_button`.

Standard windows
~~~~~~~~~~~~~~~~

The GUIDs for the standard panels may be found in the :type:`cui::panels`
namespace. The GUIDs for the standard toolbars may be found in the
:type:`cui::toolbars` namespace.

You may use these GUIDs to create the standard windows in your own component; do
not use them as GUIDs for your own windows.

.. _Microsoft Visual Studio 2019: https://visualstudio.microsoft.com/downloads/
.. _foobar2000 SDK: http://www.foobar2000.org/SDK
