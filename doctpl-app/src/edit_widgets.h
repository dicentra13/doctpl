#pragma once

#include <QLineEdit>

#include <boost/optional.hpp>

class QDoubleValidator;

namespace util {

class DoubleLineEdit : public QLineEdit {
public:
    DoubleLineEdit(
        double min,
        double max,
        int decimals,
        boost::optional<QString> defaultValue,
        QWidget* parent);

    bool valueHasChanged() const;

protected:
    void focusOutEvent(QFocusEvent* event);

private:
    QDoubleValidator* validator_;
    boost::optional<QString> defaultValue_;
};

} // namespace util
