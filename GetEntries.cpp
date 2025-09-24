#include "GetEntries.h"

std::vector<std::string> get_new_entries() {
    std::string path = GetPathVariable();
    return SplitPath(path);
}
