.. index:: Gerbera Icons

Gerbera Icons
==================

Gerbera server supports the ability to configure a custom icon that displays when UPNP clients render the icon for
the media server.  The system supports the following icons support

- Default icons from ``/web/icons/`` - This is the existing default behavior (*no configuration required*)
- Custom static list from ``config.xml`` - Allows complete custom definition of the icon properties
- Dynamic list from ``config.xml`` - Includes single template icon, and auto-generated sizes


Default Icons
-------------

If you are not interested in custom icons...The system continues to support the standard Gerbera Icon

Custom Static List
------------------

You may add configuration to the ``<server>`` stanza in the ``config.xml`` file.  The system uses the static list to
advertise the icon set to UPNP clients.

Example Configuration
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: xml

    <server>
        <icons>
            <icon resolution="120x120" depth="24" mime-type="image/png" url="/icons/mt-icon120.png">
                /web/icons/mt-icon120.png
            </icon>
            <icon resolution="120x120" depth="24" mime-type="image/x-ms-bmp" url="/icons/mt-icon120.bmp">
                /web/icons/mt-icon120.bmp
            </icon>
            <icon resolution="120x120" depth="24" mime-type="image/jpeg" url="/icons/mt-icon120.jpg">
                /web/icons/mt-icon120.jpg
            </icon>
        </icons>
    </server>


The system broadcasts the ``url`` attribute to UPNP clients, and uses the ``icon.text()`` as the path to the image.

Dynamic List
------------

The dynamic list support uses a single icon as a template along with ImageMagick to generate the various
icon types during runtime.  You may add configuration to the ``<server>`` stanza in the ``config.xml`` file.

    The system respects ``<icons>`` static list over ``<icon>`` dynamic list.  It is best to choose 1 option and configure
    only that option.

Example Configuration
~~~~~~~~~~~~~~~~~~~~~

.. code-block:: xml

    <server>
        <icon>/path/to/icon.jpg</icon>
    </server>

Supported Icon Types
~~~~~~~~~~~~~~~~~~~~

The system can use a valid image as a template to generate the 3 types listed below.

- image/jpeg
- image/png
- image/x-ms-bmp

Supported Icon Resolutions
~~~~~~~~~~~~~~~~~~~~~~~~~~

The dynamic list generates 3 resolutions (*in pixels*) for each of the types listed above.

- 120x120
- 48x48
- 32x32

Icon Content Handler
--------------------

The system uses a request handler to provide the dynamic icon generation.  The system broadcasts the icon routes
similar to below

- ``/content/icons/grb-icon-120.png``
- ``/content/icons/grb-icon-48.png``
- ``/content/icons/grb-icon-32.png``

The system generates the icon from the template icon at runtime based on the ``<icon>`` entry in the configuration.