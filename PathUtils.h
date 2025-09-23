#pragma once
#include <string>
#include <vector>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h>
#endif

inline std::string GetPathVariable()
{
    #ifdef _WIN32
    char* buffer = nullptr;
    size_t size = 0;
    _dupenv_s(&buffer, &size, "PATH");
    std::string path = buffer ? buffer : "";
    free(buffer);
    return path;
    #else
    const char* path = std::getenv("PATH");
    return path ? std::string(path) : "";
    #endif
}

inline std::vector<std::string> SplitPath(const std::string& path)
{
    std::vector<std::string> entries;
    #ifdef _WIN32
    char delimiter = ';';
    #else
    char delimiter = ':';
    #endif
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
