#ifndef TEMPLATELAYOUTEXCEPTIONS_H
#define TEMPLATELAYOUTEXCEPTIONS_H

#include "../exception.h"

class InvalidPageNumber : public Exception {
public:
    InvalidPageNumber(const std::string& file, size_t line,
                      const std::string& what)
        : Exception(file, line, what, "InvalidPageNumber") {};
};

class NonExistentFieldName : public Exception {
public:
    NonExistentFieldName(const std::string& file, size_t line,
                         const std::string& what)
        : Exception(file, line, what, "NonExistentFieldName") {};
};

class DuplicateFieldName : public Exception {
public:
    DuplicateFieldName(const std::string& file, size_t line,
                       const std::string& what)
        : Exception(file, line, what, "DuplicateFieldName") {};
};

class EmptyPageSet : public Exception {
public:
    EmptyPageSet(const std::string& file, size_t line,
                 const std::string& what)
        : Exception(file, line, what, "EmptyPageSet") {};
};

#endif // TEMPLATELAYOUTEXCEPTIONS_H
