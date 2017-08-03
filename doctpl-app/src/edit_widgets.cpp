#include "edit_widgets.h"

#include <QDoubleValidator>

namespace util {

DoubleLineEdit::DoubleLineEdit(
        double min,
        double max,
        int decimals,
        boost::optional<QString> defaultValue,
        QWidget* parent)
    : QLineEdit(parent)
    , defaultValue_(std::move(defaultValue))
{
    validator_ = new QDoubleValidator(min, max, decimals, this);
    setValidator(validator_);
}

bool DoubleLineEdit::valueHasChanged() const
{
    return !defaultValue_  || text() != *defaultValue_;
}

void DoubleLineEdit::focusOutEvent(QFocusEvent* event)
{
    QLineEdit::focusOutEvent(event);

    if (text().isEmpty() && defaultValue_) {
        setText(*defaultValue_);
    }
}

} // namespace util
