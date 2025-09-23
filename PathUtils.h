#pragma once
#include <string>
#include <vector>
#include <cstdlib>
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

// Setzt die PATH-Variable in der aktuellen Umgebung
inline bool WritePathVariable(const std::string& newPath)
{
    // Windows: Setzt die Umgebungsvariable für den aktuellen Prozess
    if (SetEnvironmentVariableA("PATH", newPath.c_str()))
        return true;
    else
    {
        std::cerr << "Error while Updating PATH: " << GetLastError() << std::endl;
        return false;
    }
}

void OpenFileInDefaultEditor(const std::string& filepath)
{
    ShellExecuteA(nullptr, "open", filepath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}
