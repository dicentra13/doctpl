#include "table_field_impl.h"

#include "text_item.h"

#include <doctpl/exception.h>

#include <QGraphicsScene>

namespace doctpl {

TableField::Impl::Impl(
    double width,
    double height,
    size_t rows,
    size_t cols,
    const FormattingSettingsPtr& fs,
    Field* field)
{
    ASSERT(fs);
    ASSERT(field);
    double cellWidth = width / (double) cols;
    double cellHeight = height / (double) rows;
    for (size_t j = 0; j < cols; ++j) {
        columnsFormatting_.push_back(
            std::make_shared<FormattingSettings>(
                fs->global(),
                fs->local()->copy(),
                fs->mode()));
    }
    for (size_t i = 0; i < rows; ++i) {
        std::vector<TextItem *> row;
        for (size_t j = 0; j < cols; ++j) {
            row.push_back(new TextItem(
                cellWidth, cellHeight, columnsFormatting_[j], field));
            row[j]->setPos(cellWidth * j, cellHeight * i);
        }
        cells_.push_back(row);
    }
}

TableField::Impl::Impl(
    const std::vector<double>& columnWidth,
    const std::vector<double>& rowHeight,
    std::vector<FormattingSettingsPtr> columnFormatting,
    Field* field)
{
    ASSERT(field);
    for (const auto& fs : columnFormatting) {
        ASSERT(fs);
    }
    REQUIRE(
        columnFormatting.size() == columnWidth.size(),
        "Formatting settings vector size mismatch");

    columnsFormatting_ = std::move(columnFormatting);
    for (size_t i = 0; i < rowHeight.size(); ++i) {
        std::vector<TextItem *> row;
        for (size_t j = 0; j < columnWidth.size(); ++j) {
            row.push_back(new TextItem(
                columnWidth[j], rowHeight[i], columnsFormatting_[j], field));
            double x = (j == 0) ? 0.0 : row[j - 1]->x() + row[j - 1]->width();
            double y = (i == 0) ? 0.0 : cells_[i - 1][0]->y() +
                cells_[i - 1][0]->height();
            row[j]->setPos(x, y);
        }
        cells_.push_back(row);
    }
}

void TableField::Impl::clear()
{
    for (size_t i = 0; i < rowsCount(); ++i) {
        for (size_t j = 0; j < columnsCount(); ++j) {
            cells_[i][j]->setText("");
        }
    }
}

void TableField::Impl::crossAllLines(QPainter* painter, Field* field)
{
    size_t freeRow = rowsCount();
    for ( ; freeRow > 0; --freeRow) {
        size_t j = 0;
        for (; j < columnsCount() && text(freeRow - 1, j).isEmpty(); ++j)
            ;
        if (j < columnsCount()) {
            break;
        }
    }

    if (freeRow == rowsCount()) {
        return;
    }

    double xl, yt, xr, yb;
    xl = 4.0; //cells_[currentAppendRow_][0]->height() / 2.0;
    xr = field->width() - 4.0; //cells_[cells_.size() - 1][0]->height() / 2.0;
    if (freeRow == rowsCount() - 1) {
        yt = rowTopY(freeRow) + rowHeight(freeRow) / 4.0;
        yb = rowTopY(freeRow) + 3.0 * rowHeight(freeRow) / 4.0;
    } else {
        yt = rowTopY(freeRow) + rowHeight(freeRow) / 2.0;
        yb = field->height() - rowHeight(rowsCount() - 1) / 2.0;
    }
    painter->drawLine(QPointF(xl, yt), QPointF(xr, yt));
    painter->drawLine(QPointF(xr, yt), QPointF(xl, yb));
    painter->drawLine(QPointF(xl, yb), QPointF(xr, yb));
    painter->drawLine(
        QPointF(field->width() / 4.0, (yt + yb) /2.0),
        QPointF(3.0 * field->width() / 4.0, (yt + yb) /2.0));
}

void TableField::Impl::resizeWidth(double delta)
{
    const auto newLastColumnWidth = columnWidth(columnsCount() - 1) + delta;
    for (size_t i = 0; i < rowsCount(); ++i) {
        cells_[i][columnsCount() - 1]->setWidth(newLastColumnWidth);
    }
}

void TableField::Impl::resizeHeight(double delta)
{
    const auto newLastRowHeight = rowHeight(rowsCount() - 1) + delta;
    for (size_t j = 0; j < columnsCount(); ++j) {
        cells_[rowsCount() - 1][j]->setHeight(newLastRowHeight);
    }
}

void TableField::Impl::addRow(size_t row, double rowHeight, Field* field)
{
    double newY = row == 0
        ? 0.0
        : rowTopY(row - 1) + this->rowHeight(row - 1);

    for (size_t i = row; i < rowsCount(); ++i) {
        const auto y = rowTopY(i) + rowHeight;
        for (size_t j = 0; j < columnsCount(); ++j) {
            cells_[i][j]->setY(y);
        }
    }
    std::vector<TextItem *> newRow;
    for (size_t j = 0; j < columnsCount(); ++j) {
        newRow.push_back(new TextItem(
            columnWidth(j), rowHeight, columnsFormatting_[j], field));
        newRow[j]->setPos(columnLeftX(j), newY);
    }
    cells_.insert(cells_.begin() + row, newRow);
}

void TableField::Impl::deleteRow(size_t row)
{
    const double rowHeight = this->rowHeight(row);
    for (size_t j = 0; j < columnsCount(); ++j) {
        deleteItem(row, 0);
    }

    cells_.erase(cells_.begin() + row);

    for (size_t i = row; i < rowsCount(); ++i) {
        const double newY = rowTopY(i) - rowHeight;
        for (size_t j = 0; j < columnsCount(); ++j) {
            cells_[i][j]->setY(newY);
        }
    }
}

void TableField::Impl::resizeRow(size_t row, double height)
{
    double delta = height - rowHeight(row);
    for (size_t i = row + 1; i < rowsCount(); ++i) {
        const double newY = rowTopY(i) + delta;
        for (size_t j = 0; j < columnsCount(); ++j) {
            cells_[i][j]->setY(newY);
        }
    }
    for (size_t j = 0; j < columnsCount(); ++j)
        cells_[row][j]->setHeight(height);
}

double TableField::Impl::rowHeight(size_t row) const { return cells_.at(row)[0]->height(); }

double TableField::Impl::rowTopY(size_t row) const { return cells_.at(row)[0]->y(); }

size_t TableField::Impl::rowsCount() const { return cells_.size(); }

void TableField::Impl::addColumn(
    size_t col, double width, const FormattingSettingsPtr& fs, Field* parentField)
{
    ASSERT(fs);

    double newX = col == 0
        ? 0.0
        : columnLeftX(col - 1) + columnWidth(col - 1);

    for (size_t j = col; j < columnsCount(); ++j) {
        const double newColX = columnLeftX(j) + width;
        for (size_t i = 0; i < rowsCount(); ++i) {
            cells_[i][j]->setX(newColX);
        }
    }

    columnsFormatting_.insert(columnsFormatting_.begin() + col, fs);

    for (size_t i = 0; i < rowsCount(); ++i) {
        cells_[i].insert(
            cells_[i].begin() + col,
            new TextItem(
                width, rowHeight(i), columnsFormatting_[col], parentField));
        cells_[i][col]->setPos(newX, cells_[i][col == 0 ? 1 : 0]->y());
    }
}

void TableField::Impl::deleteColumn(size_t col)
{
    double colWidth = columnWidth(col);

    columnsFormatting_.erase(columnsFormatting_.begin() + col);

    for (size_t i = 0; i < rowsCount(); ++i) {
        deleteItem(i, col);
    }

    for (size_t j = col; j < columnsCount(); ++j) {
        const double newX = columnLeftX(j) - colWidth;
        for (size_t i = 0; i < rowsCount(); ++i) {
            cells_[i][j]->setX(newX);
        }
    }
}

void TableField::Impl::resizeColumn(size_t col, double width)
{
    double delta = width - columnWidth(col);

    for (size_t j = col + 1; j < columnsCount(); ++j) {
        const double newX = columnLeftX(j) + delta;
        for (size_t i = 0; i < rowsCount(); ++i) {
            cells_[i][j]->setX(newX);
        }
    }

    for (size_t i = 0; i < rowsCount(); ++i) {
        cells_[i][col]->setWidth(width);
    }
}

double TableField::Impl::columnWidth(size_t col) const { return cells_[0].at(col)->width(); }

double TableField::Impl::columnLeftX(size_t col) const { return cells_[0].at(col)->x(); }

size_t TableField::Impl::columnsCount() const { return cells_[0].size(); }

FormattingSettings& TableField::Impl::columnFormatting(size_t col)
{
    return *columnsFormatting_.at(col);
}

const FormattingSettings& TableField::Impl::columnFormatting(size_t col) const
{
    return *columnsFormatting_.at(col);
}

void TableField::Impl::setColumnFormatting(size_t col, const FormattingSettingsPtr& fs)
{
    ASSERT(fs);
    columnsFormatting_.at(col) = fs;
    for (size_t i = 0; i < rowsCount(); ++i) {
        cells_[i][col]->setFormatting(columnsFormatting_[col]);
    }
}

// text

void TableField::Impl::setText(size_t row, size_t column, const QString& text)
{
    cells_.at(row).at(column)->setText(text);
}

QString TableField::Impl::text(size_t row, size_t column) const
{
    return cells_.at(row).at(column)->text();
}


void TableField::Impl::checkRowIndexIsValid(size_t row) const
{
    REQUIRE(row < rowsCount(), "Row " << row << " is out of range");
}

void TableField::Impl::checkColumnIndexIsValid(size_t col) const
{
    REQUIRE(col < columnsCount(), "Column " << col << " is out of range");
}

void TableField::Impl::deleteItem(size_t row, size_t col)
{
    TextItem* item = cells_[row][col];
    QGraphicsScene* scene = item->scene();
    item->setParentItem(nullptr);
    scene->removeItem(item);
    cells_[row].erase(cells_[row].begin() + col);
    delete item;
}

} // namespace doctpl
