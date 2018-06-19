#pragma once

#include <QFontDatabase>

#include <memory>

namespace doctpl {

class Settings {
public:
    Settings();
    ~Settings();

    const QFontDatabase& fontDB() const;
    QFontDatabase::WritingSystem script() const;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace doctpl
