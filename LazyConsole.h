#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>
#include <cstdio>

// Global variable to track console state
static bool consoleOpen = false;

// Function to open the console
void EnsureConsole()
{
    if (!consoleOpen)
    {
        AllocConsole(); // Konsole erzeugen
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        consoleOpen = true;
    }
}

// Function to close the console
void CloseConsole()
{
    if (consoleOpen)
    {
        FreeConsole(); // Trennt dein Programm vom Konsolenfenster
        consoleOpen = false;
    }
}
