#pragma once

#include "PathUtils.h"
#include <string>
#include <vector>

// Nur Deklarationen — keine .cpp-Dateien inkludieren!

// Get the user PATH entries
std::vector<std::string> get_new_entries();

// Get the admin PATH entries
std::vector<std::string> get_new_admin_entries();
