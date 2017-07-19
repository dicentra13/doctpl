#ifndef TABLEFIELDWIDGET_H
#define TABLEFIELDWIDGET_H

#include "../fieldsettingswidget.h"
#include <QString>

//class CommonFieldSettingsWidget;
class FormatFieldSettingsWidget;
class TableField;
class TableFieldController;
class TextRenderInfo;

class QLineEdit;
class QSpinBox;
class QPushButton;
class QLabel;
class QDoubleSpinBox;
class QCheckBox;

class TableFieldWidget : public FieldSettingsWidget {

    Q_OBJECT

public:
    TableFieldWidget(TableFieldController *controller, QWidget *parent);
    virtual ~TableFieldWidget();
    virtual void setEnabled(bool);
    void setField(TableField *field);
    void setNumberOfRows(size_t num);
    // FIXME у тебя же уже есть такие слоты. СЛоты можно вызывать как методы. Не надо делать по два одинаковых метода у класса
    void setCurrentRow(size_t row);
    void setCurrentRowHeight(double height);
    void setNumberOfColumns(size_t num);
    void setCurrentColumn(size_t col);
    void setCurrentColumnWidth(double width);
    void setCurrentColumnFormatSettings(TextRenderInfo *formatSettings, bool isLocal);
    void setText(const QString &text);
    void setFormatting(TextRenderInfo *format);
signals:
    void addRow(size_t, double);
    void removeRow(size_t);
    void resizeRow(size_t, double);
    void addColumn(size_t, double);
    void removeColumn(size_t);
    void resizeColumn(size_t, double);
    void currentRowChanged(size_t);
    void currentColumnChanged(size_t);
    void textChanged(size_t, size_t, QString);
    void switchSettingsToLocal(size_t, bool);
    //void setCrossEnabled(bool);
protected slots:
    void textChanged_(QString);
    void settingsSwitchedToLocal(bool);
    void addRow();
    void removeRow();
    void addColumn();
    void removeColumn();
    void currentRowChanged(int);
    void currentColumnChanged(int);
    void currentRowHeightEdited(double);
    void currentColumnWidthEdited(double);
    //void crossModeChanged(bool isCross);
private:
    void generateLayout();

    TableFieldController *controller_;
    FormatFieldSettingsWidget *formatSettings_;

    QLineEdit *text_;
    QCheckBox *isLocalFormatting_;
    QLabel *numberOfRows_, *numberOfColumns_;
    QDoubleSpinBox *rowHeight_, *columnWidth_;
    QSpinBox *rowSelector_, *columnSelector_;
    QPushButton *addRow_, *removeRow_, *addColumn_, *removeColumn_;
    //QCheckBox *crossEmptyLines_;
    // FIXME ты используешь этот флаг не для того, чтобы описать состояние объекта, а чтобы
    // передавать параметры в методы. Это очень плохо. Во-первых у тебя все методы стали нереентерабельными.
    // Об этом надо предупреждать большими буквами в описании методов. Во-вторых это не очевидно, не потокобезопасно, не exception безопасно.
    // И, наконец, если значение поменяли программно, его все равно поменяли и те, кто
    // отслеживают его изменение должны полчить сигнал. Иначе у тебя получается, что вместо того, чтобы поменять значение и дать ему райзойтись сигналами,
    // каждый твой объект знает кто с кем связан и сам всем устанавливает значения. Добавление нового объекта - переписывание всех остальных.
    // Сигнал все равно НАО кидать, а чтобы он не циклился, в слотах надо проверять, меняется ли значение на самом деле и, если не меняется,
    // ничего не делать
    bool isSignalsBlocked_;
};

#endif // TABLEFIELDWIDGET_H
