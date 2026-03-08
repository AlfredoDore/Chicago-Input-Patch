#include <windows.h>
#include <mmsystem.h>
#include <string.h>

static HMODULE realWinMM;

typedef UINT (WINAPI *p_joyGetNumDevs)(void);
typedef MMRESULT (WINAPI *p_joyGetDevCapsA)(UINT, LPJOYCAPSA, UINT);
typedef MMRESULT (WINAPI *p_joyGetDevCapsW)(UINT, LPJOYCAPSW, UINT);
typedef MMRESULT (WINAPI *p_joyGetPos)(UINT, LPJOYINFO);
typedef MMRESULT (WINAPI *p_joyGetPosEx)(UINT, LPJOYINFOEX);
typedef MMRESULT (WINAPI *p_joyGetThreshold)(UINT, LPJOYTHRESHOLD);
typedef MMRESULT (WINAPI *p_joySetThreshold)(UINT, LPJOYTHRESHOLD);
typedef MMRESULT (WINAPI *p_joySetCapture)(HWND, UINT, UINT, BOOL);
typedef MMRESULT (WINAPI *p_joyReleaseCapture)(UINT);
typedef BOOL (WINAPI *p_joyConfigChanged)(DWORD);

typedef DWORD (WINAPI *p_timeGetTime)(void);
typedef MMRESULT (WINAPI *p_timeBeginPeriod)(UINT);
typedef MMRESULT (WINAPI *p_timeEndPeriod)(UINT);
typedef MMRESULT (WINAPI *p_timeGetDevCaps)(LPTIMECAPS, UINT);

static p_joyGetNumDevs real_joyGetNumDevs;
static p_joyGetDevCapsA real_joyGetDevCapsA;
static p_joyGetDevCapsW real_joyGetDevCapsW;
static p_joyGetPos real_joyGetPos;
static p_joyGetPosEx real_joyGetPosEx;
static p_joyGetThreshold real_joyGetThreshold;
static p_joySetThreshold real_joySetThreshold;
static p_joySetCapture real_joySetCapture;
static p_joyReleaseCapture real_joyReleaseCapture;
static p_joyConfigChanged real_joyConfigChanged;

static p_timeGetTime real_timeGetTime;
static p_timeBeginPeriod real_timeBeginPeriod;
static p_timeEndPeriod real_timeEndPeriod;
static p_timeGetDevCaps real_timeGetDevCaps;

static void load_real()
{
    if(realWinMM) return;

    realWinMM = LoadLibraryA("winmm_real.dll");

    real_joyGetNumDevs = (p_joyGetNumDevs)GetProcAddress(realWinMM,"joyGetNumDevs");
    real_joyGetDevCapsA = (p_joyGetDevCapsA)GetProcAddress(realWinMM,"joyGetDevCapsA");
    real_joyGetDevCapsW = (p_joyGetDevCapsW)GetProcAddress(realWinMM,"joyGetDevCapsW");
    real_joyGetPos = (p_joyGetPos)GetProcAddress(realWinMM,"joyGetPos");
    real_joyGetPosEx = (p_joyGetPosEx)GetProcAddress(realWinMM,"joyGetPosEx");
    real_joyGetThreshold = (p_joyGetThreshold)GetProcAddress(realWinMM,"joyGetThreshold");
    real_joySetThreshold = (p_joySetThreshold)GetProcAddress(realWinMM,"joySetThreshold");
    real_joySetCapture = (p_joySetCapture)GetProcAddress(realWinMM,"joySetCapture");
    real_joyReleaseCapture = (p_joyReleaseCapture)GetProcAddress(realWinMM,"joyReleaseCapture");
    real_joyConfigChanged = (p_joyConfigChanged)GetProcAddress(realWinMM,"joyConfigChanged");

    real_timeGetTime = (p_timeGetTime)GetProcAddress(realWinMM,"timeGetTime");
    real_timeBeginPeriod = (p_timeBeginPeriod)GetProcAddress(realWinMM,"timeBeginPeriod");
    real_timeEndPeriod = (p_timeEndPeriod)GetProcAddress(realWinMM,"timeEndPeriod");
    real_timeGetDevCaps = (p_timeGetDevCaps)GetProcAddress(realWinMM,"timeGetDevCaps");
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID reserved)
{
    if(reason == DLL_PROCESS_ATTACH)
        load_real();
    return TRUE;
}

UINT WINAPI joyGetNumDevs(void)
{
    load_real();
    if(real_joyGetNumDevs)
        return real_joyGetNumDevs();
    return 1;
}

MMRESULT WINAPI joyGetDevCapsA(UINT id, LPJOYCAPSA caps, UINT size)
{
    load_real();
    if(real_joyGetDevCapsA)
        return real_joyGetDevCapsA(id,caps,size);

    if(size >= sizeof(JOYCAPSA))
    {
        ZeroMemory(caps,sizeof(JOYCAPSA));
        strcpy(caps->szPname,"Chicago Input Patch");
        caps->wNumAxes=2;
        caps->wNumButtons=4;
        caps->wXmin=0;
        caps->wXmax=65535;
        caps->wYmin=0;
        caps->wYmax=65535;
        return MMSYSERR_NOERROR;
    }
    return MMSYSERR_INVALPARAM;
}

MMRESULT WINAPI joyGetDevCapsW(UINT id, LPJOYCAPSW caps, UINT size)
{
    load_real();
    if(real_joyGetDevCapsW)
        return real_joyGetDevCapsW(id,caps,size);
    return MMSYSERR_NODRIVER;
}

MMRESULT WINAPI joyGetPos(UINT id, LPJOYINFO info)
{
    load_real();
    if(real_joyGetPos)
        return real_joyGetPos(id,info);
    return MMSYSERR_NODRIVER;
}

MMRESULT WINAPI joyGetPosEx(UINT id, LPJOYINFOEX info)
{
    load_real();
    if(real_joyGetPosEx)
        return real_joyGetPosEx(id,info);

    if(info)
    {
        info->dwXpos=32768;
        info->dwYpos=32768;
        info->dwZpos=32768;
        info->dwButtons=0;
    }
    return MMSYSERR_NOERROR;
}

MMRESULT WINAPI joyGetThreshold(UINT id, LPJOYTHRESHOLD t)
{
    load_real();
    if(real_joyGetThreshold)
        return real_joyGetThreshold(id,t);
    return MMSYSERR_NOERROR;
}

MMRESULT WINAPI joySetThreshold(UINT id, LPJOYTHRESHOLD t)
{
    load_real();
    if(real_joySetThreshold)
        return real_joySetThreshold(id,t);
    return MMSYSERR_NOERROR;
}

MMRESULT WINAPI joySetCapture(HWND h, UINT id, UINT p, BOOL f)
{
    load_real();
    if(real_joySetCapture)
        return real_joySetCapture(h,id,p,f);
    return MMSYSERR_NOERROR;
}

MMRESULT WINAPI joyReleaseCapture(UINT id)
{
    load_real();
    if(real_joyReleaseCapture)
        return real_joyReleaseCapture(id);
    return MMSYSERR_NOERROR;
}

BOOL WINAPI joyConfigChanged(DWORD f)
{
    load_real();
    if(real_joyConfigChanged)
        return real_joyConfigChanged(f);
    return TRUE;
}

DWORD WINAPI timeGetTime(void)
{
    load_real();
    if(real_timeGetTime)
        return real_timeGetTime();
    return GetTickCount();
}

MMRESULT WINAPI timeBeginPeriod(UINT p)
{
    load_real();
    if(real_timeBeginPeriod)
        return real_timeBeginPeriod(p);
    return TIMERR_NOERROR;
}

MMRESULT WINAPI timeEndPeriod(UINT p)
{
    load_real();
    if(real_timeEndPeriod)
        return real_timeEndPeriod(p);
    return TIMERR_NOERROR;
}

MMRESULT WINAPI timeGetDevCaps(LPTIMECAPS caps, UINT size)
{
    load_real();
    if(real_timeGetDevCaps)
        return real_timeGetDevCaps(caps,size);
    return TIMERR_NOERROR;
}
