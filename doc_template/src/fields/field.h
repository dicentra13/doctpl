#ifndef FIELD_H
#define FIELD_H

#include "../template/templatelayoutitem.h"

#include <QString>

//#include <boost/noncopyable.hpp>

class Page;
class Data;
class FieldController;

// FIXME без проверок на наложение и правильное расположение на странице
// FIXME Пусть накладываются. Почему  не имею права добавить, скажем поле типа "картинка"
// и поверх поле типа "текст"?

class Field : public TemplateLayoutItem/*, private boost::noncopyable*/ {
public:
    virtual ~Field() {
    // TODO FIXME удалить ссылки на себя из контроллеров
    // Эта фараза пишется комментарием к деструктору и деструктор делается чисто виртуальным
    // чтобы дочерние поля обязаны были его определить

    };
    virtual FieldController *controller() const = 0;

    QString fieldType() const {
        return type_;
    }

    Page *page() const {
        return page_;
    }
    void setPage(Page *p) {
        page_ = p;
    }

    void setName(const QString &name) {
        name_ = name;
    }
    QString name() const {
        return name_;
    }
    virtual void clear() = 0;

    static void setFinalPrinting(bool f) {
        isFinalPrinting_ = f;
    }

    static bool isFinalPrinting() {
        return isFinalPrinting_;
    }
protected:
    // FIXME а type_ кто будет устанавливать? Либо в конструктор, либо просто сделать функцию type()
    // виртуальной и убрать поле type_. А сейчас ты можешь забыть его установить
    Field(const QString &name, double width, double height);

    QString name_;
    QString type_;
    // FIXME а бывает поле без страницы? Если нет, то может быть можно страницу в конструктор передавать.
    Page *page_; // отсюда доступ к items
    // x и y хранятся в QGraphicsItem
    static bool isFinalPrinting_;
};

#endif // FIELD_H
