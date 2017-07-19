#ifndef BYPAGEMODE_H
#define BYPAGEMODE_H

#include "templateviewmode.h"

class TemplateLayout;
class TemplateView;

class QWheelEvent;
class QMouseEvent;
class QResizeEvent;

class ByPageMode : public TemplateViewMode {
public:
    static TemplateViewMode *instance();

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
    virtual void init(TemplateLayout *tl, TemplateView *tv);
protected:
    ByPageMode() {};
private:
    static ByPageMode *instance_;
};

#endif // BYPAGEMODE_H
