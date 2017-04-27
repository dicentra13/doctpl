#include <doctpl/template.h>

namespace doctpl {

class Template::Impl {
public:
    Impl(Template* doc, const QString& filename)
        : filename(filename)
        , layout(new Layout(doc))
        , fields(std::make_shared<Template::Fields>())
    {}

    QString filename;
    std::unique_ptr<Layout> layout;
    FormattingPtr defaultFormatting;
    Template::FieldsPtr fields;
};

Template::Template()
    : impl_(new Impl(this, ""))
{}

Template::Template(const QString& filename)
    : impl_(new Impl(this, filename))
{}

Template::~Template() {}

const FormattingPtr& Template::defaultFormatting() const
{
    return impl_->defaultFormatting;
}

void Template::setDefaultFormatting(const FormattingPtr& formatting)
{
    ASSERT(formatting);
    impl_->defaultFormatting = formatting;
}

const Template::FieldsPtr& Template::fields() const { return impl_->fields; }

Layout* Template::layout() const { return impl_->layout.get(); }

const QString& Template::filename() const { return impl_->filename; }

void Template::setFilename(const QString& filename) { impl_->filename = filename; }

void Template::print(
    const QString& filename)
{
    impl_->layout->print(filename);
}

void Template::print(
    const QString& filename,
    const std::set<Layout::Index>& pages)
{
    impl_->layout->print(filename, pages);
}

} // namespace doctpl
