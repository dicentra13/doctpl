#pragma once

#include <doctpl/settings.h>

#include <QString>
#include <QFontDatabase>

#include <map>

/**
 * Supported types:
 *   QString
 *   int
 *   double
 */

class Settings {
public:
    Settings();

    explicit Settings(const QString& filename);

    template <class T>
    T get(const QString& key) const;

    template <class T>
    void set(const QString& key, const T& value);

    doctpl::Settings& fontSettings() { return fontSettings_; }

private:
    void initDefaults();

    std::map<QString, QString> settings_;
    doctpl::Settings fontSettings_;
};

/// static singleton
Settings& settings();
