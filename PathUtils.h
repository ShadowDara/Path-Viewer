#pragma once
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <shobjidl.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>

std::string GetDownloadsPath();

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

std::string GetUserPathVariable();

std::string GetAdminPathVariable();

std::vector<std::string> SplitPath(const std::string& path);

// Update Path Variables for the Logged User
bool SetUserPath(const std::string& newPath);

void OpenFileInDefaultEditor(const std::string& filepath);

void OpenFolderDialog(const std::string& outPath);

bool OpenFolderDialog(std::string& outPath);
