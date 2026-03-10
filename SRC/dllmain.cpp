#include <windows.h>
#include "proxy.h"

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD reason,
    LPVOID reserved
)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            LoadOriginalDInput();
            break;
        }

        case DLL_PROCESS_DETACH:
        {
            if (g_dinput)
                FreeLibrary(g_dinput);
            break;
        }
    }

    return TRUE;
}

extern "C"
HRESULT WINAPI DirectInput8Create(
    HINSTANCE hinst,
    DWORD version,
    REFIID riid,
    LPVOID *out,
    LPUNKNOWN punk
)
{
    if (!Real_DirectInput8Create)
        return E_FAIL;

    return Real_DirectInput8Create(
        hinst,
        version,
        riid,
        out,
        punk
    );
}
