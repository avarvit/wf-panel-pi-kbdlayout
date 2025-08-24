# wf-panel-pi `kbdlayout`

A keyboard layout reporting/switching widget for wf-panel-pi

## About

`kbdlayout` is a widget made for wf-panel-pi, the default shell taskbar
for Wayland environments in Raspberry Pi OS (bookworm). When used in a
multi-language environment, it displays the current keyboard layout (aka
language). When clicked on, it pops up a menu with the available layouts
(aka languages) and allows switching the current keyboard layout in a
point-and-click way.

## Prerequisites

`kbdlayout` cannot work on its own. It is based on functionality and
messaging that must be added to the Wayland compositor. Currently,
`kbdlayout` can be made to work with both Wayfire and Labwc, the two
supported Wayland compositors for Raspberry Pi OS; however, both these
compositors must be adjusted to add the required functionality (see
below).

## Build and install

1. Dependencies

To build, you must first install the following dependencies:
```
sudo apt install libglibmm-2.4-dev libgtkmm-3.0-dev libxkbcommon-dev wf-panel-pi-dev
```

2. Configure meson

To configure the build:
```
meson setup builddir --prefix=/usr --libdir=/usr/lib/<library-location>
```
On a 32-bit system, `<library-location>` should be `arm-linux-gnueabihf`;
On a 64-bit system, `<library-location>` should be `aarch64-linux-gnu`.

3. Build

Build using
```
meson compile -C builddir # or, equivalently, (cd builddir && ninja compile)
```

4. Install

To install:
```
sudo meson install -C builddir @ (or, equivalently, (cd builddir && sudo ninja install)
```
This will install three files:

-  /usr/lib/aarch64-linux-gnu/wf-panel-pi/libkbdlayout.so` the actual plugin
- `/usr/share/wf-panel-pi/metadata/kbdlayout.xml` XML configuration for the plugin
- `/usr/lib/aarch64-linux-gnu/pkgconfig/wf-panel-pi-kbdlayout.pc` plugin pkgconfig info

5. Uninstall

Uninstallation is just as simple:
```
(cd builddir && sudo ninja uninstall)
```

## Configuring wf-panel-pi
To start using the plugin, edit your `$HOME/.config/wf-panel-pi.ini`. If this file
does not exist, do not worry, run `wf-panel-pi` once and it will get created. Then,
edit the file and locate a line starting with `widgets_right=`, then change it to
include kbdlayout as follows (you can place the kbdlayout wherever you please with
respect to other widgets). If no such line exists, feel free to add it.
```
widgets_right=tray ejecter updater spacing2 bluetooth spacing2 netman spacing2 volumepulse spacing2 clock spacing2 kbdlayout spacing2 connect
```

## Configuring the compositor

For Wayfire, a Wayfire plugin must be added that implements the "kbdd" functionality
required by the `kbdlayout` widget. See https://github.com/avarvit/wayfire-kbdd-plugin
for more information and installation instructions.

For Labwc, the functionality is provided by preloading a library that hooks a few
wlroots calls. See https://github.com/avarvit/wlroots-kbdd or more information
and installation instructions.
