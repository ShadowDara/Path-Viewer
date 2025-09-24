#include "GetEntries.h"

std::vector<std::string> get_new_entries() {
    std::string path = GetUserPathVariable();
    return SplitPath(path);
}

std::vector<std::string> get_new_admin_entries() {
    //std::string path = GetAdminPathVariable();
    //return SplitPath(path);

    return {}; // Placeholder until implemented
}
