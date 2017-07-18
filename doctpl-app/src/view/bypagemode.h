#ifndef BYPAGEMODE_H
#define BYPAGEMODE_H

#include "Viewmode.h"

class TemplateLayout;
class View;

class QWheelEvent;
class QMouseEvent;
class QResizeEvent;

class ByPageMode : public ViewMode {
public:
    static ViewMode *instance();

    virtual void adjustCurrentField(TemplateLayout *tl, View *tv);
    virtual void adjustCurrentPage(TemplateLayout *tl, View *tv);
    virtual void processWheelEvent(TemplateLayout *tl, View *tv,
        QWheelEvent *event);
    virtual void processResizeEvent(TemplateLayout *tl, View *tv,
        QResizeEvent *event);
    virtual void processScrollByEvent(TemplateLayout *tl, View *tv,
        int dx, int dy);
    virtual void processDoubleClickEvent(TemplateLayout *tl, View *tv,
        QMouseEvent *event);
    virtual void init(TemplateLayout *tl, View *tv);
protected:
    ByPageMode() {};
private:
    static ByPageMode *instance_;
};

#endif // BYPAGEMODE_H
