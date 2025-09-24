#pragma once
#include "imgui.h"
#include "PathUtils.h"

#include <fstream>
#include <string>
#include <iostream>

#include "LazyConsole.h"
#include "GetEntries.h"

#include <cstdlib>
#include <sstream>

inline void RenderPathViewer()
{
    // Fullscreen-Fenster Flags
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoScrollbar |
                                    ImGuiWindowFlags_NoCollapse;

    // Vollbild-Fenster setzen
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("PATH Viewer", nullptr, window_flags);

    ImGui::SetWindowFontScale(1.2f);

    // Child "Content" füllt das gesamte Fenster
    ImGui::BeginChild("Content", ImGui::GetContentRegionAvail(), false);

    std::string path = GetPathVariable();
    auto entries = SplitPath(path);

    // Title
    ImGui::Text("WINDOWS PATH ENTRYS:");
    ImGui::Separator();

    // Scrollbares Child für die Liste, Höhe dynamisch (volle Resthöhe)
    ImVec2 child_size = ImVec2(0, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing());
    ImGui::BeginChild("PathEntries", child_size, true, ImGuiWindowFlags_HorizontalScrollbar);

    static std::vector<std::string> editableEntries;

    // Nur beim ersten Frame initialisieren
    if (editableEntries.empty())
    {
        std::string path = GetPathVariable();
        editableEntries = SplitPath(path);
    }

    static bool scrollToEnd = false;

    // Zeichne alle InputText-Felder + Delete Buttons
    for (size_t i = 0; i < editableEntries.size(); ++i)
    {
        char buffer[4096];
        strncpy_s(buffer, sizeof(buffer), editableEntries[i].c_str(), _TRUNCATE);

        ImGui::InputText(("Entry " + std::to_string(i + 1)).c_str(), buffer, sizeof(buffer));
        editableEntries[i] = buffer;

        ImGui::SameLine();
        if (ImGui::Button(("Delete##" + std::to_string(i + 1)).c_str()))
        {
            editableEntries.erase(editableEntries.begin() + i);
            --i;
        }

        ImGui::SameLine();
        if (ImGui::Button(("Copy##" + std::to_string(i + 1)).c_str()))
        {
            ImGui::SetClipboardText(editableEntries[i].c_str());
        }

        ImGui::SameLine();
        if (ImGui::Button(("Select Folder##" + std::to_string(i + 1)).c_str()))
        {
            // Add soon
            // OpenFolderDialog(editableEntries[i]);
        }
    }

    // Scroll ans Ende, wenn neuer Eintrag hinzugefügt
    if (scrollToEnd)
    {
        ImGui::SetScrollHereY(1.0f);
        scrollToEnd = false;
    }

    ImGui::EndChild(); // PathEntries

    // Buttons **unterhalb** der scrollbaren Liste

    // Add new PATH Entry
    if (ImGui::Button("ADD PATH"))
    {
        editableEntries.push_back("");
        scrollToEnd = true; // Scroll ans neue Item
    }

    // Select Folder to Add
    ImGui::SameLine();
    if (ImGui::Button("SELECT FOLDER TO ADD"))
    {
        // OpenFolderDialog(editableEntries[i]);
    }

    // Copy all PATH to Clipboard
    ImGui::SameLine();
    if (ImGui::Button("COPY PATH"))
    {
        // Seperator für Windows PATH
        char delimiter = ';';

        std::string newPath;
        for (size_t i = 0; i < editableEntries.size(); ++i)
        {
            newPath += editableEntries[i];
            if (i + 1 < editableEntries.size())
                newPath += delimiter;
        }
        ImGui::SetClipboardText(newPath.c_str());
    }

    // Save PATH
    ImGui::SameLine();
    if (ImGui::Button("SAVE PATHS"))
    {
        std::string newPath;

        for (size_t i = 0; i < editableEntries.size(); ++i)
        {
            newPath += editableEntries[i];
            newPath += ";";
        }

        if (SetUserPath(newPath))
        {
            EnsureConsole();
            std::cout << "PATH updated successfully.\n";
        }
        else
        {
            EnsureConsole();
            std::cout << "Failed to update PATH.\n";
        }

        EnsureConsole();
        std::cout << "New PATH:\n"
                  << newPath << "\n";
        
        std::cout << "===============================\n\n";
        std::cout << " PATH updated successfully !!!\n\n";
        std::cout << "===============================";
    }

    // Reload PATH
    ImGui::SameLine();
    if (ImGui::Button("RELOAD PATHS"))
    {
        editableEntries = get_new_entries();
        std::cout << "Reloaded.\n";
    }

    // Dumb PATH to Console
    ImGui::SameLine();
    if (ImGui::Button("DUMB TO CONSOLE"))
    {
        std::string newPath;

        for (size_t i = 0; i < editableEntries.size(); ++i)
        {
            newPath += editableEntries[i];
            newPath += ";";
        }

        EnsureConsole();
        std::cout << "DUMB VARS TO CONSOLE:\n"
                  << newPath << "\n";
    }

    // Dumb PATH to File
    ImGui::SameLine();
    if (ImGui::Button("DUMB TO FILE"))
    {
        std::string filename = GetDownloadsPath();

        // Datei öffnen (erstellt, falls sie nicht existiert)
        std::ofstream file(filename);

        if (!file.is_open())
        {
            EnsureConsole();
            std::cerr << "Error while opening File!\n";
            return;
        }

        std::string newPath;
        for (size_t i = 0; i < editableEntries.size(); ++i)
        {
            newPath += editableEntries[i];
            newPath += ";";
        }

        file << newPath;

        file.close(); // Datei schließen

        EnsureConsole();
        std::cout << "Writed dumb File successfully!\n";
    }

    // Open DUMB File in Default Editor
    ImGui::SameLine();
    if (ImGui::Button("OPEN FILE"))
    {
        std::string filename = GetDownloadsPath();
        std::ifstream file(filename);

        if (file.good())
        {
            OpenFileInDefaultEditor(filename);
        }
        else
        {
            EnsureConsole();
            std::cout << "File does not exist.\n";
        }
    }

    // Info Button
    ImGui::SameLine();
    if (ImGui::Button("OPEN INFO"))
    {
        EnsureConsole();
        std::cout << "INFO PRESSED\n";
    }

    // OPEN CONSOLE OUTPUT
    ImGui::SameLine();
    if (ImGui::Button("OPEN CONSOLE OUTPUT"))
    {
        // Konsole nur jetzt erzeugen
        EnsureConsole();

        // Print
        std::cout << "WINDOWS PATH VIEWER\n";
    }

    // END
    ImGui::EndChild(); // Content
    ImGui::End();      // PATH Viewer
}
