#include "bypagemode.h"

#include "templatelayout.h"
#include "templateview.h"

#include "page.h"
#include "field.h"

#include <QGraphicsScene>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QScrollBar>

ByPageMode *ByPageMode::instance_ = 0;

TemplateViewMode *ByPageMode::instance() {

    if (!instance_)
        instance_ = new ByPageMode();
    return instance_;
}

void ByPageMode::adjustCurrentField(TemplateLayout *tl, TemplateView *tv) {

    if (tl) {
        if (tv->currentField() && tv->currentPage() &&
            tv->currentPage() != tv->currentField()->page())
        {
            changeCurrentPage(tv, tv->currentField()->page());
        }
    }
}

/**
  Вызывается при установлении новой текущей страницы извне
  viewport устанавливается на верхнюю границу страницы
  */
void ByPageMode::adjustCurrentPage(TemplateLayout *tl, TemplateView *tv) {

    if (tl && tv) {
        //QRectF sr = tv->scene()->sceneRect();
        double halfPS = tl->pageSeparator() / 2.0;
        //double ratio = tv->viewport()->size().width() / (double)
        //    tv->viewport()->size().height();
        Page *p = tv->currentPage();
        QScrollBar *vertical = tv->verticalScrollBar();
        vertical->setValue((vertical->maximum() - vertical->minimum()) /
            tl->numberOfPages() * p->num());
        //tv->fitInView(p->x() - halfPS, p->y() - halfPS,
        //    p->width() + 2.0 * halfPS, p->height() + 2.0 * halfPS,
        //    Qt::KeepAspectRatio);
        //tv->ensureVisible(0.0, tv->currentPage()->y() - halfPS, sr.x(),
        //    halfPS * 2.0, 0, 0);
        if (tv->currentField() && tv->currentField()->page() != tv->currentPage())
            changeCurrentField(tv, 0);
    }
}

void ByPageMode::processWheelEvent(TemplateLayout *tl, TemplateView *tv,
    QWheelEvent *event)
{
    TemplateViewMode::processWheelEvent(tl, tv, event);
}

void ByPageMode::processResizeEvent(TemplateLayout *tl, TemplateView *tv,
    QResizeEvent *event)
{
    if (tl && tv->currentPage()) {
        double halfPS = tl->pageSeparator() / 2.0;
        Page *p = tv->currentPage();
        tv->fitInView(p->x() - halfPS, p->y() - halfPS,
            p->width() + 2.0 * halfPS, p->height() + 2.0 * halfPS,
            Qt::KeepAspectRatio);
    } else // по умолчанию
        TemplateViewMode::processResizeEvent(tl, tv, event);
}

void ByPageMode::processScrollByEvent(TemplateLayout *tl, TemplateView *tv,
    int dx, int dy)
{
    //
    if (tl) {
        Page *p = tv->currentPage();
        QScrollBar *vertical = tv->verticalScrollBar();
        if ((vertical->maximum() - vertical->minimum()) / tl->numberOfPages() >= 1) {
            size_t newNum = vertical->value() /
                            ((vertical->maximum() - vertical->minimum()) / tl->numberOfPages())
                            + 1;
            Page *newCurrent = tl->page(vertical->value() /
                ((vertical->maximum() - vertical->minimum()) / tl->numberOfPages())
                + 1);
                if (newCurrent != p) {
                    changeCurrentPage(tv, newCurrent);
                    adjustCurrentPage(tl, tv);
                    if (tv->currentField() &&
                        tv->currentField()->page() != tv->currentPage())
                    {
                        changeCurrentField(tv, 0);
                    }
                }
        }
    } else
        TemplateViewMode::processScrollByEvent(tl, tv, dx, dy);
}

void ByPageMode::processDoubleClickEvent(TemplateLayout *tl, TemplateView *tv,
    QMouseEvent *event)
{
    // страницы определяются только скроллом
    // двойным кликом выбираются поля
    Field *field = getFieldAt(tv, event->pos());
    if (field && field != tv->currentField()) {
        changeCurrentField(tv, field);
        adjustCurrentField(tl, tv);
    } else {
        changeCurrentField(tv, 0);
    }
}

void ByPageMode::init(TemplateLayout *tl, TemplateView *tv) {

    if (tv) {
        tv->setDragMode(QGraphicsView::NoDrag);
        tv->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        tv->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        tv->horizontalScrollBar()->hide();
        if (tv->currentField())
            adjustCurrentField(tl, tv);
        else
            adjustCurrentPage(tl, tv);
    }
}
