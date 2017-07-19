#include "textfield.h"
#include "../textrenderinfo.h"
#include "../textitem.h"
#include "textfieldcontroller.h"

#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsView>

TextField::TextField(TextFieldController *controller,
        double width, double height, TextRenderInfo *local,
        bool isLocalFormatting)
    : Field(QString("text"), width, height)
    , controller_(controller)
    , isLocalFormatting_(isLocalFormatting)
{
    textItem_ = new TextItem(width, height);
    localRenderInfo_.reset(local);
    if (isLocalFormatting_) {
        renderInfo_ = localRenderInfo_.get();
    } else {
        renderInfo_ = TextRenderInfo::globals();
    }
    renderInfo_->addItem(textItem_);
    textItem_->setTextRenderInfo(renderInfo_);
    textItem_->setParentItem(this);
    textItem_->setPos(0.0, 0.0);
    setFiltersChildEvents(true);
}

TextField::~TextField() {
    controller_->removeField(name());
}

FieldController *TextField::controller() const {
    return controller_;
}

void TextField::setCurrent(bool isCurrent) {
    TemplateLayoutItem::setCurrent(isCurrent);
    if (isCurrent)
        controller_->setCurrentField(this);
    else
        controller_->setCurrentField(0);
}

void TextField::paint(QPainter *painter,
    const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->save();
    if (!Field::isFinalPrinting()) {
        QPen cur(painter->pen());
        cur.setWidth(0);
        cur.setColor(QColor(Qt::black));
        if (isCurrent_) {
            painter->setBrush(QBrush(QColor(0, 100, 200, 50)));
        }
        painter->setPen(cur);
        painter->drawRect(QRectF(QPointF(0.0, 0.0), QPointF(width_, height_)));
    }
    painter->restore();
}

bool TextField::setText(const QString &text) {
    textItem_->setText(text);
    return true;
}

QString TextField::text() const {
    return textItem_->text();
}

void TextField::setWidth(double width) {
    Field::setWidth(width);
    textItem_->setWidth(width);
    textItem_->setPos(0.0, 0.0);
}

void TextField::setHeight(double height) {
    Field::setHeight(height);
    textItem_->setHeight(height);
    textItem_->setPos(0.0, 0.0);
}

void TextField::clear() {
    textItem_->setText("");
}

void TextField::switchFormattingSettings(bool toLocal) {
    renderInfo_->removeItem(textItem_);
    isLocalFormatting_ = toLocal;
    if (isLocalFormatting_) {
        renderInfo_ = localRenderInfo_.get();
    } else {
        renderInfo_ = TextRenderInfo::globals();
    }
    renderInfo_->addItem(textItem_);
    textItem_->setTextRenderInfo(renderInfo_);
    update();
}
