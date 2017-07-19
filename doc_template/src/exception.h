#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>
#include <sstream>

class Exception : public std::exception {
public:
    Exception(const std::string& file, size_t line, const std::string& what,
              const std::string& type = "Exception")
        : file_(file)
        , line_(line)
        , what_(what) {
        std::stringstream message;
        message << type << " thrown in "
                << file << ":" << line << " - " << what;
        message_ = message.str();
    }

    virtual const char* what() {
        return message_.c_str();
    }

    std::string cause() {
        return what_;
    }

    std::string file() const {
        return file_;
    }

    size_t line() const {
        return line_;
    }

    virtual ~Exception() throw() {}

private:
    std::string file_;
    size_t line_;
    std::string what_;

    std::string message_;
};

// TODO line number and file name here
#define THROW_EX(ExType, message) \
do { \
    std::stringstream what; \
    what << message; \
    throw ExType(__FILE__, __LINE__, what.str()); \
} while(true);

#endif // EXCEPTION_H
