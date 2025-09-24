#pragma once
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

inline std::string GetDownloadsPath()
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
inline std::string GetPathVariable()
{
    char* buffer = nullptr;
    size_t size = 0;
    if (_dupenv_s(&buffer, &size, "PATH") != 0 || !buffer)
        return "";
    std::string path(buffer);
    free(buffer);
    return path;
}

inline std::vector<std::string> SplitPath(const std::string& path)
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
