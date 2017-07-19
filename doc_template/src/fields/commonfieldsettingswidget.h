#ifndef COMMONFIELDSETTINGSWIDGET_H
#define COMMONFIELDSETTINGSWIDGET_H

#include <QWidget>
#include <QString>

class QLineEdit;
class QComboBox;
class QDoubleSpinBox;


class CommonFieldSettingsWidget : public QWidget {

    Q_OBJECT

public:
    explicit CommonFieldSettingsWidget(QWidget *parent = 0);
    ~CommonFieldSettingsWidget() {};
    // FIXME const QString&
    // получение/изменение атрибутов
    QString name() const;
    bool setName(const QString &name);
    // FIXME Зачем? Поему не вызвать name().empty()
    bool isNameClear() const;
    void resetName();
    void setPages(size_t count);
    int pageNumber() const;
    bool setPageNumber(int pageNumber);
    double x() const;
    bool setX(double x);
    // FIXME у тебя получается по два сеттера на каждую переменную. Может быть их можно объединить?
    void resetX();
    // FIXME во-первых, кажется, чтов все is*Clear - это у тебя protected методы
    // и в интерфейс торчать не должны. Во-вторых, потомки могут сами обратиться к полям
    // и им эти двустрочные методы не нужны
    bool isXClear() const;
    double y() const;
    bool setY(double y);
    void resetY();
    bool isYClear() const;
    double width() const;
    bool setWidth(double width);
    void resetWidth();
    bool isWidthClear() const;
    double height() const;
    bool setHeight(double height);
    void resetHeight();
    bool isHeightClear() const;
    void setEnabled(bool isEnabled);
signals:
    void nameChanged(const QString &);
    void pageNumberChanged(int);
    void xPosChanged(double);
    void yPosChanged(double);
    void widthChanged(double);
    void heightChanged(double);
protected slots:
    void nameChanged_();
    void pageNumberChanged_(int);
    // FIXME А чем эти слоты отличаются от setX, setY и подобных? Их нельзя обединить
    void xPosChanged_(double);
    void yPosChanged_(double);
    void widthChanged_(double);
    void heightChanged_(double);
protected:
    // общие настройки для всех полей
    QLineEdit *name_;
    QComboBox *pageNumber_;
    QDoubleSpinBox *xPos_, *yPos_;
    QDoubleSpinBox *width_, *height_;
private:
    Q_DISABLE_COPY(CommonFieldSettingsWidget);

    //FIXME Ну хоть между методами и свойствами строку ставь.
    void generateLayout();

    // FIXME это очень плохой флаг. Вот вылетет исключение и никто его обратно не
    // изменит. И такие вещи очень мешают сделать программу многопоточной. По сути - это
    // передача параметра в функцию через глобальную переменную
    bool isSetProgrammaticaly_;
};

#endif // COMMONFIELDSETTINGSWIDGET_H
