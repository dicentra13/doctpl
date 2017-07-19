#include "templatelayout.h"

#include "../fields/field.h"
#include "../page.h"
#include "templateview.h"
#include "templateviewmode.h"

#include <QGraphicsScene>
#include <QGLWidget>
#include <QPrinter>
#include <QPainter>
#include <QPen>
#include <QList>
#include  <QFrame>

#include <list>
#include <iostream>

const double TemplateLayout::pageSeparator_ = 4.0;

/** Передача 0 в параметре mode означает работу без оконного режима
    вид не создается, view() возвращает 0
 */
TemplateLayout::TemplateLayout(const QString &name, TemplateViewMode *mode)
    : name_(name)
    , scene_(new QGraphicsScene)
    , isModified_(false)
    , currentField_(0)
    , currentPage_(0)
    , isCurrentPageChanging_(false)
    , isCurrentFieldChanging_(false)
{
    if (mode) {
        view_ = new TemplateView(this, mode);
        view_->setViewport(new QFrame(view_));
        view_->setRenderHint(QPainter::HighQualityAntialiasing, true);
        view_->setRenderHint(QPainter::SmoothPixmapTransform, true);
        view_->setScene(scene_.get());
        connect(view_, SIGNAL(fieldSelected(Field*)),
            this, SLOT(currentFieldChanged_(Field*)));
        connect(view_, SIGNAL(pageSelected(Page*)),
            this, SLOT(currentPageChanged_(Page*)));
    } else {
        view_ = 0;
    }
}

TemplateLayout::~TemplateLayout()
{}

bool TemplateLayout::addPage(size_t num, std::unique_ptr<Page> p) {
    bool result;
    if (1 <= num && num <= pages_.size() + 1) {
        std::vector<Page *>::iterator pi = pages_.begin() + num - 1;
        double height = (pi == pages_.begin())
            ? 0.0
            : (*(pi - 1))->y() + (*(pi - 1))->height() + pageSeparator_ ;
        pi = pages_.insert(pi, p.get());
        // графика
        updateSceneRect();
        for (size_t i = num; i < pages_.size(); i++) {
            pages_[i]->setY(pages_[i]->y() + pageSeparator_ + p->height());
            pages_[i]->setNum(i + 1);
        }
        scene_->addItem(p.release());
        // центр страниц выровнен по x = 0.0 в координатах сцены
        (*pi)->setPos(-(*pi)->width() / 2.0, height);
        (*pi)->setNum(num);
        setModified(true);
        result = true;
    } else
        result = false;
    return result;
}

bool TemplateLayout::deletePage(size_t num) {
    bool result;
    if (1 <= num && num <= pages_.size()) {
        std::vector<Page *>::iterator pi = pages_.begin() + num - 1;
        Page *p = *pi;
        pages_.erase(pi);
        // графика
        scene_->removeItem(p);
        for (size_t i = num - 1; i < pages_.size(); i++) {
            pages_[i]->setY(pages_[i]->y() - pageSeparator_ - p->height());
            pages_[i]->setNum(i + 1);
        }
        if (p == currentPage_) {
            setCurrentPage(pageOutOfRange());
            emit currentPageChanged(pageOutOfRange());
        }
        // удалить поля
        std::list<Field *> fields = p->fields();
        std::list<Field *>::iterator fi = fields.begin();
        for (; fi != fields.end(); fi++)
            deleteField((*fi)->name());
        delete p;
        updateSceneRect();
        setModified(true);
        result = true;
    } else
        result = false;
    return result;
}

Page *TemplateLayout::page(size_t num) const {
    return (1 <= num && num <= pages_.size()) ? pages_[num - 1] : 0;
}

bool TemplateLayout::pageExists(size_t num) const {
    return 1 <= num && num <= pages_.size();
}

size_t TemplateLayout::firstPage() const {
    return (pages_.size() != 0) ? 1 : pageOutOfRange();
}

size_t TemplateLayout::lastPage() const {
    return (pages_.size() != 0) ? pages_.size() : pageOutOfRange();
}

size_t TemplateLayout::pageOutOfRange() const {
    return 0;
}

bool TemplateLayout::movePage(size_t oldNum, size_t newNum) {
    bool result;
    if (1 <= oldNum && oldNum <= pages_.size() &&
        1 <= newNum && newNum <= pages_.size())
    {
        if (oldNum != newNum) {
            Page *moved = pages_[oldNum - 1];
            double newY;
            if (oldNum < newNum) {
                newY = pages_[newNum -1]->y() +
                    pages_[newNum -1]->height() - moved->height();
                for (size_t i = oldNum - 1; i <= newNum - 2; i++) {
                    pages_[i] = pages_[i + 1];
                    pages_[i]->setY(pages_[i]->y() - pageSeparator_ -
                        moved->height());
                    pages_[i]->setNum(i + 1);
                }
            } else {// oldNum > newNum
                newY = pages_[newNum - 1]->y();
                for (size_t i = oldNum - 1; i >= newNum; i--) {
                    pages_[i] = pages_[i - 1];
                    pages_[i]->setY(pages_[i]->y() + pageSeparator_ +
                       moved->height());
                    pages_[i]->setNum(i + 1);
                }
            }
            pages_[newNum - 1] = moved;
            pages_[newNum - 1]->setY(newY);
            pages_[newNum - 1]->setNum(newNum);
            setModified(true);
            result = true;
        } else
            result = false;
    } else
        result = false;
    return result;
}

bool TemplateLayout::resizePage(size_t num, double width, double height,
    double dx, double dy)
{
    bool result;
    if (1 <= num && num <= pages_.size()) {
        if (Page::checkGeometryAvailable(pages_[num - 1], width, height,
            dx, dy))
        {
            for (size_t i = num; i < pages_.size(); i++) {
                pages_[i]->setY(pages_[i]->y() - pages_[num - 1]->height() +
                    height);
            }
            pages_[num - 1]->setWidth(width);
            pages_[num - 1]->setHeight(height);
            pages_[num - 1]->setX(-width / 2.0);
            pages_[num - 1]->setDx(dx);
            pages_[num - 1]->setDy(dy);
            updateSceneRect();
            setModified(true);
            result = true;
        } else
            result = false;
    } else
        result = false;
    return result;
}

bool TemplateLayout::addField(const QString &name, std::unique_ptr<Field> field,
    size_t num, double x, double y)
{
    std::map<QString, Field *>::iterator i =
        fields_.find(name);
    if (i == fields_.end() && 1 <= num && num <= pages_.size()) {
        Field *f = field.release();
        fields_.insert(std::pair<QString, Field *>(name, f));
        f->setName(name);
        pages_[num - 1]->addField(x, y, f);
        setModified(true);
        return true;
    } else
        return false;
}

bool TemplateLayout::deleteField(const QString &name) {
    std::map<QString, Field *>::iterator i =
        fields_.find(name);
    if (i != fields_.end()) {
        Field *f = i->second;
        fields_.erase(i);
        f->setParentItem(0);
        if (f == currentField_) {
            setCurrentField(fieldOutOfRange());
            emit currentFieldChanged(fieldOutOfRange());
        }
        delete f;
        setModified(true);
        return true;
    } else
        return false;
}

Field *TemplateLayout::field(const QString &name) const {
    std::map<QString, Field *>::const_iterator i =
        fields_.find(name);
    return (i != fields_.end()) ? i->second : 0;
}

bool TemplateLayout::fieldExists(const QString &name) const {
    return !fields_.empty() && fields_.find(name) != fields_.end();
}

bool TemplateLayout::renameField(const QString &oldName,
    const QString &newName)
{
    std::map<QString, Field *>::iterator o =
        fields_.find(oldName);
    if (o != fields_.end() && fields_.find(newName) == fields_.end()) {
        Field *f = o->second;
        fields_.erase(o);
        f->setName(newName);
        fields_.insert(std::pair<QString, Field *>(newName, f));
        setModified(true);
        return true;
    } else
        return false;
}

bool TemplateLayout::moveField(const QString &name, size_t newPageNum) {
    std::map<QString, Field *>::iterator i =
        fields_.find(name);
    if (i != fields_.end() && 1 <= newPageNum && newPageNum <= pages_.size()) {
        double x = i->second->x();
        double y = i->second->y();
        i->second->setPage(pages_[newPageNum - 1]);
        i->second->setParentItem(pages_[newPageNum - 1]);
        i->second->setPos(x, y);
        setModified(true);
        return true;
    } else
        return false;
}

QStringList TemplateLayout::fieldNames() const {
    QStringList names;
    std::map<QString, Field *>::const_iterator i;
    for (i = fields_.begin(); i != fields_.end(); i++)
        names.push_back(i->first);
    return names;
}

QString TemplateLayout::fieldOutOfRange() const {
    // создание поля без имени не допускается
    return "";
}

TemplateView *TemplateLayout::view() {
    return view_;
}

void TemplateLayout::printToFile(const QString &fileName,
    std::vector<size_t> pages)
    const
{
    if (pages.empty())
        THROW_EX(EmptyPageSet, "Page set must not be empty");
    // считаем, что список не пуст
    Field::setFinalPrinting(true);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName(fileName);
    QPainter painter;
    QPen pen;
    setPageParams(printer, *(pages.begin()));
    painter.begin(&printer);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    pen.setWidthF(0.1);
    painter.setPen(pen);
    std::vector<size_t>::const_iterator i = pages.begin();
    while (i != pages.end()) {
        printPage(printer, painter, *i);
        i++;
        if (i != pages.end()) {
            setPageParams(printer, *i);
            printer.newPage();
        }
    }
    painter.end();
    Field::setFinalPrinting(false);
}

void TemplateLayout::setPageParams(QPrinter &printer, size_t pageNum) const {
    printer.setOrientation(pages_[pageNum - 1]->orientation());
    printer.setPaperSize(QSizeF(pages_[pageNum - 1]->width(),
        pages_[pageNum - 1]->height()), QPrinter::Millimeter);
    printer.setPageMargins(0.0, 0.0, 0.0, 0.0, QPrinter::Millimeter);
}

void TemplateLayout::printPage(QPrinter &printer, QPainter &painter,
    size_t pageNum) const
{
    painter.setViewport(0, 0, printer.width(), printer.height());
    painter.setWindow(0, 0, printer.widthMM(), printer.heightMM());
    scene_->render(&painter, printer.paperRect(QPrinter::Millimeter),
        QRectF(pages_[pageNum - 1]->x(), pages_[pageNum - 1]->y(),
            pages_[pageNum - 1]->width(), pages_[pageNum - 1]->height()),
            Qt::KeepAspectRatio);
}

void TemplateLayout::updateSceneRect() {
    double newWidth = 0.0, newHeight = 0.0;
    if (!pages_.empty()) {
        for (size_t i = 0; i < pages_.size(); i++) {
            if (pages_[i]->width() > newWidth)
                newWidth = pages_[i]->width();
            newHeight += pages_[i]->height() + pageSeparator_;
        }
    }
    scene_.get()->setSceneRect(QRectF(
        QPointF(-(newWidth + pageSeparator_) / 2.0, -pageSeparator_ / 2.0),
        QSizeF(newWidth + pageSeparator_, newHeight)));
}

QString TemplateLayout::currentField() const {
    return (currentField_) ? currentField_->name() : fieldOutOfRange();
}

void TemplateLayout::setCurrentField(const QString &name) {
    isCurrentFieldChanging_ = true;
    if (currentField_) {
        currentField_->setCurrent(false);
        currentField_->update();
    }
    currentField_ = field(name);
    if (currentField_) {
        currentField_->setCurrent(true);
        currentField_->update();
    }
    if (view_) {
        view_->adjustCurrentField();
    }
    isCurrentFieldChanging_ = false;
}

size_t TemplateLayout::currentPage() const {
    return (currentPage_) ? currentPage_->num() : pageOutOfRange();
}

void TemplateLayout::setCurrentPage(size_t pageNumber) {
    isCurrentPageChanging_ = true;
    if (currentPage_) {
        currentPage_->setCurrent(false);
        currentPage_->update();
    }
    currentPage_ = page(pageNumber);
    if (currentPage_) {
        currentPage_->setCurrent(true);
        currentPage_->update();
    }
    if (view_) {
        view_->adjustCurrentPage();
    }
    isCurrentPageChanging_ = false;
}

void TemplateLayout::currentPageChanged_(Page *p) {
    if (!isCurrentPageChanging_) {
        if (currentPage_) {
            currentPage_->setCurrent(false);
            currentPage_->update();
        }
        currentPage_ = p;
        if (currentPage_) {
            currentPage_->setCurrent(true);
            currentPage_->update();
        }
        size_t newNum = (currentPage_) ? currentPage_->num() : pageOutOfRange();
        emit currentPageChanged(newNum);
    }
}

void TemplateLayout::currentFieldChanged_(Field *f) {
    if (!isCurrentFieldChanging_) {
        if (currentField_) {
            currentField_->setCurrent(false);
            currentField_->update();
        }
        currentField_ = f;
        if (currentField_) {
            currentField_->setCurrent(true);
            currentField_->update();
        }
        QString newName = (currentField_) ? currentField_->name()
            : fieldOutOfRange();
        emit currentFieldChanged(newName);
    }
}
