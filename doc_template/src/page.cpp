#include "page.h"

#include <QList>
#include <QPen>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QTransform>
#include <QGraphicsRectItem>

Page::Page()
    : TemplateLayoutItem(210.0, 297.0)
{
    dx_ = dy_ = 0.0;
    setFiltersChildEvents(false);
    item_ = new QGraphicsRectItem(QRectF(QPointF(0.0, 0.0),
        QSizeF(210.0, 297.0)), this);
    item_->setPen(Qt::NoPen);
    item_->setBrush(Qt::NoBrush);
    item_->setPos(0.0, 0.0);
}

bool Page::checkGeometryAvailable(Page *p, double width, double height,
    double dx, double dy)
{
    const QList<QGraphicsItem *> &fields = p->childItems();
    QList<QGraphicsItem *>::const_iterator i = fields.begin();
    bool result = true;
    while (result && i != fields.end()) {
        const TemplateLayoutItem *tli =
            dynamic_cast<const TemplateLayoutItem *>(*i);
        if (tli) {
            result = tli->x() + tli->width() - p->dx() + dx <= width
                && tli->y() + tli->height() - p->dy() + dy <= height;
        }
        i++;
    }
    return result;
}

// reimplementing  pure virtual QGraphicsItem functions

void Page::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/,
    QWidget */*widget*/)
{
    if (!Field::isFinalPrinting()) {
        painter->save();
        QPen cur(painter->pen());
        if (isCurrent_)
            cur.setColor(QColor(Qt::red));
        else
            cur.setColor(QColor(Qt::black));
        cur.setWidth(0);
        painter->setPen(cur);
        QBrush b(QColor(Qt::white));
        painter->setBrush(b);
        painter->drawRect(QRectF(QPointF(0.0, 0.0), QPointF(width_, height_)));
        painter->restore();
    }
}

bool Page::addField(double xleft, double ytop, Field *field) {
    field->setPage(this);
    field->setParentItem(item_);
    field->setPos(xleft, ytop);

    return true;
}

void Page::setWidth(double width) {
    width_ = width;
    item_->setRect(0.0, 0.0, width_, height_);
    update();
}

void Page::setHeight(double height) {
    height_ = height;
    item_->setRect(0.0, 0.0, width_, height_);
    update();
}

void Page::setDx(double dx) {
    dx_ = dx;
    item_->setPos(dx_, dy_);
    update();
}

std::list<Field *> Page::fields() const {
    std::list<Field *> fl;
    QList<QGraphicsItem *> items(item_->childItems());
    QList<QGraphicsItem *>::iterator i = items.begin();
    for (; i != items.end(); i++) {
        Field *f = dynamic_cast<Field *>(*i);
        if (f)
            fl.push_back(f);
    }
    return fl;
}

void Page::setDy(double dy) {
    dy_ = dy;
    item_->setPos(dx_, dy_);
    update();
}

bool Page::deleteField(Field *field) {
    field->setParentItem(0);
    return true;
}

void Page::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseDoubleClickEvent(event);
    this->setCurrent(true);
}
