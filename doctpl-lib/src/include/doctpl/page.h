#pragma once

#include <doctpl/layout_item.h>

#include <QList>
#include <QPrinter>
#include <QSizeF>
#include <QPointF>

#include <boost/optional.hpp>

#include <list>
#include <memory>

namespace doctpl {

class Layout;
class Field;

class Page : public LayoutItem {
public:
    struct Offset {
        // TODO explicit
        Offset(double dx = 0.0, double dy = 0.0) : dx(dx), dy(dy) {}

        double dx;
        double dy;
    };

    struct InitData {
        InitData(const QSizeF& size, StylePtr style, const Offset& offset = Offset())
            : size(size)
            , style(std::move(style))
            , offset(offset)
        {}

        QSizeF size;
        StylePtr style;
        Offset offset;
    };

    Page(InitData data, Layout* layout);

    virtual ~Page();

    // from QGraphicsItem
    virtual void paint(
        QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget = 0);

    Layout* layout() const;

    // fields management

    QGraphicsItem* fieldsParent() const;

    // removes field from its old page if necessary
    void addField(Field* field, const QPointF& pos);

    // deletes field
    void deleteField(Field* field);

    std::list<Field*> fields() const;

    // geometry

    virtual void setWidth(double width);
    virtual void setHeight(double height);

    // printing offset

    void setDx(double dx);
    double dx() const;

    void setDy(double dy);
    double dy() const;

    void setOrientation(QPrinter::Orientation o);
    QPrinter::Orientation orientation() const;

    // geom validation
    static bool checkGeometryAvailable(
        const Page& p,
        const QSizeF& size,
        double dx, double dy);

    typedef boost::optional<std::pair<double, double> > DimensionDiff;

    struct SizeDiff {
        DimensionDiff width;
        DimensionDiff height;
    };

protected:
    //virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace doctpl
