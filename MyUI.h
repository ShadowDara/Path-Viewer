#pragma once
#include "imgui.h"
#include "PathUtils.h"

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

    ImGui::Text("PATH-Einträge:");
    ImGui::Separator();

    // Child für die Scrollable-Liste
    ImGui::BeginChild("PathEntries", ImVec2(0, 650), true, ImGuiWindowFlags_HorizontalScrollbar);

    static std::vector<std::string> editableEntries = entries;

    if (editableEntries.size() != entries.size())
        editableEntries = entries;

    for (size_t i = 0; i < editableEntries.size(); ++i)
    {
        std::string button_label = "Delete##" + std::to_string(i);
        if (ImGui::Button(button_label.c_str()))
        {
            editableEntries.erase(editableEntries.begin() + i);
            --i;
        }

        ImGui::SameLine();

        char buffer[4096];
        strncpy_s(buffer, sizeof(buffer), editableEntries[i].c_str(), _TRUNCATE);
        ImGui::InputText(("Entry " + std::to_string(i)).c_str(), buffer, sizeof(buffer));
        editableEntries[i] = buffer;
    }

    ImGui::EndChild(); // PathEntries

    // Buttons unterhalb der Liste
    if (ImGui::Button("Add Path")) {
        editableEntries.push_back("");
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::SameLine();

    if (ImGui::Button("Copy PATH"))
    {
#ifdef _WIN32
        char delimiter = ';';
#else
        char delimiter = ':';
#endif
        std::string newPath;
        for (size_t i = 0; i < editableEntries.size(); ++i)
        {
            newPath += editableEntries[i];
            if (i + 1 < editableEntries.size())
                newPath += delimiter;
        }
        ImGui::SetClipboardText(newPath.c_str());
    }

    ImGui::SameLine();

    if (ImGui::Button("Save Paths"))
    {
        std::cout << "INFO: PATH variable copied to clipboard.\n";
    }

    ImGui::SameLine();

    if (ImGui::Button("INFO"))
    {
		std::cout << "INFO: PATH variable copied to clipboard.\n";
    }

    // Entry Ende
    ImGui::EndChild(); // Content
    ImGui::End();      // PATH Viewer
}
