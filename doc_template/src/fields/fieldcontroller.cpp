#include "fieldcontroller.h"

#include "../page.h"
#include "../template/templatelayout.h"
#include "fieldsettingswidget.h"

//#include <QWidget>
#include <QString>

FieldController::FieldController(const QString &type)
    : QObject(0)
    , templateLayout_(0)
{
    type_ = type;
}

void FieldController::nameChanged(const QString &name) {
    if (currentField() && name != currentField()->name()) {
        QString oldName = currentField()->name();
        if (!templateLayout_->renameField(oldName, name)) {
            // обратно старое имя, если оно поменялось в виджете
            settingsWidget()->setName(oldName);
        } else {
            renameField(oldName, name); // в реестре fields_ контроллеров
            emit currentFieldNameChanged(name);
        }
    }
}

void FieldController::pageChanged(size_t number) {
    if (currentField() && currentField()->page()->num() != number) {
        if (!templateLayout_->moveField(currentField()->name(), number)) {
            // обратно страницу
            settingsWidget()->setPageNumber(currentField()->page()->num());
        }
    }
}

void FieldController::xPosChanged(double x) {
    Field *f = currentField();
    if (f) {
        if (x != f->x() && x >= 0.0 && x <= f->page()->width() &&
            x + f->width() >= 0.0 && x + f->width() <= f->page()->width())
        {
            f->setX(x);
            f->update();
            templateLayout_->setModified(true);
        }
        else
            settingsWidget()->setX(f->x());
    }
}

void FieldController::yPosChanged(double y) {
    Field *f = currentField();
    if (f) {
        if (y != f->y() && y >= 0.0 && y <= f->page()->height() &&
            y + f->height() >= 0.0 && y + f->height() <= f->page()->height())
        {
            f->setY(y);
            f->update();
            templateLayout_->setModified(true);
        }
        else
            settingsWidget()->setY(f->y());
    }
}

void FieldController::widthChanged(double w) {
    Field *f = currentField();
    if (f) {
        if (w != f->width() && w > 0.0 && w < f->page()->width() &&
            f->x() + w >= 0.0 && f->x() + w <= f->page()->width())
        {
            f->setWidth(w);
            f->page()->update();
            templateLayout_->setModified(true);
        }
        else
            settingsWidget()->setWidth(f->width());
    }
}

void FieldController::heightChanged(double h) {
    Field *f = currentField();
    if (f) {
        if (h != f->height() && h > 0.0 && h < f->page()->height() &&
            f->y() + h >= 0.0 && f->y() + h <= f->page()->height())
        {
            f->setHeight(h);
            f->page()->update();
            templateLayout_->setModified(true);
        }
        else
            settingsWidget()->setHeight(f->height());
    }
}

bool FieldController::checkGeometry(size_t pageNum, double x, double y,
    double width, double height)
{
    Page *p = templateLayout_->page(pageNum);
    if (!p ||
        x < 0.0 || x > p->width() ||
        y < 0.0 || y > p->height() ||
        x + width < 0.0 || x + width > p->width() ||
        y + height < 0.0 || y + height > p->height())
    {
        return false;
    } else
        return true;
}

bool FieldController::checkName(const QString &name) {
    // true если еще не использовано
    return templateLayout_->field(name) == 0;
}

void FieldController::loadDataToWidget() {
    settingsWidget()->setName(currentField()->name());
    settingsWidget()->setPageNumber(currentField()->page()->num());
    settingsWidget()->setX(currentField()->x());
    settingsWidget()->setY(currentField()->y());
    settingsWidget()->setWidth(currentField()->width());
    settingsWidget()->setHeight(currentField()->height());
}

void FieldController::updatePages(size_t count) {
    settingsWidget()->setPages(count);
    if (currentField()) {
        settingsWidget()->setPageNumber(currentField()->page()->num());
    }
}
