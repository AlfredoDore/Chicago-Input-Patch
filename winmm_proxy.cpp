#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

static void debug_log(const char* msg)
{
    OutputDebugStringA(msg);
}
#include <windows.h>
#include <mmsystem.h>
#include <cstring>

typedef MMRESULT (WINAPI *JOYGETPOSEX_PROC)(UINT, LPJOYINFOEX);
typedef MMRESULT (WINAPI *JOYGETDEVCAPSA_PROC)(UINT_PTR, LPJOYCAPSA, UINT);
typedef UINT (WINAPI *JOYGETNUMDEVS_PROC)(void);

static HMODULE realWinmm = NULL;

static JOYGETPOSEX_PROC realJoyGetPosEx = NULL;
static JOYGETDEVCAPSA_PROC realJoyGetDevCapsA = NULL;
static JOYGETNUMDEVS_PROC realJoyGetNumDevs = NULL;

static UINT cachedJoystick = (UINT)-1;

UINT FindActiveJoystick() {

    if (cachedJoystick != (UINT)-1)
        return cachedJoystick;

    if (!realJoyGetPosEx)
        return JOYSTICKID1;

    JOYINFOEX j;

    ZeroMemory(&j,sizeof(j));
    j.dwSize = sizeof(JOYINFOEX);
    j.dwFlags = JOY_RETURNALL;

    for(UINT i=0;i<16;i++) {

        if(realJoyGetPosEx(i,&j)==JOYERR_NOERROR) {

            cachedJoystick = i;
            return i;

        }

    }

    return JOYSTICKID1;
}

extern "C" MMRESULT WINAPI joyGetPosEx(UINT uJoyID, LPJOYINFOEX pji) {

    if(!realJoyGetPosEx)
        return MMSYSERR_ERROR;

    UINT realID = (uJoyID == JOYSTICKID1) ? FindActiveJoystick() : uJoyID;

    return realJoyGetPosEx(realID,pji);
}

extern "C" MMRESULT WINAPI joyGetDevCapsA(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc) {

    if(!realJoyGetDevCapsA)
        return MMSYSERR_ERROR;

    MMRESULT r = realJoyGetDevCapsA(FindActiveJoystick(),pjc,cbjc);

    if(r == JOYERR_NOERROR && pjc) {

        strncpy_s(pjc->szPname,sizeof(pjc->szPname),"USB Gamepad (Chicago Patch)",_TRUNCATE);

    }

    return r;
}

extern "C" UINT WINAPI joyGetNumDevs() {

    return 16;

}

void LoadOriginalWinMM() {

    char path[MAX_PATH];

    GetSystemDirectoryA(path,MAX_PATH);

    strcat_s(path,"\\winmm.dll");

    realWinmm = LoadLibraryA(path);

    if(realWinmm) {

        realJoyGetPosEx =
        (JOYGETPOSEX_PROC)GetProcAddress(realWinmm,"joyGetPosEx");

        realJoyGetDevCapsA =
        (JOYGETDEVCAPSA_PROC)GetProcAddress(realWinmm,"joyGetDevCapsA");

        realJoyGetNumDevs =
        (JOYGETNUMDEVS_PROC)GetProcAddress(realWinmm,"joyGetNumDevs");

    }

}

BOOL WINAPI DllMain(HINSTANCE inst,DWORD reason,LPVOID) {

    if(reason == DLL_PROCESS_ATTACH) {

        LoadOriginalWinMM();

    }

    return TRUE;

}
