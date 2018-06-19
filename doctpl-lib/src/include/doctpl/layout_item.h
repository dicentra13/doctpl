#pragma once

#include <doctpl/style_options.h>

#include <QtGui>
#include <QGraphicsItem>

#include <memory>

namespace doctpl {

class Layout;

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

    const StylePtr& style() const;
    void setStyle(StylePtr style);

protected:
    LayoutItem(const Layout* layout, const QSizeF& size, StylePtr style);

    const Layout* layout() const;

    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace doctpl
