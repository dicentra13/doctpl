#include "settings.h"

std::map<QString, QString> Settings::settings_ = std::map<QString, QString>();
QFontDatabase *Settings::fontDB_ = 0;
QFontDatabase::WritingSystem Settings::script_ = QFontDatabase::Cyrillic;
