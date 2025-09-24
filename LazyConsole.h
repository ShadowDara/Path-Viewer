#include <windows.h>
#include <iostream>
#include <cstdio>

// Function to open the console
void EnsureConsole()
{
    // Var to check if the Console Window is opened
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
