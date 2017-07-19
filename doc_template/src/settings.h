#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QFontDatabase>

#include <map>

class Settings {
public:
    static QString get(const QString &name) {
        std::map<QString, QString>::iterator i =
            Settings::settings_.find(name);
        if (i != Settings::settings_.end())
            return i->second;
        else
            return "";
    }
    static bool readFromFile(const QString &/*fileName*/) {
        // FIXME временная мера
        Settings::settings_.insert(std::pair<QString, QString>(
                "minPageWidth", "100"));
        Settings::settings_.insert(std::pair<QString, QString>(
                "maxPageWidth", "297"));
        Settings::settings_.insert(std::pair<QString, QString>(
                "minPageHeight", "100"));
        Settings::settings_.insert(std::pair<QString, QString>(
                "maxPageHeight", "297"));
        Settings::settings_.insert(std::pair<QString, QString>(
                "maxPageOffset", "100"));
        Settings::script_ = QFontDatabase::Cyrillic;
        return true;
    }
    static QFontDatabase *fontDB() {
        if (!fontDB_)
            fontDB_ = new QFontDatabase;
        return fontDB_;
    }
    static QFontDatabase::WritingSystem script() {
        return Settings::script_;
    }

private:
    static std::map<QString, QString> settings_;
    static QFontDatabase *fontDB_;
    static QFontDatabase::WritingSystem script_;
};

#endif // SETTINGS_H
