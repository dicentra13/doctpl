#ifndef TEMPLATESOURCESEXCEPTIONS_H
#define TEMPLATESOURCESEXCEPTIONS_H

#include <exception>

class NonExistentSourceName : public std::exception {
public:
    NonExistentSourceName() {};
    ~NonExistentSourceName() {};
};

class DuplicateSourceName : public std::exception {
public:
    DuplicateSourceName() {};
    ~DuplicateSourceName() {};
};

#endif // TEMPLATESOURCESEXCEPTIONS_H
