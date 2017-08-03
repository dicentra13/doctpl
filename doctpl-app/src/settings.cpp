#include "settings.h"

#include <doctpl/exception.h>

Settings::Settings() {}

Settings::Settings(const QString& /*filename*/)
{
    REQUIRE(false, "not implemented");
}

void Settings::initDefaults()
{
    settings_.emplace("minPageWidth", "100");
    settings_.emplace("maxPageWidth", "297");
    settings_.emplace("minPageHeight", "100");
    settings_.emplace("maxPageHeight", "297");
    settings_.emplace("maxPageOffset", "100");
}

namespace {

using SettingsMap = std::map<QString, QString>;

QString getAsQString(const SettingsMap& vals, const QString& key)
{
    auto it = vals.find(key);
    REQUIRE(it != vals.end(),
        "Setting \"" << key.toStdString() << "\" not found");
    return it->second;
}

template <class T>
using Conv = std::function<T(const QString, bool&)>;

template <class T>
T getAs(
    const SettingsMap& vals,
    const QString& key,
    const Conv<T>& conv,
    const std::string& typeName)
{
    const auto& str = getAsQString(vals, key);
    bool ok;
    T v = conv(str, ok);
    REQUIRE(
        ok,
        "Setting \"" << key.toStdString()
            << "\" is not of type " << typeName);
    return v;
}

} // namespace

template <>
int Settings::get<int>(const QString& key) const
{
    Conv<int> conv = [] (const QString& s, bool& ok) {
        int v = s.toInt(&ok);
        return v;
    };

    return getAs<int>(settings_, key, conv, "int");
}

template <>
double Settings::get<double>(const QString& key) const
{
    Conv<double> conv = [] (const QString& s, bool& ok) {
        double v = s.toDouble(&ok);
        return v;
    };

    return getAs<double>(settings_, key, conv, "double");
}

template <>
QString Settings::get<QString>(const QString& key) const
{
    return getAsQString(settings_, key);
}


namespace {

template <class T>
void setAsNum(SettingsMap& vals, const QString& key, T v)
{
    QString vs;
    vs.setNum(v);
    vals[key] = vs;
}

} // namespace

template <>
void Settings::set<QString>(const QString& key, const QString& value)
{
    settings_[key] = value;
}

template <>
void Settings::set<int>(const QString& key, const int& value)
{
    setAsNum(settings_, key, value);
}

template <>
void Settings::set<double>(const QString& key, const double& value)
{
    setAsNum(settings_, key, value);
}


Settings& settings()
{
    static Settings s_settings;

    return s_settings;
}
