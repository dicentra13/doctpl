#include "text_item.h"

#include <doctpl/field.h>
#include <doctpl/formatting_settings.h>
#include <doctpl/exception.h>

#include <QGraphicsSimpleTextItem>
#include <QFontMetrics>
#include <QPen>

namespace doctpl {

namespace {

const double SCALE_FACTOR = (25.4 / 72.0); // pt to mm

TextItem::ID genID()
{
    static TextItem::ID s_id = 0;
    return ++s_id;
}

} // namespace

class TextItem::Impl {
public:
    Impl(
            TextItem* parent,
            double width,
            double height,
            const FormattingSettingsPtr& fs)
        : width(width)
        , height(height)
        , textWidth(0.0)
        , textHeight(0.0)
        , formatting(fs)
    {
        id = genID();

        ASSERT(parent);
        ASSERT(formatting);
        ti = new QGraphicsSimpleTextItem(parent);
        // set all dimensions in ti in pt but scale to scene coords in mm
        ti->setScale(SCALE_FACTOR);
        QPen p;
        p.setWidthF(0.1);
        p.setCosmetic(true);
        ti->setPen(p);
    }

    void setWidth(double width)
    {
        this->width = width;
        ti->setPos(textPos());
    }

    void setHeight(double height)
    {
        this->height = height;
        ti->setPos(textPos());
    }

    QPointF textPos()
    {
        auto fs = formatting->current();
        const auto& margins = fs->margins();
        QFontMetrics m(fs->font());

        double x, y;
        if (fs->flags() & Qt::AlignHCenter) {
            x = (width - textWidth) / 2.0;
        } else if (fs->flags() & Qt::AlignRight) {
            x = width - margins.right - textWidth;
        } else { // default - AlignLeft
            x = margins.left;
        }

        if (fs->flags() & Qt::AlignVCenter) {
            y = (height - textHeight) / 2.0;
        } else if (fs->flags() & Qt::AlignTop) {
            y = margins.top;
        } else { // default - AlignBottom
            y = - m.ascent() * SCALE_FACTOR + height - margins.bottom;
        }

        return QPointF(x, y);
    }

    void setText(const QString &text)
    {
        ti->setText(text);
        if (text.isEmpty()) {
            textWidth = textHeight = 0.0;
            return;
        }
        update();
    }

    void setFormatting(const FormattingSettingsPtr& fs)
    {
        ASSERT(fs);
        formatting = fs;
        QFont font = formatting->current()->font();
        ti->setFont(font);
        if (!ti->text().isEmpty()) {
            update();
        }
    }

    void update()
    {
        textWidth = ti->boundingRect().width() * SCALE_FACTOR; // mm
        textHeight = ti->boundingRect().height() * SCALE_FACTOR; // mm
        ti->setPos(textPos());
    }

    TextItem::ID id;

    double width;
    double height;
    double textWidth;
    double textHeight;
    QGraphicsSimpleTextItem* ti;
    FormattingSettingsPtr formatting;
};


TextItem::TextItem(
        double width,
        double height,
        const FormattingSettingsPtr& fs,
        Field* parentField)
    : impl_(new Impl(this, width, height, fs))
{
    ASSERT(parentField);
    setParentItem(parentField);
    impl_->setFormatting(fs);
    impl_->formatting->add(this);
}

TextItem::~TextItem() { impl_->formatting->remove(this); }

const TextItem::ID& TextItem::id() const { return impl_->id; }

QRectF TextItem::boundingRect() const
{
    QPen p;
    p.setWidthF(1.0);
    double f = p.widthF();
    return QRectF(
        QPointF(-f / 2.0, -f / 2.0),
        QSizeF(impl_->width + f, impl_->height + f));
}

void TextItem::paint(
    QPainter* /*painter*/,
    const QStyleOptionGraphicsItem* /*option*/,
    QWidget* /*widget*/)
{}


double TextItem::width() const { return impl_->width; }

void TextItem::setWidth(double width)
{
    impl_->setWidth(width);
    update();
}

double TextItem::height() const { return impl_->height; }

void TextItem::setHeight(double height)
{
    impl_->setHeight(height);
    update();
}

void TextItem::setText(const QString &text)
{
    impl_->setText(text);
    update();
}

QString TextItem::text() const { return impl_->ti->text(); }

void TextItem::setFormatting(const FormattingSettingsPtr& fs)
{
    ASSERT(fs);
    if (impl_->formatting != fs) {
        impl_->formatting->remove(this);
        impl_->setFormatting(fs);
        impl_->formatting->add(this);
        update();
    }
}

const FormattingSettings& TextItem::formatting() const
{
    return *impl_->formatting;
}

FormattingSettings& TextItem::formatting()
{
    return *impl_->formatting;
}

void TextItem::forceUpdate()
{
    impl_->ti->setFont(impl_->formatting->current()->font());
    if (!impl_->ti->text().isEmpty()) {
        impl_->update();
        update();
    }
}

} // namespace doctpl
