#include "proxy.h"

HMODULE g_dinput = NULL;
DirectInput8CreateProc Real_DirectInput8Create = NULL;

bool LoadOriginalDInput()
{
    char path[MAX_PATH];

    GetSystemDirectoryA(path, MAX_PATH);
    strcat_s(path, "\\dinput8.dll");

    g_dinput = LoadLibraryA(path);

    if (!g_dinput)
        return false;

    Real_DirectInput8Create =
        (DirectInput8CreateProc)GetProcAddress(
            g_dinput,
            "DirectInput8Create"
        );

    return (Real_DirectInput8Create != NULL);
}
