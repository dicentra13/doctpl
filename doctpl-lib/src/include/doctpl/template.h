#pragma once

#include <doctpl/text_field.h>
#include <doctpl/table_field.h>
#include <doctpl/page.h>
#include <doctpl/fields_collection.h>
#include <doctpl/layout.h>

#include <QString>

#include <memory>
#include <set>

namespace doctpl {

class Template {
public:
    // new empty document
    Template();

    // associated with file @filename
    explicit Template(const QString& filename);

    ~Template();

    // default text formatting

    const FormattingPtr& defaultFormatting() const;
    void setDefaultFormatting(const FormattingPtr& formatting);

    // fields management

    typedef FieldsCollection<TextField, TableField> Fields;
    typedef std::shared_ptr<Fields> FieldsPtr;

    const FieldsPtr& fields() const;

    // layout

    Layout* layout() const;

    // associated file

    const QString& filename() const;
    void setFilename(const QString& filename);

    // PDF printing

    // processes all data sources for data extraction
    void print(
        const QString& filename);
    void print(
        const QString& filename,
        const std::set<Layout::Index>& pages);

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace doctpl
