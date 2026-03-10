#pragma once
#include <windows.h>
#include <dinput.h>

extern HMODULE g_dinput;

typedef HRESULT (WINAPI *DirectInput8CreateProc)(
    HINSTANCE,
    DWORD,
    REFIID,
    LPVOID *,
    LPUNKNOWN
);

extern DirectInput8CreateProc Real_DirectInput8Create;

bool LoadOriginalDInput();
