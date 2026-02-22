#include "GetEntries.h"

// Get the user PATH entries
std::vector<std::string> get_new_entries() {
    std::string path = GetUserPathVariable();
    return SplitPath(path);
}

// Get the admin PATH entries
std::vector<std::string> get_new_admin_entries() {
    std::string path = GetAdminPathVariable();
    return SplitPath(path);
}
