#ifndef FIELDCREATIONDIALOG_H
#define FIELDCREATIONDIALOG_H

#include <QDialog>
#include <QString>

#include <memory>

class FieldController;
class Field;
class CommonFieldSettingsWidget;
class QWidget;
class QDialogButtonBox;

class FieldCreationDialog : public QDialog {

    Q_OBJECT

public:
    virtual ~FieldCreationDialog();
    // FIXME судя по описаниям, следующие три метода должны быть protected
    /**
      Возвращает виджет с общими настройками
      Дочерние классы могут разместить его в своей layout требуемым способом
      */
    QWidget *commonSettingsWidget();
    /**
      Возвращает набор кнопок, связанных с нужными слотами
      Дочерние классы могут разместить его в своей layout требуемым способом
      */
    QDialogButtonBox *buttons();
    /**
      Проверяет наличие незаполненных полей во вводе
      Возвращает false, если есть незаполненные поля
      */
    virtual bool checkNonEmptyInput();
    /**
      Возвращает созданное поле
      */
    std::unique_ptr<Field> createdField();
    // FIXME А зачем кому-то получать от виджета контроллер. У тебя и так цикл:
    // контроллер создает виджет и виджет его обратно пинает. А ты этот цикл наружу показываешь
    /**
      Возвращает контроллер поля
      Переопределяется во всех производных классах
      */
    virtual FieldController *controller() = 0;
    // FIXME четыре метода на одно свойство - это многовато. Получается куча кода.
    // получение/изменение атрибутов
    // имя
    bool setName(const QString &name);
    void resetName();
    QString name() const;
    bool isNameClear() const;
    // страницы
    void setPages(size_t count);
    bool setPageNumber(int pageNumber);
    int pageNumber() const;
    // x координата поля
    bool setX(double x);
    void resetX();
    double x() const;
    bool isXClear() const;
    // y координата поля
    bool setY(double y);
    void resetY();
    double y() const;
    bool isYClear() const;
    // ширина
    bool setWidth(double width);
    void resetWidth();
    double width() const;
    bool isWidthClear() const;
    // высота
    bool setHeight(double height);
    void resetHeight();
    double height() const;
    bool isHeightClear() const;
protected slots:
    virtual void accept();
protected:
    explicit FieldCreationDialog(QWidget *parent);
    /**
      Возвращает поле, созданное с введенными в диалог данными
      Если метод exec не был вызван или сработал слот reject,
      возвращается нулевой указатель
      */
    virtual std::unique_ptr<Field> createField() = 0;
private:
    Q_DISABLE_COPY(FieldCreationDialog);

    CommonFieldSettingsWidget *widget_;
    QDialogButtonBox *buttons_;
    std::unique_ptr<Field> createdField_;
};

#endif // FIELDCREATIONDIALOG_H
