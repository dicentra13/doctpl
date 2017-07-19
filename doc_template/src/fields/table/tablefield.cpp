#include "tablefield.h"

#include "../textrenderinfo.h"
#include "../textitem.h"
#include "tablefieldcontroller.h"
#include "../../page.h"

#include <QPainter>

TableField::TableField(TableFieldController *controller,
        double width, double height,
        size_t rows, size_t cols)
    : Field("table", width, height)
    , controller_(controller)
    , currentRow_(0)
    , currentCol_(0)
    , currentAppendRow_(0)
    , mode_(CrossAllLines)
{
    double cellWidth = width / (double) cols;
    double cellHeight = height / (double) rows;
    for (size_t j = 0; j < cols; j++) {
        localCellsFormatting_.push_back(TextRenderInfo::defaults());
        isLocalFormatting_.push_back(false);
        cellsFormatting_.push_back(TextRenderInfo::globals());
    }
    for (size_t i = 0; i < rows; i++) {
        std::vector<TextItem *> row;
        for (size_t j = 0; j < cols; j++) {
            row.push_back(new TextItem(cellWidth, cellHeight));
            cellsFormatting_[j]->addItem(row[j]);
            row[j]->setTextRenderInfo(cellsFormatting_[j]);
            row[j]->setParentItem(this);
            row[j]->setPos(cellWidth * j, cellHeight * i);
        }
        cells_.push_back(row);
    }
}

TableField::TableField(TableFieldController *controller,
        double width, double height,
        std::vector<double> heights, std::vector<double> widths,
        TextRenderInfoList columnFormatting,
        std::vector<bool> isLocalFormatting)
    : Field("table", width, height)
    , controller_(controller)
    , currentRow_(0)
    , currentCol_(0)
    , currentAppendRow_(0)
    , mode_(CrossAllLines)
{
    localCellsFormatting_.assign(columnFormatting.begin(),
        columnFormatting.end());
    isLocalFormatting_.assign(isLocalFormatting.begin(),
        isLocalFormatting.end());
    for (size_t j = 0; j < widths.size(); j++) {
        if (isLocalFormatting_[j]) {
            cellsFormatting_.push_back(&localCellsFormatting_[j]);
        } else {
            cellsFormatting_.push_back(TextRenderInfo::globals());
        }
    }
    for (size_t i = 0; i < heights.size(); i++) {
        std::vector<TextItem *> row;
        for (size_t j = 0; j < widths.size(); j++) {
            row.push_back(new TextItem(widths[j], heights[i]));
            cellsFormatting_[j]->addItem(row[j]);
            row[j]->setTextRenderInfo(cellsFormatting_[j]);
            row[j]->setParentItem(this);
            double x = (j == 0) ? 0.0 : row[j - 1]->x() + row[j - 1]->width();
            double y = (i == 0) ? 0.0 : cells_[i - 1][0]->y() +
                cells_[i - 1][0]->height();
            row[j]->setPos(x, y);
        }
        cells_.push_back(row);
    }
}

TableField::~TableField() {
    controller_->removeField(name());
    for (size_t i = 0; i < cells_.size(); i++) {
        for (size_t j = 0; j < cells_[0].size(); j++) {
            cells_[i][j]->setParentItem(0);
            delete cells_[i][j];
        }
    }
}

FieldController *TableField::controller() const {
    return controller_;
}

void TableField::setCurrent(bool isCurrent) {
    Field::setCurrent(isCurrent);
    if (isCurrent)
        controller_->setCurrentField(this);
    else
        controller_->setCurrentField(0);
}

void TableField::paint(QPainter *painter,
    const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->save();
    QPen cur;
    if (!Field::isFinalPrinting()) {
        cur = painter->pen();
        cur.setColor(QColor(Qt::black));
        cur.setWidth(0);
        painter->setPen(cur);
        double linePos = cells_[0][0]->width();
        for (size_t j = 1; j < cells_[0].size(); j++) {
            painter->drawLine(QPointF(linePos, 0.0), QPointF(linePos, height_));
            linePos += cells_[0][j]->width();
        }
        linePos = cells_[0][0]->height();
        for (size_t i = 1; i < cells_.size(); i++) {
            painter->drawLine(QPointF(0.0, linePos), QPointF(width_, linePos));
            linePos += cells_[i][0]->height();
        }
        if (isCurrent_)
            cur.setColor(QColor(Qt::blue));
        else
            cur.setColor(QColor(Qt::black));
        painter->setPen(cur);
        painter->drawRect(QRectF(QPointF(0.0, 0.0), QPointF(width_, height_)));
        cur.setWidth(0);
        cur.setColor(QColor(150, 150, 150));
        painter->setPen(cur);
        if (mode_ == CrossEverySingleLine) {
            crossEverySingleLine(painter);
        } else if (mode_ == CrossAllLines) {
            crossAllLines(painter);
        }
        if (isCurrent_) {
            painter->setBrush(QBrush(QColor(0, 100, 200, 50)));
            painter->drawRect(QRectF(QPointF(
                cells_[currentRow_][currentCol_]->x(),
                cells_[currentRow_][currentCol_]->y()), QPointF(
                cells_[currentRow_][currentCol_]->x() +
                cells_[currentRow_][currentCol_]->width(),
                cells_[currentRow_][currentCol_]->y() +
                cells_[currentRow_][currentCol_]->height())));
        }
    } else {
        cur.setWidthF(0.1);
        cur.setColor(QColor(Qt::black));
        painter->setPen(cur);
        if (mode_ == CrossEverySingleLine) {
            crossEverySingleLine(painter);
        } else if (mode_ == CrossAllLines) {
            crossAllLines(painter);
        }
    }
    painter->restore();
}

void TableField::crossEverySingleLine(QPainter */*painter*/) {
    for (size_t i = currentAppendRow_; i < cells_.size(); i++) {
        for (size_t j = 0; j < cells_[0].size(); j++) {
            cells_[i][j]->setText("Z");
        }
    }
}

void TableField::crossAllLines(QPainter *painter) {
    if (currentAppendRow_ != cells_.size()) {
        double xl, yt, xr, yb;
        xl = 4.0; //cells_[currentAppendRow_][0]->height() / 2.0;
        xr = width_ - 4.0; //cells_[cells_.size() - 1][0]->height() / 2.0;
        if (currentAppendRow_ == cells_.size() - 1) {
            yt = cells_[currentAppendRow_][0]->y() +
                cells_[currentAppendRow_][0]->height() / 4.0;
            yb = cells_[currentAppendRow_][0]->y() +
                3.0 * cells_[currentAppendRow_][0]->height() / 4.0;
        } else {
            yt = cells_[currentAppendRow_][0]->y() +
                cells_[currentAppendRow_][0]->height() / 2.0;
            yb = height_ - cells_[cells_.size() - 1][0]->height() / 2.0;
        }
        painter->drawLine(QPointF(xl, yt), QPointF(xr, yt));
        painter->drawLine(QPointF(xr, yt), QPointF(xl, yb));
        painter->drawLine(QPointF(xl, yb), QPointF(xr, yb));
        painter->drawLine(QPointF(width_ / 4.0, (yt + yb) /2.0),
            QPointF(3.0 * width_ / 4.0, (yt + yb) /2.0));
    }
}

// контроллер должен обеспечить, чтобы мы не вышли за пределы последнего
// столбца
void TableField::setWidth(double width) {
    double delta = width - width_;
    for (size_t i = 0; i < cells_.size(); i++) {
        cells_[i][cells_[0].size() - 1]->setWidth(
            cells_[i][cells_[0].size() - 1]->width() + delta);
    }
    Field::setWidth(width);
    update();
}

// контроллер должен обеспечить, чтобы мы не вышли за пределы последней
// строки
void TableField::setHeight(double height) {
    double delta = height - height_;
    for (size_t j = 0; j < cells_[0].size(); j++) {
        cells_[cells_.size() - 1][j]->setHeight(
            cells_[cells_.size() - 1][j]->height() + delta);
    }
    Field::setHeight(height);
    update();
}

void TableField::addRow(size_t pos, double height) {
    if (1  <= pos && pos <= cells_.size() + 1) {
        double newHeight = (pos != cells_.size() + 1)
            ? cells_[pos - 1][0]->y()
            : height_;
        Field::setHeight(height_ + height);
        for (size_t i = pos - 1; i < cells_.size(); i++)
            for (size_t j = 0; j < cells_[0].size(); j++)
                cells_[i][j]->setY(cells_[i][j]->y() + height);
        std::vector<TextItem *> row;
        // номер строки, на которую можно сослаться для получения ширины и x
        // хотя бы одна строка всегда остается
        size_t refPos = (pos != 1) ? 0 : 1;
        for (size_t j = 0; j < cells_[0].size(); j++) {
            row.push_back(new TextItem(cells_[0][j]->width(), height));
            cellsFormatting_[j]->addItem(row[j]);
            row[j]->setTextRenderInfo(cellsFormatting_[j]);
            row[j]->setParentItem(this);
            row[j]->setPos(cells_[refPos][j]->x(), newHeight);
        }
        cells_.insert(cells_.begin() + (pos - 1), row);
        page_->update();
        updateCurrentAppendRow();
    }
}
// единственную строку удалить нельзя
void TableField::deleteRow(size_t pos) {
    if (cells_.size() > 1 && 1  <= pos && pos <= cells_.size()) {
        double rowHeight = cells_[pos - 1][0]->height();
        for (size_t j = 0; j < cells_[0].size(); j++) {
            cellsFormatting_[j]->removeItem(cells_[pos - 1][j]);
            cells_[pos - 1][j]->setParentItem(0);
            delete cells_[pos - 1][j];
        }
        Field::setHeight(height_ - rowHeight);
        cells_.erase(cells_.begin() + (pos - 1));
        for (size_t i = pos - 1; i < cells_.size(); i++)
            for (size_t j = 0; j < cells_[0].size(); j++)
                cells_[i][j]->setY(cells_[i][j]->y() - rowHeight);
        page_->update();
        updateCurrentAppendRow();
    }
}

double TableField::rowHeight(size_t pos) const {
    if (1  <= pos && pos <= cells_.size())
        return cells_[pos - 1][0]->height();
    else
        return 0.0;
}
// высота меняется
void TableField::resizeRow(size_t pos, double height) {
    if (1  <= pos && pos <= cells_.size()) {
        double delta = height - cells_[pos - 1][0]->height();
        Field::setHeight(height_ + delta);
        for (size_t i = pos; i < cells_.size(); i++)
            for (size_t j = 0; j < cells_[0].size(); j++)
                cells_[i][j]->setY(cells_[i][j]->y() + delta);
        for (size_t j = 0; j < cells_[0].size(); j++)
            cells_[pos - 1][j]->setHeight(height);
        page_->update();
    }
}

size_t TableField::currentRow() const {
    return currentRow_ + 1;
}

void TableField::setCurrentRow(size_t pos) {
    if (1  <= pos && pos <= cells_.size())
        currentRow_ = pos - 1;
    else
        currentRow_ = 0;
}

size_t TableField::numberOfRows() const {
    return cells_.size();
}

void TableField::addColumn(size_t pos, double width) {
    if (1  <= pos && pos <= cells_[0].size() + 1) {
        double newWidth = (pos != cells_[0].size() + 1)
            ? cells_[0][pos - 1]->x()
            : width_;
        Field::setWidth(width_ + width);
        for (size_t j = pos - 1; j < cells_[0].size(); j++)
            for (size_t i = 0; i < cells_.size(); i++)
                cells_[i][j]->setX(cells_[i][j]->x() + width);
        cellsFormatting_.insert(cellsFormatting_.begin() + (pos - 1),
            TextRenderInfo::defaults());
        // номер столбца, на который можно сослаться для получения высоты и y
        // хотя бы один столбец всегда остается
        size_t refPos = (pos != 1) ? 0 : 1;
        for (size_t i = 0; i < cells_.size(); i++) {
            cells_[i].insert(cells_[i].begin() + (pos - 1),
                new TextItem(width, cells_[i][0]->height()));
            cellsFormatting_[pos - 1]->addItem(cells_[i][pos - 1]);
            cells_[i][pos - 1]->setTextRenderInfo(cellsFormatting_[pos - 1]);
            cells_[i][pos - 1]->setParentItem(this);
            cells_[i][pos - 1]->setPos(newWidth, cells_[i][refPos]->y());
        }
        page_->update();
    }
}

void TableField::deleteColumn(size_t pos) {
    if (cells_[0].size() > 1 && 1  <= pos && pos <= cells_[0].size()) {
        double colWidth = cells_[0][pos - 1]->width();
        for (size_t i = 0; i < cells_.size(); i++) {
            cellsFormatting_[pos - 1]->removeItem(cells_[i][pos - 1]);
            cells_[i][pos - 1]->setParentItem(0);
            delete cells_[i][pos - 1];
            cells_[i].erase(cells_[i].begin() + (pos - 1));
        }
        cellsFormatting_.erase(cellsFormatting_.begin() + (pos - 1));
        Field::setWidth(width_ - colWidth);
        for (size_t j = pos - 1; j < cells_[0].size(); j++)
            for (size_t i = 0; i < cells_.size(); i++)
                cells_[i][j]->setX(cells_[i][j]->x() - colWidth);
        page_->update();
        updateCurrentAppendRow();
    }
}

double TableField::columnWidth(size_t pos) const {
    if (1  <= pos && pos <= cells_[0].size())
        return cells_[0][pos - 1]->width();
    else
        return 0.0;
}

void TableField::resizeColumn(size_t pos, double width) {
    if (1  <= pos && pos <= cells_[0].size()) {
        double delta = width - cells_[0][pos - 1]->width();
        Field::setWidth(width_ + delta);
        for (size_t j = pos; j < cells_[0].size(); j++)
            for (size_t i = 0; i < cells_.size(); i++)
                cells_[i][j]->setX(cells_[i][j]->x() + delta);
        for (size_t i = 0; i < cells_.size(); i++)
            cells_[i][pos - 1]->setWidth(width);
        page_->update();
    }
}

size_t TableField::currentColumn() const {
    return currentCol_ + 1;
}

void TableField::setCurrentColumn(size_t pos) {
    if (1  <= pos && pos <= cells_[0].size())
        currentCol_ = pos - 1;
    else
        currentCol_ = 0;
}

TextRenderInfo *TableField::columnFormatting(size_t pos) {
    if (1  <= pos && pos <= cells_[0].size())
        return cellsFormatting_[pos - 1];
    else
        return 0;
}

size_t TableField::numberOfColumns() const {
    return cells_[0].size();
}

void TableField::setText(size_t row, size_t column, const QString &text) {
    if (1  <= row && row <= cells_.size() &&
        1  <= column && column <= cells_[0].size())
    {
        cells_[row - 1][column - 1]->setText(text);
        update();
        updateCurrentAppendRow();
    }
}

QString TableField::text(size_t row, size_t column) const {
    if (1  <= row && row <= cells_.size() &&
        1  <= column && column <= cells_[0].size())
    {
        return cells_[row - 1][column - 1]->text();
    } else
        return "";
}

void TableField::updateCurrentAppendRow() {
    bool isEmpty = true;
    currentAppendRow_ = cells_.size();
    for (int i = cells_.size() - 1; isEmpty && i >= 0; i--) {
        for (size_t j = 0; isEmpty && j < cells_[0].size(); j++) {
            isEmpty = isEmpty && cells_[i][j]->text().isEmpty();
        }
        if (isEmpty) {
            currentAppendRow_ = (size_t) i;
        }
    }
}

void TableField::clear() {
    for (size_t i = 0; i < cells_.size(); i++) {
        for (size_t j = 0; j < cells_[0].size(); j++) {
            cells_[i][j]->setText("");
        }
    }
    currentAppendRow_ = 0;
}

bool TableField::appendData(QStringList row) {
    if ((size_t)row.size() != cells_[0].size() ||
        currentAppendRow_ == cells_.size())
    {
        return false;
    }
    for (size_t j = 0; j < cells_[0].size(); j++) {
        cells_[currentAppendRow_][j]->setText(row.at(j));
    }
    currentAppendRow_++;
    return true;
}

bool TableField::atEnd() const {
    return currentAppendRow_ == cells_.size();
}

void TableField::switchLocalFormatting(size_t column, bool isLocal) {
    if (1 <= column && column <= cells_[0].size()) {
        for (size_t i = 0; i < cells_.size(); i++) {
            cellsFormatting_[column - 1]->removeItem(cells_[i][column - 1]);
        }
        isLocalFormatting_[column - 1] = isLocal;
        if (isLocalFormatting_[column - 1]) {
            cellsFormatting_[column - 1] = &localCellsFormatting_[column - 1];
        } else {
            cellsFormatting_[column - 1] = TextRenderInfo::globals();
        }
        for (size_t i = 0; i < cells_.size(); i++) {
            cellsFormatting_[column - 1]->addItem(cells_[i][column - 1]);
            cells_[i][column - 1]->setTextRenderInfo(cellsFormatting_[column - 1]);
        }
        update();
    }
}

TextRenderInfo *TableField::localColumnFormatting(size_t pos) {
    if (1  <= pos && pos <= cells_[0].size())
        return &localCellsFormatting_[pos - 1];
    else
        return 0;
}
