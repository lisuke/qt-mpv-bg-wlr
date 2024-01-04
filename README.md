# qt-mpv-bg-wlr
>
> A mpv based video wallpaper program for wlroots based wayland compositors. (Video Dynamic Wallpaper for Linux).

## Dependencies

``` bash
qt5
mpv
layer-shell-qt
pkg-config
meson
```

## Building

``` bash
git clone https://github.com/lisuke/qt-mpv-bg-wlr
cd qt-mpv-bg-wlr
meson setup build
ninja -C build
```

## Installing

`sudo ninja -C build install`

## Uninstalling

`sudo ninja -C build uninstall`

## Running

`qt-mpv-bg-wlr ~/reimu.mp4`

## Controlling

`echo 'cycle pause' | socat - /tmp/mpv-socket-eDP-1`

## Bug Reports

Please file bug reports at <https://github.com/lisuke/qt-mpv-bg-wlr>

## Contributing

Please submit patches to <https://github.com/lisuke/qt-mpv-bg-wlr>

## Packages

If you're on Arch there's an AUR package <https://aur.archlinux.org/packages/qt-mpv-bg-wlr/>

## reference

- [GhostNaN/mpvpaper](https://github.com/GhostNaN/mpvpaper)
- [xdavidwu/qt-video-wlr](https://github.com/xdavidwu/qt-video-wlr)
- [libmpv/qt_opengl](https://github.com/mpv-player/mpv-examples/tree/master/libmpv/qt_opengl)
