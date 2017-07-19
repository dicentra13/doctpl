#pragma once

#include <doctpl/formatting_settings.h>

#include <QGraphicsItem>
#include <QFont>

#include <memory>
#include <functional>

namespace doctpl {

class QMouseEvent;

/**
 * Base functionality of a single-line text block.
 * Used as TextField or TableField`s cell contents
 */

class Field;

class TextItem : public QGraphicsItem {
public:
    typedef size_t ID;

    TextItem(
        double width,
        double height,
        const FormattingSettingsPtr& fs,
        Field* parentField);

    virtual ~TextItem();

    const ID& id() const;

    // reimplementing 2 pure virtual QGraphicsItem functions

    virtual QRectF boundingRect() const;

    virtual void paint(
        QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget = nullptr);


    double width() const;
    void setWidth(double width);

    double height() const;
    void setHeight(double height);

    void setText(const QString &text);
    QString text() const;

    void setFormatting(const FormattingSettingsPtr& fs);
    FormattingSettings& formatting();
    const FormattingSettings& formatting() const;

    void forceUpdate();

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace doctpl
