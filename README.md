This is a trivial fork of [`pin.cc`](https://geelaw.blog/entries/msedge-pins/assets/pin.cc) by [Gee Law](https://geelaw.blog/).

For details, take a look at the following article:  
[Microsoft Edge (Chromium) sets a bad and good example: the case of Taskbar pinning](https://geelaw.blog/entries/msedge-pins/)


## Prerequisite

- Windows 10/11
- Visual C++ 2022


## Demo

Invoke `.\run.bat`.  
It will build and run `.\x64\Release\Win32PinShortcutToTaskbar.exe`.  The executable file pins (adds) itself to your taskbar.


## Use it as a library

You can use `src/Win32PinShortcutToTaskbar.hpp` as a single header library.  
Do not forget to initialize COM before using it.  See `src/main.cpp` for actual example.
