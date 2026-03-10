#pragma once
#include <dinput.h>

HRESULT Hook_GetDeviceState(
    IDirectInputDevice8 *device,
    DWORD size,
    LPVOID data
);
