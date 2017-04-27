#include <doctpl/table_field.h>

#include "text_item.h"
#include "table_field_impl.h"

#include <doctpl/page.h>
#include <doctpl/layout.h>
#include <doctpl/template.h>
#include <doctpl/exception.h>

#include <algorithm>

namespace doctpl {

namespace {

auto sum = [] (const std::vector<double>& v) -> double
{
    return std::accumulate(
        v.begin(),
        v.end(),
        0.0,
        [] (double v1, double v2) { return v1 + v2; });
};

const QString TABLE_TYPE = "table";

} // namespace

TableField::TableField(
        const QString& name,
        const QSizeF& size,
        const QPointF& pos,
        size_t rows,
        size_t cols,
        const FormattingSettingsPtr& fs,
        Page* page)
    : WatchedField<TableField>(
        name,
        size,
        pos,
        page,
        {page->layout()->document()->fields()->as<TableField>()})
    , impl_(new Impl(size.width(), size.height(), rows, cols, fs, this))
{}

TableField::TableField(
        const QString& name,
        const std::vector<double>& columnWidth,
        const std::vector<double>& rowHeight,
        const QPointF& pos,
        std::vector<FormattingSettingsPtr> columnFormatting,
        Page* page)
    : WatchedField<TableField>(
        name,
        QSizeF(sum(columnWidth), sum(rowHeight)),
        pos,
        page,
        {page->layout()->document()->fields()->as<TableField>()})
    , impl_(new Impl(columnWidth, rowHeight, std::move(columnFormatting), this))
{}

TableField::~TableField() {}

const QString& TableField::fieldType() const { return TABLE_TYPE; }

void TableField::clear() { impl_->clear(); }

void TableField::setWidth(double width)
{
    ASSERT(columnsCount() > 0);
    resizeColumn(
        columnsCount() - 1,
        columnWidth(columnsCount() - 1) + width - this->width());
}

void TableField::setHeight(double height)
{
    ASSERT(rowsCount() > 0);
    resizeRow(
        rowsCount() - 1,
        rowHeight(rowsCount() - 1) + height - this->height());
}

void TableField::paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* /*option*/,
    QWidget* /*widget*/)
{
    painter->save();
    QPen cur;
////    if (!Field::isFinalPrinting()) {
//        cur = painter->pen();
//        cur.setColor(QColor(Qt::black));
//        cur.setWidth(0);
//        painter->setPen(cur);
//        double lineX = columnWidth(0);
//        for (size_t j = 1; j < columnsCount(); ++j) {
//            painter->drawLine(QPointF(lineX, 0.0), QPointF(lineX, height()));
//            lineX += columnWidth(j);
//        }
//        double lineY = rowHeight(0);
//        for (size_t i = 1; i < rowsCount(); ++i) {
//            painter->drawLine(QPointF(0.0, lineY), QPointF(width(), lineY));
//            lineY += rowHeight(i);
//        }
////        if (isCurrent_)
////            cur.setColor(QColor(Qt::blue));
////        else
//            cur.setColor(QColor(Qt::black));
//        painter->setPen(cur);
//        painter->drawRect(QRectF(QPointF(0.0, 0.0), QPointF(width(), height())));
////        cur.setWidth(0);
////        cur.setColor(QColor(150, 150, 150));
////        painter->setPen(cur);
////        if (mode_ == CrossEverySingleLine) {
////            crossEverySingleLine(painter);
////        } else if (mode_ == CrossAllLines) {
////            crossAllLines(painter);
////        }
////        if (isCurrent_) {
////            painter->setBrush(QBrush(QColor(0, 100, 200, 50)));
////            painter->drawRect(QRectF(QPointF(
////                cells_[currentRow_][currentCol_]->x(),
////                cells_[currentRow_][currentCol_]->y()), QPointF(
////                cells_[currentRow_][currentCol_]->x() +
////                cells_[currentRow_][currentCol_]->width(),
////                cells_[currentRow_][currentCol_]->y() +
////                cells_[currentRow_][currentCol_]->height())));
////        }
////    } else {
        cur.setWidthF(0.1);
        cur.setColor(QColor(Qt::black));
        painter->setPen(cur);
//        if (mode_ == CrossEverySingleLine) {
//            crossEverySingleLine(painter);
//        } else if (mode_ == CrossAllLines) {
            impl_->crossAllLines(painter, this);
//        }
//    }
    painter->restore();
}

// rows

void TableField::addRow(size_t row, double rowHeight)
{
    REQUIRE(rowHeight >= 0.0, "Negative row height");
    REQUIRE(row <= rowsCount(), "Row index " << row << " is out of range");
    Field::setHeight(height() + rowHeight);
    impl_->addRow(row, rowHeight, this);
    update(); // page()->update();
}

void TableField::deleteRow(size_t row)
{
    REQUIRE(rowsCount() > 1, "Cannot delete last row");
    impl_->checkRowIndexIsValid(row);
    const double newHeight = height() - impl_->rowHeight(row);
    Field::setHeight(newHeight);
    impl_->deleteRow(row);
    update(); // page()->update();
}

void TableField::resizeRow(size_t row, double height)
{
    REQUIRE(height >= 0.0, "Negative row height");
    impl_->checkRowIndexIsValid(row);
    const double delta = height - rowHeight(row);
    Field::setHeight(this->height() + delta);
    impl_->resizeRow(row, height);
    update(); // page()->update();
}

double TableField::rowHeight(size_t row) const
{
    impl_->checkRowIndexIsValid(row);
    return impl_->rowHeight(row);
}

double TableField::rowY(size_t row) const
{
    impl_->checkRowIndexIsValid(row);
    return impl_->rowTopY(row);
}

size_t TableField::rowsCount() const { return impl_->rowsCount(); }

// columns

void TableField::addColumn(
    size_t col, double colWidth, const FormattingSettingsPtr& fs)
{
    REQUIRE(colWidth >= 0.0, "Negative column width");
    ASSERT(fs);
    REQUIRE(col <= columnsCount(), "Column index " << col << " is out of range");
    Field::setWidth(width() + colWidth);
    impl_->addColumn(col, colWidth, fs, this);
    update(); // page()->update();
}

void TableField::deleteColumn(size_t col)
{
    REQUIRE(columnsCount() > 1, "Cannot delete last column");
    impl_->checkColumnIndexIsValid(col);
    const double newWidth = width() - impl_->columnWidth(col);
    Field::setWidth(newWidth);
    impl_->deleteColumn(col);
    update(); // page()->update();
}

void TableField::resizeColumn(size_t col, double width)
{
    REQUIRE(width >= 0.0, "Negative column width");
    impl_->checkColumnIndexIsValid(col);
    const double delta = width - columnWidth(col);
    Field::setWidth(this->width() + delta);
    impl_->resizeColumn(col, width);
    update(); // page()->update();
}

double TableField::columnWidth(size_t col) const
{
    impl_->checkColumnIndexIsValid(col);
    return impl_->columnWidth(col);
}

double TableField::columnX(size_t col) const
{
    impl_->checkColumnIndexIsValid(col);
    return impl_->columnLeftX(col);
}

size_t TableField::columnsCount() const { return impl_->columnsCount(); }

// text formatting

FormattingSettings& TableField::columnFormatting(size_t col)
{
    impl_->checkColumnIndexIsValid(col);
    return impl_->columnFormatting(col);
}

const FormattingSettings& TableField::columnFormatting(size_t col) const
{
    impl_->checkColumnIndexIsValid(col);
    return impl_->columnFormatting(col);
}

void TableField::setColumnFormatting(size_t col, const FormattingSettingsPtr& fs)
{
    ASSERT(fs);
    impl_->checkColumnIndexIsValid(col);
    impl_->setColumnFormatting(col, fs);
}

void TableField::setText(size_t row, size_t column, const QString& text)
{
    impl_->checkRowIndexIsValid(row);
    impl_->checkColumnIndexIsValid(column);
    impl_->setText(row, column, text);
    update();
}

QString TableField::text(size_t row, size_t column) const
{
    impl_->checkRowIndexIsValid(row);
    impl_->checkColumnIndexIsValid(column);
    return impl_->text(row, column);
}

} // namespace doctpl
