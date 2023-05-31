// SPDX-License-Identifier: MIT
// Copyright (c) 2020 by Gee Law
//
// Microsoft Edge (Chromium) sets a bad and good example: the case of Taskbar pinning
//   by Gee Law
//   https://geelaw.blog/entries/msedge-pins/#code
//
//  > ... Basically, we follow the steps used by Edge (Chromium):
//  >
//  > 1. Initialise COM.
//  > 2. Use ILCreateFromPathW to get an absolute PIDL.
//  > 3. Use CoCreateInstance to activate CLSID_TaskbandPin with interface IID_IPinnedList3.
//  > 4. Use Modify method with the PIDL to pin/unpin the target.
//  > 5. Release the interface, the PIDL and uninitialse COM.
//
// pin.cc
//   Copyright (c) 2020 by Gee Law
//   under MIT license.
//   https://geelaw.blog/entries/msedge-pins/assets/pin.cc

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <array>

#include "Win32PinShortcutToTaskbar.hpp"

int main()
{
    //
    //
    // NOTE : Do not forget to initialize COM.
    //
    //
    struct CoInit {
        CoInit()
        {
            CoInitialize(nullptr);
        }

        ~CoInit()
        {
            CoUninitialize();
        }
    };
    CoInit coInit{};

    wchar_t moduleFileName[MAX_PATH] = {0};
    GetModuleFileNameW(nullptr, moduleFileName, static_cast<DWORD>(std::size(moduleFileName)));

    // NOTE : You can pass your own command instead of moduleFileName.
    // See https://geelaw.blog/entries/msedge-pins/assets/pin.cc
    bool const pinning = true;
    bool const result  = Win32PinShortcutToTaskbar(pinning, moduleFileName);

    printf("result = %s\n", result ? "OK" : "NG");

    printf("Press enter to close\n");
    getchar();
}
