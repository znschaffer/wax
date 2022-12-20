# wax

A terminal audio player written in `C` with `ncurses`.

## Dependancies

We make use of [TagLib](https://taglib.org/) for audio metadata and [ncurses](https://invisible-island.net/ncurses/) for our UI.
If you're on OSX:

```bash
brew install taglib ncurses
export PKG_CONFIG_PATH="/opt/homebrew/opt/ncurses/lib/pkgconfig"
make
sudo make install
```

## Usage

Wax is configured through a plaintext file `config` located inside $HOME/.config/wax/.

```
music_dir = ~/Music
```
is the default.

You can also pass in your music directory via the command line flag -d.

```bash
wax -d <dir>
```
