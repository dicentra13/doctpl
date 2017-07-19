#ifndef PAGE_H
#define PAGE_H

#include "template/templatelayoutitem.h"
#include "fields/field.h"

#include <QList>
#include <QPrinter>

#include <list>

class QGraphicsSceneMouseEvent;
class QGraphicsRectItem;

class Page : public TemplateLayoutItem {
public:
    Page();
    // деструктор виртуальный
    // reimplementing 2 pure virtual QGraphicsItem functions
    Page *copy() {return new Page;} // без полей
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    bool addField(double xleft, double ytop, Field *field);
    bool deleteField(Field *field);
    static bool checkGeometryAvailable(Page *p, double width, double height,
        double dx, double dy);
    std::list<Field *> fields() const;
    virtual void setWidth(double width);
    virtual void setHeight(double height);
    void setDx(double dx);
    void setDy(double dy);
    double dx() const {
        return dx_;
    }
    double dy() const {
        return dy_;
    }

    void setNum(size_t i) {
        num_ = i;
    }
    size_t num() const {
        return num_;
    }
    // pos, x, y - из QGraphicsItem
    void setOrientation(QPrinter::Orientation o) {
        orientation_ = o;
    }
    QPrinter::Orientation orientation() const {
        return orientation_;
    }
protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
private:
    size_t num_;
    double dx_, dy_; // смещение при печати
    QPrinter::Orientation orientation_;
    QGraphicsRectItem *item_;
};

#endif // PAGE_H
