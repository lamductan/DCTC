#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include <string>

class Logger {
private:
    std::string filename_;

public:
    Logger();
    Logger(const std::string& filename);
    std::string getFilename() const;
    void set(const std::string& filename, const char* mode="w");
    void append(const char* format, ...) const;
    void write(const char* format, ...) const;
};

#endif //LOGGER_LOGGER_H
