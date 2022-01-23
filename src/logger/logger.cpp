#include <stdarg.h>
#include <string.h>
#include <filesystem>

#include "utils.h"
#include "logger/logger.h"

Logger::Logger() { set("log.txt"); };

Logger::Logger(const std::string& filename) { set(filename); }

std::string Logger::getFilename() const {
    return filename_;
}

void Logger::set(const std::string& filename, const char* mode) {
    filename_ = filename;
    std::string parent_directory = getParentPathAtLevel(filename_, 1);
    std::filesystem::create_directories(parent_directory);
    FILE* f = fopen(filename_.c_str(), mode);
    fclose(f);
}

void Logger::append(const char* format, ...) const {
    FILE* f = fopen(filename_.c_str(), "a");
    va_list args;
    va_start(args, format);
    vfprintf(f, format, args);
    va_end(args);
    fclose(f);
}

void Logger::write(const char* format, ...) const {
    FILE* f = fopen(filename_.c_str(), "w");
    va_list args;
    va_start(args, format);
    vfprintf(f, format, args);
    va_end(args);
    fclose(f);
}