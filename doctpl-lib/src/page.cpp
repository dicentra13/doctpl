#include <doctpl/page.h>

#include <doctpl/field.h>
#include <doctpl/layout.h>
#include <doctpl/exception.h>

namespace doctpl {

class Page::Impl {
public:
    Impl(
            const QSizeF& size,
            double dx,
            double dy,
            Page* page,
            Layout* layout)
        : layout(layout)
    {
        ASSERT(page);
        ASSERT(layout);
        item = new QGraphicsRectItem(
            QRectF(QPointF(0.0, 0.0), size),
            /* parent = */ page);
        item->setPen(Qt::NoPen);
        item->setBrush(Qt::NoBrush);
        item->setPos(dx, dy);
    }

    void setWidth(double width)
    {
        item->setRect(0.0, 0.0, width, item->rect().height());
    }

    void setHeight(double height)
    {
        item->setRect(0.0, 0.0, item->rect().width(), height);
    }

    void setDx(double dx)
    {
        item->setX(dx);
    }

    void setDy(double dy)
    {
        item->setY(dy);
    }

    QGraphicsRectItem* item;
    Layout* layout;
};


Page::Page(const QSizeF& size, Layout* layout)
    : LayoutItem(size)
    , impl_(new Impl(size, 0.0, 0.0, this, layout))
{}

Page::Page(const QSizeF& size, double dx, double dy, Layout* layout)
    : LayoutItem(size)
    , impl_(new Impl(size, dx, dy, this, layout))
{}

Page::~Page() {}

QGraphicsItem* Page::fieldsParent() const { return impl_->item; }

Layout* Page::layout() const { return impl_->layout; }

void Page::paint(
    QPainter* /*painter*/,
    const QStyleOptionGraphicsItem* /*option*/,
    QWidget* /*widget*/)
{
//    painter->save();
//    QPen cur(painter->pen());
//    if (isCurrent_)
//        cur.setColor(QColor(Qt::red));
//    else
//        cur.setColor(QColor(Qt::black));
//    cur.setWidth(0);
//    painter->setPen(cur);
//    QBrush b(QColor(Qt::white));
//    painter->setBrush(b);
//    painter->drawRect(QRectF(QPointF(0.0, 0.0), QPointF(width_, height_)));
//    painter->restore();
}

void Page::addField(Field* field, const QPointF& pos)
{
    if (field->parentItem() != fieldsParent()) {
        field->setParentItem(fieldsParent());
    }
    field->setPos(pos);
}

void Page::deleteField(Field* field)
{
    ASSERT(field->parentItem() == fieldsParent());
    field->setParentItem(nullptr);
    delete field;
}

std::list<Field*> Page::fields() const
{
    std::list<Field*> fl;
    QList<QGraphicsItem*> items(impl_->item->childItems());
    for (auto i = items.begin(); i != items.end(); ++i) {
        Field* f = dynamic_cast<Field*>(*i);
        if (f) {
            fl.push_back(f);
        }
    }
    return fl;
}

void Page::setWidth(double width)
{
    const double old = this->width();
    LayoutItem::setWidth(width);
    impl_->setWidth(width);
    layout()->onPageResize(this, SizeDiff{{{old, width}}, boost::none});
    update();
}

void Page::setHeight(double height)
{
    const double old = this->height();
    LayoutItem::setHeight(height);
    impl_->setHeight(height);
    layout()->onPageResize(this, SizeDiff{boost::none, {{old, height}}});
    update();
}

void Page::setDx(double dx)
{
    impl_->setDx(dx);
    update();
}

double Page::dx() const { return impl_->item->x(); }

void Page::setDy(double dy)
{
    impl_->setDy(dy);
    update();
}

double  Page::dy() const { return impl_->item->y(); }

void Page::setOrientation(QPrinter::Orientation o)
{
     QPrinter::Orientation cur = width() > height()
        ? QPrinter::Orientation::Landscape
        : QPrinter::Orientation::Portrait;

    if (o == cur) {
        return;
    }

    double h = width();
    double w = height();
    LayoutItem::setWidth(w);
    LayoutItem::setHeight(h);
    impl_->setWidth(w);
    impl_->setHeight(h);
    layout()->onPageResize(this, SizeDiff{{{h, w}}, {{w, h}}});
    update();
}

QPrinter::Orientation  Page::orientation() const
{
    return width() > height()
        ? QPrinter::Orientation::Landscape
        : QPrinter::Orientation::Portrait;
}

bool Page::checkGeometryAvailable(
    const Page& p,
    const QSizeF& size,
    double dx, double dy)
{
    const QList<QGraphicsItem* >& fields = p.childItems();
    QList<QGraphicsItem* >::const_iterator i = fields.begin();
    bool result = true;
    while (result && i != fields.end()) {
        const LayoutItem* tli = dynamic_cast<const LayoutItem*>(*i);
        if (tli) {
            result = tli->x() + tli->width() - p.dx() + dx <= size.width() &&
                tli->y() + tli->height() - p.dy() + dy <= size.height();
        }
        ++i;
    }
    return result;
}

void Page::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
    //this->setCurrent(true); // set current page in context
}

} // namespace doctpl
