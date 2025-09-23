#include <windows.h>
#include <iostream>
#include <cstdio>

inline void EnsureConsole()
{
    static bool consoleOpen = false;
    if (!consoleOpen)
    {
        AllocConsole(); // Konsole erzeugen
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        consoleOpen = true;
    }
}
