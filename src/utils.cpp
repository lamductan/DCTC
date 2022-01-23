#include <filesystem>
#include "utils.h"

std::string getParentPathAtLevel(const std::string& path, int level) {
    std::filesystem::path p = std::filesystem::absolute(path);
    for(int i = 0; i < level; ++i) {
        p = p.parent_path();
    }
    return p.string();
}