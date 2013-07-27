spa
===

In my eternal quest to find inner peace: yet another silly game, but this time
using allegro5 instead of sdl2 (and written in C, rather than really-bad-C++).

This one I think is supposed to be a [Galaxian][] clone.

[Galaxian]: https://en.wikipedia.org/wiki/Galaxian

requirements
------------

For the purposes of potential future Android compatibility, I develop this
with allegro5.1 locally, though allegro5.0 should also work fine until such
time that I actually end up using allegro5.1-specific features.

So, yeah, [allegro5][] is indeed a mandatory dependency.

Once the dependencies are installed, (along with `glibc`, `gcc`, `glib`, and
`make`) you'll be able to build `spa`.

[allegro5]: http://alleg.sourceforge.net

building
--------

Just run `make`; this will create the `spa` binary.


