#include "input_hook.h"
#include <windows.h>

HRESULT Hook_GetDeviceState(
    IDirectInputDevice8 *device,
    DWORD size,
    LPVOID data
)
{
    typedef HRESULT (WINAPI *GetDeviceStateProc)(
        IDirectInputDevice8 *,
        DWORD,
        LPVOID
    );

    static GetDeviceStateProc Real_GetDeviceState = NULL;

    if (!Real_GetDeviceState)
    {
        void **vtable = *(void ***)device;
        Real_GetDeviceState = (GetDeviceStateProc)vtable[9];
    }

    HRESULT hr = Real_GetDeviceState(device, size, data);

    if (SUCCEEDED(hr) && size == 256)
    {
        BYTE *keys = (BYTE *)data;

        // exemplo: garantir tecla pressionada corretamente
        if (keys[DIK_UP])
            keys[DIK_UP] = 0x80;
    }

    return hr;
}
