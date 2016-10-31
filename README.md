A Desktop application for communicating with the Floppy Orgel

Dependencies:
-------------
All of the following dependencies (except Qt) are included in this repository as submodules:

- Qt 5.4 (**Download here:** https://www.qt.io/qt5-4/)
- Serial Library by William Woodall and John Harrison
- RtMidi Library by Gary P. Scavone
- [embedded-midilib](https://github.com/coon42/embedded-midilib) by coon

Supported Operating systems:
----------------------------
- Windows only (Linux and Mac support coming soon)

Compiling Notes
---------------
RtMidi has a weird API, but it was the only one I found so far which supports Windows, Linux and Mac. It needs a compiler flag before it works correctly. Depending on which OS you are working on you should use one of the following flags:

  - Windows: __WINDOWS_MM__
  - Linux: __UNIX_JACK__ or __LINUX_ALSA__
  - Mac: __MACOSX_CORE__

If you are not providing any of these flags, the code will compile but any call to RtMidi API functions will lead to empty functions which do not fail but just do nothing.

On **Windows** you also have to add the **Setupapi.lib** to your linker symbols. I did not try to compile on Linux and Mac yet but maybe you will also need an additional lib there!
