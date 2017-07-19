#ifndef TEMPLATELAYOUTITEM_H
#define TEMPLATELAYOUTITEM_H

#include <QtGui>
#include <QGraphicsItem>
#include <QPen>

/**
  QGraphicsItem is noncopyable, so is this class and his subclasses
  */
class TemplateLayoutItem : public QGraphicsItem {
public:
    virtual ~TemplateLayoutItem() {};

    virtual QRectF boundingRect() const {
        QPen p;
        p.setWidthF(1.0);
        double f = p.widthF();
        return QRectF(QPointF(-f / 2.0, -f/ 2.0),
            QSizeF(width_ + f, height_ + f));
    }

    virtual void setWidth(double width) {
        width_ = width;
    }
    double width() const {
        return width_;
    }

    virtual void setHeight(double height) {
        height_ = height;
    }
    double height() const {
        return height_;
    }
    virtual void setCurrent(bool isCurrent) {

        isCurrent_ = isCurrent;
    }
    bool isCurrent() const {
        return isCurrent_;
    }
protected:
    TemplateLayoutItem(double width, double height)
        : width_(width)
        , height_(height)
        , isCurrent_(false)
    {}

    double width_, height_;
    bool isCurrent_;
};

#endif // TEMPLATELAYOUTITEM_H
