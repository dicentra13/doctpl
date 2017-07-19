#ifndef TEXT_ITEM_H
#define TEXT_ITEM_H

#include <QGraphicsItem>
#include <QFont>
#include <QGraphicsSimpleTextItem>

class TextRenderInfo;
class QMouseEvent;

class TextItem : public QGraphicsItem {
public:
        TextItem(double width, double height);

    // reimplementing 2 pure virtual QGraphicsItem functions
    virtual QRectF boundingRect() const;
        virtual void paint(QPainter *painter,
            const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
        double width() const {
            return width_;
        }
        void setWidth(double width);
        double height() const {
            return height_;
        }
        void setHeight(double height);
        void setText(const QString &text);
        QString text() const {
            return ti_->text();
        }
        void setTextRenderInfo(TextRenderInfo *info);
        // само поле здесь ничего не меняет
        // измененные данные приходят извне
        TextRenderInfo *textRenderInfo() const {
            return info_;
        }
        //bool state() const {
        //    return state_;
        //}
        void needsUpdate();
private:
        QPointF textPos();
        // пересчет мм в пункты
        static const double scaleFactor_;
        double width_, height_;
        bool isTextSet_;
        bool state_;
        double textWidth_, textHeight_;
        QGraphicsSimpleTextItem *ti_;
        TextRenderInfo *info_;
};

#endif
