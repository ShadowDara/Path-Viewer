#pragma once
#include "imgui.h"
#include "PathUtils.h"

inline void RenderPathViewer()
{
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;      // Kein Titelbalken
    window_flags |= ImGuiWindowFlags_NoResize;        // Nicht skalierbar
    window_flags |= ImGuiWindowFlags_NoMove;          // Nicht verschiebbar
    window_flags |= ImGuiWindowFlags_NoScrollbar;     // Keine Scrollbars
    window_flags |= ImGuiWindowFlags_NoCollapse;      // Kein Minimieren

    ImGui::Begin("PATH Viewer", nullptr, window_flags);
    ImGui::BeginChild("Content", ImGui::GetContentRegionAvail(), false);

    ImGui::SetWindowFontScale(1.2f);

    std::string path = GetPathVariable();
    auto entries = SplitPath(path);

    ImGui::Text("PATH-Einträge:");

    ImGui::Separator();

    ImGui::BeginChild("PathEntries", ImVec2(1300, 400), true);

    // Wir speichern die Einträge temporär in einem char-Array für InputText
    static std::vector<std::string> editableEntries = entries;

    // Größe anpassen, falls PATH sich geändert hat
    if (editableEntries.size() != entries.size())
        editableEntries = entries;

    for (size_t i = 0; i < editableEntries.size(); ++i)
    {
        // char-Array für InputText
        char buffer[4096];
        // sichere Version von strncpy für Visual Studio
        strncpy_s(buffer, sizeof(buffer), editableEntries[i].c_str(), _TRUNCATE);

        ImGui::InputText(("Entry " + std::to_string(i)).c_str(), buffer, sizeof(buffer));

        ImGui::SameLine();

        if (ImGui::Button("Delete Path"))
        {
            std::cout << "Button 1 wurde geklickt!\n";
        }

        // Update zurück in den Vector
        editableEntries[i] = buffer;
    }

    ImGui::EndChild();

    if (ImGui::Button("Add Path"))
    {
        std::cout << "Button 1 wurde geklickt!\n";
    }

    ImGui::SameLine();

    if (ImGui::Button("copy Paths PATH"))
    {
        // Zusammensetzen und in die Zwischenablage
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

    ImGui::End();
}
