#ifndef TEMPLATEVIEWMODE_H
#define TEMPLATEVIEWMODE_H

#include <QPoint>

class TemplateLayout;
class TemplateView;
class Page;
class Field;

class QWheelEvent;
class QMouseEvent;
class QResizeEvent;

class TemplateViewMode {
public:
    virtual void adjustCurrentField(TemplateLayout *tl, TemplateView *tv);
    virtual void adjustCurrentPage(TemplateLayout *tl, TemplateView *tv);
    virtual void processWheelEvent(TemplateLayout *tl, TemplateView *tv,
        QWheelEvent *event);
    virtual void processResizeEvent(TemplateLayout *tl, TemplateView *tv,
        QResizeEvent *event);
    virtual void processScrollByEvent(TemplateLayout *tl, TemplateView *tv,
        int dx, int dy);
    virtual void processDoubleClickEvent(TemplateLayout *tl, TemplateView *tv,
        QMouseEvent *event);
    virtual void adjust(TemplateLayout *, TemplateView *) {};
protected:
    TemplateViewMode() {};
    void changeMode(TemplateView *tv, TemplateViewMode *newMode);
    void changeCurrentPage(TemplateView *tv, Page *newCurrent);
    void changeCurrentField(TemplateView *tv, Field *newCurrent);
    Page *getPageAt(TemplateView *tv, const QPoint &pos);
    Field *getFieldAt(TemplateView *tv, const QPoint &pos);
private:
    TemplateViewMode(const TemplateViewMode &);
    TemplateViewMode &operator=(const TemplateViewMode &);
};

#endif // TEMPLATEVIEWMODE_H
