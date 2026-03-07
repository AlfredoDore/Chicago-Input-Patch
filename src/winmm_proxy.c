#include <windows.h>
#include <mmsystem.h>

HMODULE realWinMM = NULL;

/* Ponteiros das funções reais */

typedef UINT (WINAPI *p_joyGetNumDevs)(void);
typedef MMRESULT (WINAPI *p_joyGetDevCapsA)(UINT, LPJOYCAPSA, UINT);
typedef MMRESULT (WINAPI *p_joyGetDevCapsW)(UINT, LPJOYCAPSW, UINT);
typedef MMRESULT (WINAPI *p_joyGetPos)(UINT, LPJOYINFO);
typedef MMRESULT (WINAPI *p_joyGetPosEx)(UINT, LPJOYINFOEX);
typedef MMRESULT (WINAPI *p_joyGetThreshold)(UINT, LPUINT);
typedef MMRESULT (WINAPI *p_joySetThreshold)(UINT, UINT);
typedef MMRESULT (WINAPI *p_joySetCapture)(HWND, UINT, UINT, BOOL);
typedef MMRESULT (WINAPI *p_joyReleaseCapture)(UINT);
typedef DWORD (WINAPI *p_joyConfigChanged)(DWORD);

typedef DWORD (WINAPI *p_timeGetTime)(void);
typedef MMRESULT (WINAPI *p_timeBeginPeriod)(UINT);
typedef MMRESULT (WINAPI *p_timeEndPeriod)(UINT);
typedef MMRESULT (WINAPI *p_timeGetDevCaps)(LPTIMECAPS, UINT);

/* Ponteiros */

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

/* Carregar winmm original */

void LoadRealWinMM()
{
    if(realWinMM) return;

    realWinMM = LoadLibraryA("winmm_real.dll");

    if(!realWinMM)
        realWinMM = LoadLibraryA("winmm.dll");

    if(!realWinMM)
        return;

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

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID reserved)
{
    if(reason == DLL_PROCESS_ATTACH)
        LoadRealWinMM();

    return TRUE;
}

/* JOYSTICK */

UINT WINAPI joyGetNumDevs(void)
{
    if(real_joyGetNumDevs)
        return real_joyGetNumDevs();

    return 1;
}

MMRESULT WINAPI joyGetDevCapsA(UINT id, LPJOYCAPSA caps, UINT size)
{
    if(real_joyGetDevCapsA)
        return real_joyGetDevCapsA(id,caps,size);

    return MMSYSERR_NODRIVER;
}

MMRESULT WINAPI joyGetDevCapsW(UINT id, LPJOYCAPSW caps, UINT size)
{
    if(real_joyGetDevCapsW)
        return real_joyGetDevCapsW(id,caps,size);

    return MMSYSERR_NODRIVER;
}

MMRESULT WINAPI joyGetPos(UINT id, LPJOYINFO info)
{
    if(real_joyGetPos)
        return real_joyGetPos(id,info);

    return MMSYSERR_NODRIVER;
}

MMRESULT WINAPI joyGetPosEx(UINT id, LPJOYINFOEX info)
{
    if(real_joyGetPosEx)
        return real_joyGetPosEx(id,info);

    return MMSYSERR_NODRIVER;
}

MMRESULT WINAPI joyGetThreshold(UINT id, LPUINT threshold)
{
    if(real_joyGetThreshold)
        return real_joyGetThreshold(id,threshold);

    return MMSYSERR_NODRIVER;
}

MMRESULT WINAPI joySetThreshold(UINT id, UINT threshold)
{
    if(real_joySetThreshold)
        return real_joySetThreshold(id,threshold);

    return MMSYSERR_NODRIVER;
}

MMRESULT WINAPI joySetCapture(HWND hwnd, UINT id, UINT period, BOOL changed)
{
    if(real_joySetCapture)
        return real_joySetCapture(hwnd,id,period,changed);

    return MMSYSERR_NODRIVER;
}

MMRESULT WINAPI joyReleaseCapture(UINT id)
{
    if(real_joyReleaseCapture)
        return real_joyReleaseCapture(id);

    return MMSYSERR_NODRIVER;
}

DWORD WINAPI joyConfigChanged(DWORD flags)
{
    if(real_joyConfigChanged)
        return real_joyConfigChanged(flags);

    return 0;
}

/* TIMER */

DWORD WINAPI timeGetTime(void)
{
    if(real_timeGetTime)
        return real_timeGetTime();

    return GetTickCount();
}

MMRESULT WINAPI timeBeginPeriod(UINT p)
{
    if(real_timeBeginPeriod)
        return real_timeBeginPeriod(p);

    return TIMERR_NOERROR;
}

MMRESULT WINAPI timeEndPeriod(UINT p)
{
    if(real_timeEndPeriod)
        return real_timeEndPeriod(p);

    return TIMERR_NOERROR;
}

MMRESULT WINAPI timeGetDevCaps(LPTIMECAPS caps, UINT size)
{
    if(real_timeGetDevCaps)
        return real_timeGetDevCaps(caps,size);

    return MMSYSERR_NODRIVER;
}
