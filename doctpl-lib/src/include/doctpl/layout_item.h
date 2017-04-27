#pragma once

#include <QtGui>
#include <QGraphicsItem>

#include <memory>

namespace doctpl {

/**
  QGraphicsItem is noncopyable, so is this class and his subclasses
  */

class LayoutItem : public QGraphicsItem {
public:
    virtual ~LayoutItem();

    virtual QRectF boundingRect() const;

    virtual void setWidth(double width);
    double width() const;

    virtual void setHeight(double height);
    double height() const;

protected:
    explicit LayoutItem(const QSizeF& size);

    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace doctpl
