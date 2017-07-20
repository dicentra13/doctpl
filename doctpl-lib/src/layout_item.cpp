#include <doctpl/layout_item.h>

#include <doctpl/exception.h>

#include <QPen>

namespace doctpl {

struct LayoutItem::Impl {
    const Layout* layout;

    double width;
    double height;

    StylePtr style;
};

LayoutItem::LayoutItem(const Layout* layout, const QSizeF& size, StylePtr style)
{
    ASSERT(layout);
    REQUIRE(size.isValid(), "Invalid layout item dimensions");
    REQUIRE(style, "Nullptr style");
    impl_.reset(new Impl{layout, size.width(), size.height(), std::move(style)});
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

const StylePtr& LayoutItem::style() const { return impl_->style; }

void LayoutItem::setStyle(StylePtr style) { impl_->style = std::move(style); }

const Layout* LayoutItem::layout() const { return impl_->layout; }

} // namespace doctpl
