#include "PathUtils.h"

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

// Function to open a file in the default editor
void OpenFileInDefaultEditor(const std::string& filepath) {
    ShellExecuteA(
        nullptr,        // hwnd
        "open",         // operation
        filepath.c_str(), // file to open
        nullptr,        // parameters
        nullptr,        // default directory
        SW_SHOWNORMAL   // show mode
    );
}

// Function to open a folder in Windows Explorer
void OpenFolderDialog(const std::string& path)
{
    // Convert std::string → wide string (UTF-16)
    std::wstring wPath(path.begin(), path.end());

    // Open the folder in Explorer
    ShellExecuteW(nullptr, L"open", wPath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
}

// Function to open a folder selection dialog and return the selected path
bool OpenFolderDialog(std::string& outPath)
{
    bool result = false;

    // COM initialisieren
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) return false;

    IFileDialog* pFileDialog = nullptr;
    hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
    if (SUCCEEDED(hr))
    {
        // Ordner-Auswahl aktivieren
        DWORD options;
        pFileDialog->GetOptions(&options);
        pFileDialog->SetOptions(options | FOS_PICKFOLDERS);

        // Dialog anzeigen
        hr = pFileDialog->Show(nullptr);
        if (SUCCEEDED(hr))
        {
            IShellItem* pItem;
            hr = pFileDialog->GetResult(&pItem);
            if (SUCCEEDED(hr))
            {
                PWSTR pszFilePath = nullptr;
                pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                if (pszFilePath)
                {
                    // wchar_t* → std::string
                    int size_needed = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, nullptr, 0, nullptr, nullptr);
                    outPath.resize(size_needed - 1);               // Platz für alle Zeichen ohne Nullterminator
                    WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, &outPath[0], size_needed, nullptr, nullptr);

                    CoTaskMemFree(pszFilePath);
                    result = true;
                }

                pItem->Release();
            }
        }
        pFileDialog->Release();
    }

    CoUninitialize();
    return result;
}
