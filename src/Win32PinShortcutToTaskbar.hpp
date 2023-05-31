// SPDX-License-Identifier: MIT
//
// This source code is adopted from the following article and code by Gee Law
//
// pin.cc
//   https://geelaw.blog/entries/msedge-pins/assets/pin.cc
//
// Microsoft Edge (Chromium) sets a bad and good example: the case of Taskbar pinning
//   https://geelaw.blog/entries/msedge-pins/#code
//
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>
#include <array>

//
//
// NOTE : Before using this function, do not forget to initialize COM.
//
//

inline bool Win32PinShortcutToTaskbar(bool pinning, const wchar_t *shortcutPath)
{
    struct PIDLFromPath {
        ITEMIDLIST __unaligned *pidl = nullptr;

        PIDLFromPath(const wchar_t *path) : pidl{ILCreateFromPathW(path)} {}

        ~PIDLFromPath()
        {
            if (pidl) {
                ILFree(pidl);
            }
        }

        operator PIDLIST_ABSOLUTE() const
        {
            return pidl;
        }
    };

    static constexpr GUID CLSID_TaskbandPin = {0x90aa3a4e,
                                               0x1cba,
                                               0x4233,
                                               {0xb8, 0xbb, 0x53, 0x57, 0x73, 0xd4, 0x84, 0x49}};

    static constexpr GUID IID_IPinnedList3 = {0x0dd79ae2,
                                              0xd156,
                                              0x45d4,
                                              {0x9e, 0xeb, 0x3b, 0x54, 0x97, 0x69, 0xe9, 0x40}};

    enum PLMC : int32_t { PLMC_EXPLORER = 4 };

    struct IPinnedList3Vtbl;
    struct IPinnedList3 {
        IPinnedList3Vtbl *vtbl;
    };

    using ReleaseFunc = ULONG STDMETHODCALLTYPE(IPinnedList3 * that);
    using ModifyFunc =
        HRESULT STDMETHODCALLTYPE(IPinnedList3 * that, PCIDLIST_ABSOLUTE unpin, PCIDLIST_ABSOLUTE pin, PLMC caller);

    struct IPinnedList3Vtbl {
        void        *QueryInterface;
        void        *AddRef;
        ReleaseFunc *Release;
        void        *MtthodSlot_4_to_16[13];
        ModifyFunc  *Modify;
    };

    constexpr auto plmc = PLMC::PLMC_EXPLORER;

    // We need to create PIDL which will be passed to IPinnedList3::Modify()
    PIDLFromPath pidl(shortcutPath);

    IPinnedList3 *pinnedList = nullptr;

    HRESULT hr = S_OK;

    {
        // Retrieve TaskbandPin
        hr = CoCreateInstance(CLSID_TaskbandPin, nullptr, CLSCTX_INPROC_SERVER, IID_IPinnedList3, (void **)&pinnedList);
        if (FAILED(hr) || !pinnedList) {
            printf("%s(%d) : FAILED\n", __FILE__, __LINE__);
            goto end;
        }
    }

    // Invoke IPinnedList3::Modify to pin/unpin the target (pidl).
    if (pinning) {
        // pin
        hr = pinnedList->vtbl->Modify(pinnedList, nullptr, pidl, plmc);
        if (FAILED(hr)) {
            printf("%s(%d) : FAILED\n", __FILE__, __LINE__);
            goto end;
        }
    } else {
        // unpin
        hr = pinnedList->vtbl->Modify(pinnedList, pidl, nullptr, plmc);
        if (FAILED(hr)) {
            printf("%s(%d) : FAILED\n", __FILE__, __LINE__);
            goto end;
        }
    }

end:
    if (pinnedList) {
        pinnedList->vtbl->Release(pinnedList);
        pinnedList = nullptr;
    }

    return SUCCEEDED(hr);
}
