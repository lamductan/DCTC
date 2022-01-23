#ifndef UTILS_H_
#define UTILS_H_

#include <string>

std::string getParentPathAtLevel(const std::string& path, int level);
const std::string PROJECT_ROOT_PATH = getParentPathAtLevel(__FILE__, 2);

#endif //UTILS_H_