#include <doctpl/text_field.h>

#include "text_item.h"

#include <doctpl/page.h>
#include <doctpl/layout.h>
#include <doctpl/template.h>
#include <doctpl/exception.h>

// update() is called by TextItem setters

namespace doctpl {

namespace {

const QString TEXT_TYPE = "text";

} // namespace

class TextField::Impl {
public:
    Impl(
            double width,
            double height,
            const FormattingSettingsPtr& formattingSettings,
            Field* field)
        : item(new TextItem(width, height, formattingSettings, field))
    {
        item->setPos(0.0, 0.0);
    }

    TextItem* item;
};


TextField::TextField(
        const QString& name,
        const QSizeF& size,
        const QPointF& pos,
        const FormattingSettingsPtr& formattingSettings,
        StylePtr style,
        Page* page)
    : WatchedField<TextField>(
        name,
        size,
        pos,
        std::move(style),
        page,
        {page->layout()->document()->fields()->as<TextField>()})
    , impl_(new Impl(size.width(), size.height(), formattingSettings, this))
{
    setFiltersChildEvents(true);
}

TextField::~TextField() {}

const QString& TextField::fieldType() const { return TEXT_TYPE; }

void TextField::clear() { impl_->item->setText(""); }

void TextField::setWidth(double width)
{
    Field::setWidth(width);
    impl_->item->setWidth(width);
}

void TextField::setHeight(double height)
{
    Field::setHeight(height);
    impl_->item->setHeight(height);
}

void TextField::paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* /*option*/,
    QWidget* /*widget*/)
{
//    painter->save();
////    if (!Field::isFinalPrinting()) {
//        QPen cur(painter->pen());
//        cur.setWidth(0);
//        cur.setColor(QColor(Qt::black));
////        if (isCurrent_) {
////            painter->setBrush(QBrush(QColor(0, 100, 200, 50)));
////        }
//        painter->setPen(cur);
//        painter->drawRect(QRectF(QPointF(0.0, 0.0), QPointF(width(), height())));
////    }
//    painter->restore();
}

void TextField::setText(const QString &text) { impl_->item->setText(text); }

QString TextField::text() const { return impl_->item->text(); }

FormattingSettings& TextField::formattingSettings() { return impl_->item->formatting(); }
const FormattingSettings& TextField::formattingSettings() const { return impl_->item->formatting(); }

void TextField::setFormattingSettings(const FormattingSettingsPtr& fs)
{
    ASSERT(fs);
    impl_->item->setFormatting(fs);
}

} // namespace doctpl
