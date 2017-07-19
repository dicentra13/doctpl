#include "field.h"

#include <QPen>

bool Field::isFinalPrinting_ = false;

// ссылка на страницу просто хранится
// поле в страницу добавляется шаблоном (Template)
Field::Field(const QString &type, double width, double height)
    : TemplateLayoutItem(width, height)
    , type_(type)
    , page_(0)
{
    //this->setHandlesChildEvents(true);
}


