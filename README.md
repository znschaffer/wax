# wax

A terminal audio player written in `C` with `ncurses`.

## Dependancies

We make use of [TagLib](https://taglib.org/) for audio metadata and [ncurses](https://invisible-island.net/ncurses/) for our UI.
If you're on OSX:

```bash
brew install taglib ncurses
export PKG_CONFIG_PATH="/opt/homebrew/opt/ncurses/lib/pkgconfig"
make
```

## Usage

```bash
wax </audio-directory-here>
```
