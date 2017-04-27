#include <doctpl/layout_item.h>

#include <doctpl/exception.h>

#include <QPen>

namespace doctpl {

struct LayoutItem::Impl {
    double width;
    double height;
};

LayoutItem::LayoutItem(const QSizeF& size)
{
    REQUIRE(size.isValid(), "Invalid layout item dimensions");
    impl_.reset(new Impl{size.width(), size.height()});
}

LayoutItem::~LayoutItem() {}

QRectF LayoutItem::boundingRect() const
{
    QPen p;
    p.setWidthF(1.0);
    double f = p.widthF();
    return QRectF(
        QPointF(-f / 2.0, -f/ 2.0),
        QSizeF(impl_->width + f, impl_->height + f));
}

void LayoutItem::setWidth(double width)
{
    REQUIRE(width >= 0.0, "Negative item width");
    impl_->width = width;
}

double LayoutItem::width() const { return impl_->width; }

void LayoutItem::setHeight(double height)
{
    REQUIRE(height >= 0.0, "Negative item height");
    impl_->height = height;
}

double LayoutItem::height() const { return impl_->height; }

} // namespace doctpl
