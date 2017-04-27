#pragma once

#include <exception>
#include <string>
#include <sstream>

namespace doctpl {

class Exception : public std::exception {
public:
    Exception() {}

    template <class T>
    const Exception& operator << (const T& val) const
    {
        std::ostringstream os;
        os << val;
        std::string& s = const_cast<std::string&>(s_);
        s += os.str();
        return *this;
    }

    virtual const char* what() const noexcept
    {
        return s_.c_str();
    }

private:
    std::string s_;
};

} // namespace doctpl

#define REQUIRE(cond, msg) \
    for (bool f = true; !(cond) && f; f = false) { \
        throw doctpl::Exception() << __FILE__ << ":" << __LINE__ << ": " << msg; \
    } \

#define DOCTPL_ERROR(msg) \
    for (bool f = true; f; f = false) { \
        throw doctpl::Exception() << __FILE__ << ":" << __LINE__ << ": " << msg; \
    } \

#define ASSERT(cond) \
    for (bool f = true; !(cond) && f; f = false) { \
        throw doctpl::Exception() \
            << __FILE__ \
            << ":" << __LINE__ \
            << ": " << " assertion " << #cond << " failed"; \
    } \

