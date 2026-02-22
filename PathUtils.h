#pragma once

#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <shobjidl.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>

// Function to get the Downloads folder path for the current user
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

// Get User and Admin PATH variables separately (Windows-specific)
std::string GetUserPathVariable();
std::string GetAdminPathVariable();

// Function to split a PATH variable into individual entries
std::vector<std::string> SplitPath(const std::string& path);

// Update Path Variables for the Logged User
bool SetUserPath(const std::string& newPath);

// Function to open a file in the default editor
void OpenFileInDefaultEditor(const std::string& filepath);

// Function to open a folder in Windows Explorer
void OpenFolderDialog(const std::string& outPath);

// Function to open a folder selection dialog and return the selected path
bool OpenFolderDialog(std::string& outPath);
