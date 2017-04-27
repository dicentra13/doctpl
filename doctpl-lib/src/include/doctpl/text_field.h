#pragma once

#include <doctpl/field.h>
#include <doctpl/formatting_settings.h>

#include <memory>

namespace doctpl {

class Page;

class TextField : public WatchedField<TextField> {
public:
    TextField(
        const QString &name,
        const QSizeF& size,
        const QPointF& pos,
        const FormattingSettingsPtr& formattingSettings,
        Page* page);

    virtual ~TextField();

    // from Field

    virtual const QString& fieldType() const;
    virtual void clear();

    // from LayoutItem

    virtual void setWidth(double width);
    virtual void setHeight(double height);

    // from QGraphicsItem

    virtual void paint(
        QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget = 0);

    // text

    void setText(const QString &text);
    QString text() const;

    FormattingSettings& formattingSettings();
    const FormattingSettings& formattingSettings() const;
    void setFormattingSettings(const FormattingSettingsPtr& fs);

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace doctpl
