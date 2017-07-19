#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "../field.h"

#include <QString>

#include <memory>

class TextRenderInfo;
class TextItem;
class TextFieldController;

class TextField : public Field {
public:
    TextField(TextFieldController *controller, double width, double height,
        TextRenderInfo *local, bool isLocalFormatting);
    virtual ~TextField();

    virtual void setCurrent(bool isCurrent);
    // из QGraphicsItem
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual FieldController *controller() const;

    bool setText(const QString &text);
    // FIXME может быть const QString&
    QString text() const;
    /**
      TODO
      */
    //void setTextRenderInfo(TextRenderInfo *tri) {/*Is really needed?*/}
    TextRenderInfo *textRenderInfo() const {
        return renderInfo_;
    }
    TextRenderInfo *localTextRenderInfo() const {
        return localRenderInfo_.get();
    }
    virtual void setWidth(double width);
    virtual void setHeight(double height);
    virtual void clear();
    void switchFormattingSettings(bool isLocal);
    bool isLocalFormatting() const {
        return isLocalFormatting_;
    }
private:
    TextItem *textItem_;
    TextRenderInfo *renderInfo_;
    TextFieldController *controller_;
    std::unique_ptr<TextRenderInfo> localRenderInfo_;
    bool isLocalFormatting_;
};

#endif // TEXTFIELD_H
