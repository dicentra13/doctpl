#pragma once

#include <doctpl/table_field.h>

namespace doctpl {

class TableField::Impl {
public:
    Impl(
        double width,
        double height,
        size_t rows,
        size_t cols,
        const FormattingSettingsPtr& fs,
        Field* field);

    Impl(
        const std::vector<double>& columnWidth,
        const std::vector<double>& rowHeight,
        std::vector<FormattingSettingsPtr> columnFormatting,
        Field* field);

    void crossAllLines(QPainter* painter, Field* field) ;

    void resizeWidth(double delta);
    void resizeHeight(double delta);

    void addRow(size_t row, double rowHeight, Field* parentField);
    void deleteRow(size_t row);
    void resizeRow(size_t row, double height);

    double rowHeight(size_t row) const;
    double rowTopY(size_t row) const;
    size_t rowsCount() const;

    void addColumn(
        size_t col, double width, const FormattingSettingsPtr& formattingSettings,
        Field* parentField);
    void deleteColumn(size_t col);
    void resizeColumn(size_t col, double width);

    double columnWidth(size_t col) const;
    double columnLeftX(size_t col) const;
    size_t columnsCount() const;

    FormattingSettings& columnFormatting(size_t col);
    const FormattingSettings& columnFormatting(size_t col) const;
    void setColumnFormatting(size_t col, const FormattingSettingsPtr& formattingSettings);

    void setText(size_t row, size_t column, const QString& text);
    QString text(size_t row, size_t column) const;

    void clear();

    void checkRowIndexIsValid(size_t /*row*/) const;
    void checkColumnIndexIsValid(size_t /*col*/) const;

private:
    void deleteItem(size_t row, size_t col);

    std::vector<std::vector<TextItem *> > cells_;
    std::vector<FormattingSettingsPtr> columnsFormatting_;
};

} // namespace doctpl
