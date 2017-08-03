#include <doctpl/settings.h>

namespace doctpl {

class Settings::Impl {
public:
    explicit Impl(QFontDatabase::WritingSystem script)
        : fontDB(new QFontDatabase())
        , script(script)
    {}

    std::unique_ptr<QFontDatabase> fontDB;
    QFontDatabase::WritingSystem script;
};

Settings::Settings()
    : impl_(new Impl(QFontDatabase::Cyrillic)) // FIXME
{}

Settings::~Settings() {}

const QFontDatabase& Settings::fontDB() const { return *impl_->fontDB; }

QFontDatabase::WritingSystem Settings::script() const { return impl_->script; }

} // namespace doctpl
