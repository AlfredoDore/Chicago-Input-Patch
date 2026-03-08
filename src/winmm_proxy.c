#include <windows.h>
#include <mmsystem.h>

static HMODULE realWinMM = NULL;

/* Ponteiros para funções reais */

typedef UINT (WINAPI *p_joyGetNumDevs)(void);
typedef MMRESULT (WINAPI *p_joyGetDevCapsA)(UINT, LPJOYCAPSA, UINT);
typedef MMRESULT (WINAPI *p_joyGetDevCapsW)(UINT, LPJOYCAPSW, UINT);
typedef MMRESULT (WINAPI *p_joyGetPos)(UINT, LPJOYINFO);
typedef MMRESULT (WINAPI *p_joyGetPosEx)(UINT, LPJOYINFOEX);

typedef DWORD (WINAPI *p_timeGetTime)(void);
typedef MMRESULT (WINAPI *p_timeBeginPeriod)(UINT);
typedef MMRESULT (WINAPI *p_timeEndPeriod)(UINT);
typedef MMRESULT (WINAPI *p_timeGetDevCaps)(LPTIMECAPS, UINT);

/* Variáveis globais */

static p_joyGetNumDevs real_joyGetNumDevs = NULL;
static p_joyGetDevCapsA real_joyGetDevCapsA = NULL;
static p_joyGetDevCapsW real_joyGetDevCapsW = NULL;
static p_joyGetPos real_joyGetPos = NULL;
static p_joyGetPosEx real_joyGetPosEx = NULL;

static p_timeGetTime real_timeGetTime = NULL;
static p_timeBeginPeriod real_timeBeginPeriod = NULL;
static p_timeEndPeriod real_timeEndPeriod = NULL;
static p_timeGetDevCaps real_timeGetDevCaps = NULL;


/* Carrega a winmm_real.dll */

static void LoadRealWinMM(void)
{
    if (realWinMM)
        return;

    realWinMM = LoadLibraryA("winmm_real.dll");

    if (!realWinMM)
    {
        MessageBoxA(
            NULL,
            "Chicago Input Patch\n\nwinmm_real.dll not found.",
            "WinMM Proxy Error",
            MB_ICONERROR
        );
        return;
    }

    real_joyGetNumDevs = (p_joyGetNumDevs)GetProcAddress(realWinMM, "joyGetNumDevs");
    real_joyGetDevCapsA = (p_joyGetDevCapsA)GetProcAddress(realWinMM, "joyGetDevCapsA");
    real_joyGetDevCapsW = (p_joyGetDevCapsW)GetProcAddress(realWinMM, "joyGetDevCapsW");
    real_joyGetPos = (p_joyGetPos)GetProcAddress(realWinMM, "joyGetPos");
    real_joyGetPosEx = (p_joyGetPosEx)GetProcAddress(realWinMM, "joyGetPosEx");

    real_timeGetTime = (p_timeGetTime)GetProcAddress(realWinMM, "timeGetTime");
    real_timeBeginPeriod = (p_timeBeginPeriod)GetProcAddress(realWinMM, "timeBeginPeriod");
    real_timeEndPeriod = (p_timeEndPeriod)GetProcAddress(realWinMM, "timeEndPeriod");
    real_timeGetDevCaps = (p_timeGetDevCaps)GetProcAddress(realWinMM, "timeGetDevCaps");
}


/* DLL Entry */

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        LoadRealWinMM();
    }

    return TRUE;
}


/* ========================= */
/* Joystick wrappers */
/* ========================= */

UINT WINAPI joyGetNumDevs(void)
{
    if (real_joyGetNumDevs)
        return real_joyGetNumDevs();

    return 0;
}


MMRESULT WINAPI joyGetDevCapsA(UINT id, LPJOYCAPSA caps, UINT size)
{
    if (real_joyGetDevCapsA)
        return real_joyGetDevCapsA(id, caps, size);

    return MMSYSERR_NODRIVER;
}


MMRESULT WINAPI joyGetDevCapsW(UINT id, LPJOYCAPSW caps, UINT size)
{
    if (real_joyGetDevCapsW)
        return real_joyGetDevCapsW(id, caps, size);

    return MMSYSERR_NODRIVER;
}


MMRESULT WINAPI joyGetPos(UINT id, LPJOYINFO info)
{
    if (real_joyGetPos)
        return real_joyGetPos(id, info);

    return MMSYSERR_NODRIVER;
}


MMRESULT WINAPI joyGetPosEx(UINT id, LPJOYINFOEX info)
{
    if (real_joyGetPosEx)
        return real_joyGetPosEx(id, info);

    return MMSYSERR_NODRIVER;
}


/* ========================= */
/* Timer wrappers */
/* ========================= */

DWORD WINAPI timeGetTime(void)
{
    if (real_timeGetTime)
        return real_timeGetTime();

    return GetTickCount();
}


MMRESULT WINAPI timeBeginPeriod(UINT period)
{
    if (real_timeBeginPeriod)
        return real_timeBeginPeriod(period);

    return TIMERR_NOERROR;
}


MMRESULT WINAPI timeEndPeriod(UINT period)
{
    if (real_timeEndPeriod)
        return real_timeEndPeriod(period);

    return TIMERR_NOERROR;
}


MMRESULT WINAPI timeGetDevCaps(LPTIMECAPS caps, UINT size)
{
    if (real_timeGetDevCaps)
        return real_timeGetDevCaps(caps, size);

    return MMSYSERR_NODRIVER;
}
    return MMSYSERR_NODRIVER;
}
