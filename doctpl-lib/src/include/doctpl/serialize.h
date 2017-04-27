#pragma once

#include <doctpl/exception.h>
#include <doctpl/template.h>
#include <doctpl/field.h>

#include <QDomElement>

#include <memory>
#include <functional>

namespace doctpl {

class XMLParseDOCTPL_ERROR : public Exception {
public:
    XMLParseDOCTPL_ERROR(size_t xmlLine, size_t xmlSymbol)
        : xmlLine_(xmlLine)
        , xmlSymbol_(xmlSymbol)
    {}

    size_t xmlLine() const {
        return xmlLine_;
    }

    size_t xmlSymbol() const {
        return xmlSymbol_;
    }

protected:
    size_t xmlLine_;
    size_t xmlSymbol_;
};

#define XML_REQUIRE(cond, line, symbol, msg) \
    for (bool f = true; !(cond) && f; f = false) { \
        throw XMLParseDOCTPL_ERROR(line, symbol) \
            << __FILE__ << ":" << __LINE__ << ": " << msg; \
    } \

#define XML_DOCTPL_ERROR(line, symbol, msg) \
    for (bool f = true; f; f = false) { \
        throw XMLParseDOCTPL_ERROR(line, symbol) \
            << __FILE__ << ":" << __LINE__ << ": " << msg; \
    } \

namespace xml {

// object factories

typedef std::function<Field*(const QDomElement&, Page*)> FieldFactory;

class Factory {
public:
    Factory();
    ~Factory();

    void add(const QString& type, const FieldFactory& factory);

    const FieldFactory& fieldFactory(const QString& type) const;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

typedef std::shared_ptr<Factory> FactoryPtr;

// returns factory singleton
FactoryPtr factory();


// reader
// objects constructed using factory

std::unique_ptr<Template> read(const QString& filename);

// writer

// void write(const Template& document);

} // namespace xml

} // namespace doctpl
