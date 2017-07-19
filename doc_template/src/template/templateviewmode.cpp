#include "templateviewmode.h"

#include "templatelayout.h"
#include "templateview.h"
#include "../page.h"
#include "../fields/field.h"

#include <QWheelEvent>
#include <QResizeEvent>
#include <QMouseEvent>

void TemplateViewMode::adjustCurrentField(TemplateLayout */*tl*/,
    TemplateView */*tv*/)
{}

void TemplateViewMode::adjustCurrentPage(TemplateLayout */*tl*/,
    TemplateView */*tv*/)
{}

void TemplateViewMode::processWheelEvent(TemplateLayout */*tl*/,
    TemplateView *tv, QWheelEvent *event)
{
    tv->QGraphicsView::wheelEvent(event);
}

void TemplateViewMode::processResizeEvent(TemplateLayout */*tl*/,
    TemplateView *tv, QResizeEvent *event)
{
    tv->QGraphicsView::resizeEvent(event);
}

void TemplateViewMode::processScrollByEvent(TemplateLayout */*tl*/,
    TemplateView *tv, int dx, int dy)
{
    tv->QGraphicsView::scrollContentsBy(dx, dy);
}

void TemplateViewMode::processDoubleClickEvent(TemplateLayout */*tl*/,
    TemplateView *tv, QMouseEvent *event)
{
    tv->QGraphicsView::mouseDoubleClickEvent(event);
}

void TemplateViewMode::changeMode(TemplateView *tv, TemplateViewMode *newMode) {

    tv->setMode(newMode);
}

void TemplateViewMode::changeCurrentPage(TemplateView *tv, Page *newCurrent) {

    tv->selectedPageChanged(newCurrent);
}

void TemplateViewMode::changeCurrentField(TemplateView *tv, Field *newCurrent) {

    tv->selectedFieldChanged(newCurrent);
}

Page *TemplateViewMode::getPageAt(TemplateView *tv, const QPoint &pos) {

    QList<QGraphicsItem *> il(tv->items(pos));
    QList<QGraphicsItem *>::iterator i = il.begin();
    Page *pAtPos = 0;
    while (i != il.end() && !(pAtPos = dynamic_cast<Page *>(*i)))
        i++;
    return pAtPos;
}

Field *TemplateViewMode::getFieldAt(TemplateView *tv, const QPoint &pos) {

    QList<QGraphicsItem *> il(tv->items(pos));
    QList<QGraphicsItem *>::iterator i = il.begin();
    Field *fAtPos = 0;
    while (i != il.end() && !(fAtPos = dynamic_cast<Field *>(*i)))
        i++;
    return fAtPos;
}
