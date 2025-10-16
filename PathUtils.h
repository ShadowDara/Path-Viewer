#pragma once
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

std::string GetDownloadsPath()
{
    char* buffer = nullptr;
    size_t size = 0;
    if (_dupenv_s(&buffer, &size, "USERPROFILE") != 0 || !buffer)
        return ""; // Fehler
    std::string downloadsPath = std::string(buffer) + "\\Downloads\\PATH-VAR-DUMB-FILE.txt";
    free(buffer);
    return downloadsPath;
}

// ---- Get PATH variable ----
/*
std::string GetPathVariable()
{
    char* buffer = nullptr;
    size_t size = 0;
    if (_dupenv_s(&buffer, &size, "PATH") != 0 || !buffer)
        return "";
    std::string path(buffer);
    free(buffer);
    return path;
}
*/

std::string GetUserPathVariable()
{
    HKEY hKey;
    const char* subkey = "Environment";
    char value[32767]; // Max registry value length for environment vars
    DWORD value_length = sizeof(value);
    DWORD type = 0;

    // Open the key for the current user
    if (RegOpenKeyExA(HKEY_CURRENT_USER, subkey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
        return "";

    // Query the PATH value
    if (RegQueryValueExA(hKey, "Path", nullptr, &type, reinterpret_cast<LPBYTE>(value), &value_length) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        return "";
    }

    RegCloseKey(hKey);

    // Ensure the value is a string
    if (type != REG_SZ && type != REG_EXPAND_SZ)
        return "";

    return std::string(value);
}

std::string GetAdminPathVariable()
{
    HKEY hKey;
    const char* subkey = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
    char value[32767]; // Max registry value length for environment vars
    DWORD value_length = sizeof(value);
    DWORD type = 0;

    // Open the key for the local machine (admin)
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subkey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
        return "";

    // Query the PATH value
    if (RegQueryValueExA(hKey, "Path", nullptr, &type, reinterpret_cast<LPBYTE>(value), &value_length) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        return "";
    }

    RegCloseKey(hKey);

    // Ensure the value is a string
    if (type != REG_SZ && type != REG_EXPAND_SZ)
        return "";

    return std::string(value);
}

std::vector<std::string> SplitPath(const std::string& path)
{
    std::vector<std::string> entries;

	// Only Windows uses ';' as delimiter
    char delimiter = ';';

    std::string temp;
    for (char c : path)
    {
        if (c == delimiter)
        {
            if (!temp.empty())
                entries.push_back(temp);
            temp.clear();
        }
        else temp += c;
    }
    if (!temp.empty())
        entries.push_back(temp);
    return entries;
}

// Update Path Variables for the Logged User
bool SetUserPath(const std::string& newPath)
{
    HKEY hKey;
    LONG result = RegOpenKeyExA(HKEY_CURRENT_USER, "Environment", 0, KEY_SET_VALUE, &hKey);
    if (result != ERROR_SUCCESS)
    {
        std::cerr << "Failed to open registry key: " << result << std::endl;
        return false;
    }

    result = RegSetValueExA(hKey, "PATH", 0, REG_EXPAND_SZ, (const BYTE*)newPath.c_str(), (DWORD)(newPath.size() + 1));
    RegCloseKey(hKey);

    if (result != ERROR_SUCCESS)
    {
        std::cerr << "Failed to set PATH in registry: " << result << std::endl;
        return false;
    }

    // Broadcast Änderung an das System (für laufende Prozesse)
    SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0,
                        (LPARAM)"Environment", SMTO_ABORTIFHUNG, 5000, nullptr);

    return true;
}

void OpenFileInDefaultEditor(const std::string& filepath)
{
    ShellExecuteA(nullptr, "open", filepath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}
