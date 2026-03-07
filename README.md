# Chicago Input Patch

Chicago Input Patch is a small compatibility layer designed to fix joystick detection in legacy Windows 95 games.

Many early Windows games rely on the WinMM joystick API which expects hardware connected through the classic gameport interface. Modern USB controllers often appear in higher device slots or through HID translation layers, causing these games to fail detecting them.

This project provides a proxy `winmm.dll` that redirects joystick requests to the first active controller found on the system.

The goal is to restore compatibility with legacy titles without modifying the original game executables.

---

## Features

- Fixes joystick detection in many Windows 95 / DirectX 1 era games
- Works with USB gamepads and legacy devices
- No external software required
- Drop-in DLL replacement

---

## How it works

The patch replaces `winmm.dll` in the game directory.

When the game requests joystick data using the WinMM API, the proxy scans all possible joystick IDs and redirects the request to the first active controller found.

This bypasses the classic bug where games only check joystick IDs 0 or 1.

---

## Installation

1. Compile the project to produce `winmm.dll`
2. Copy `winmm.dll` into the game folder
3. Launch the game normally

## Compatibility

Designed for games using the legacy WinMM joystick interface.

Examples:

- Doom95

- MechWarrior 2

- Moto Racer

- Interstate 76

- many more games

---
Troubleshooting Doom 95 Crash
- Rename the original Windows winmm.dll (System32) to `winmm_real.dll` and copy it to the game folder.

- If it still fails, use Process Monitor (ProcMon) filtering for doom95.exe → look for "NAME NOT FOUND" in GetProcAddress for winmm.dll. Tell me which function to adjust.

## License

Open source. Contributions and improvements are welcome.

---

## Project Goal


This project aims to preserve compatibility between modern USB controllers and legacy Windows games originally designed for analog gameport hardware.

