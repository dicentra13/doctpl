#include "textitem.h"

#include "textrenderinfo.h"
#include "field.h"

#include <QPen>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QPainterPath>

const double TextItem::scaleFactor_ = (25.4 / 72.0);

TextItem::TextItem(double width, double height)
    : width_(width)
    , height_(height)
    , isTextSet_(false)
{
    ti_ = new QGraphicsSimpleTextItem;
    // ВАЖНО!!! Чтобы все размеры в ti_ задавались в пунктах, в т.ч. размер шрифта
    // а приводится к координатам сцены - в мм

    ti_->setScale(scaleFactor_);
    ti_->setParentItem(this);
    QPen p;
    p.setWidthF(0.1);
    p.setCosmetic(true);
    ti_->setPen(p);
}

QRectF TextItem::boundingRect() const {

        QPen p;
        p.setWidthF(1.0);
        double f = p.widthF();
        return QRectF(QPointF(-f / 2.0, -f / 2.0),
                      QSizeF(width_ + f, height_ + f));
}

void TextItem::paint(QPainter *painter,
    const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    // границы у TextItem не отрисовываются
    // отрисовка QGraphicsSimpleItem производится сама
}

void TextItem::setWidth(double width) {
    width_ = width;
    ti_->setPos(textPos());
    update();
}

void TextItem::setHeight(double height) {
    height_ = height;
    ti_->setPos(textPos());
    update();
}

void TextItem::setText(const QString &text) {

    ti_->setText(text);
    if (text.size() == 0) {
        isTextSet_ = false;
        state_ = true;
    } else {
        // непустой текст
        textWidth_ = ti_->boundingRect().width() * scaleFactor_; // в мм
        textHeight_ = ti_->boundingRect().height() * scaleFactor_; // в мм
        // проверяем только ширину
        // FIXME info_ не должно быть 0
        // должно быть установлено хотя бы в настройки по умолчанию
        Margins margins = info_->margins();
        state_ = textWidth_ + margins.left + margins.right < width_;
        ti_->setPos(textPos());
        isTextSet_ = true;
    }
    update();
}

void TextItem::setTextRenderInfo(TextRenderInfo *info) {

    if (info) {
        info_ = info;
        QFont font = info_->font();
//        font.setPointSize(11);
//        font.setStyleStrategy(QFont::PreferMatch);
        ti_->setFont(font);
        std::cout << font.styleStrategy() << std::endl;
        if (isTextSet_) {
            textWidth_ = ti_->boundingRect().width() * scaleFactor_; // в мм
            textHeight_ = ti_->boundingRect().height() * scaleFactor_; // в мм
            Margins margins = info_->margins();
            state_ = textWidth_ + margins.left + margins.right < width_;
            ti_->setPos(textPos());
            //info_->addItem(this);
        }
        update();
    }
}

QPointF TextItem::textPos() {

    Margins margins = info_->margins();
    QFontMetrics m(info_->font());
    // расчет x и y для pos с учетом alignment
    double x, y;
    if (info_->flags() & Qt::AlignHCenter)
        x = (width_ - textWidth_) / 2.0;
    else if (info_->flags() & Qt::AlignRight)
        x = width_ - margins.right - textWidth_;
    else // default - AlignLeft
        x = margins.left;
    if (info_->flags() & Qt::AlignVCenter)
        y = (height_ - textHeight_) / 2.0;
    else if (info_->flags() & Qt::AlignTop)
        y = margins.top;
    else // default - AlignBottom
        y = -m.ascent() * scaleFactor_ + height_ - margins.bottom;

    return QPointF(x, y);
}

void TextItem::needsUpdate() {

    ti_->setFont(info_->font());
    if (isTextSet_) {
        ti_->setFont(info_->font());
        textWidth_ = ti_->boundingRect().width() * scaleFactor_; // в мм
        textHeight_ = ti_->boundingRect().height() * scaleFactor_; // в мм
        Margins margins = info_->margins();
        state_ = textWidth_ + margins.left + margins.right < width_;
        ti_->setPos(textPos());
    update();
    }

}
