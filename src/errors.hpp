#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <string>
#include <map>

#define ENUM_ERROR_LEVELS \
    X(Warning) \
    X(Error) \
    X(FatalError) \
    X(CriticalError) \
    X(SystemError)


enum class ErrorLevel : int {
    #define X(name) name,
    ENUM_ERROR_LEVELS
    #undef X
};

const std::map<ErrorLevel, std::string> error_level_names = {
    #define X(name) { ErrorLevel::name, #name },
    ENUM_ERROR_LEVELS
    #undef X
};

void error(const std::string &error_text, ErrorLevel error_level = ErrorLevel::FatalError, const std::string &ctx = "");


#endif // ERRORS_HPP
