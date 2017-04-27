#pragma once

#include <doctpl/field.h>
#include <doctpl/formatting_settings.h>

#include <memory>
#include <vector>

namespace doctpl {

class TableField : public WatchedField<TableField> {
public:
    TableField(
        const QString& name,
        const QSizeF& size,
        const QPointF& pos,
        size_t rows,
        size_t cols,
        const FormattingSettingsPtr& formattingSettings,
        Page* page);

    TableField(
        const QString& name,
        const std::vector<double>& columnWidth,
        const std::vector<double>& rowHeight,
        const QPointF& pos,
        std::vector<FormattingSettingsPtr> columnFormatting,
        Page* page);

    virtual ~TableField();

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

    // rows management

    void addRow(size_t row, double height);
    void deleteRow(size_t row);
    void resizeRow(size_t row, double height);

    double rowHeight(size_t row) const;
    double rowY(size_t row) const;
    size_t rowsCount() const;

    // columns management

    void addColumn(size_t col, double width, const FormattingSettingsPtr& fs);
    void deleteColumn(size_t col);
    void resizeColumn(size_t col, double width);

    double columnWidth(size_t col) const;
    double columnX(size_t col) const;
    size_t columnsCount() const;

    // columns formatting

    FormattingSettings& columnFormatting(size_t col);
    const FormattingSettings& columnFormatting(size_t col) const;
    void setColumnFormatting(size_t col, const FormattingSettingsPtr& fs);

    // text

    void setText(size_t row, size_t column, const QString &text);
    QString text(size_t row, size_t column) const;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace doctpl
