#ifndef TEXTFIELDWIDGET_H
#define TEXTFIELDWIDGET_H

#include "../fieldsettingswidget.h"
#include <QString>

class CommonFieldSettingsWidget;
class FormatFieldSettingsWidget;
class TextField;
class TextFieldController;
class TextRenderInfo;

class QLineEdit;
class QCheckBox;

class TextFieldWidget : public FieldSettingsWidget {

    Q_OBJECT

public:
    TextFieldWidget(TextFieldController *controller, QWidget *parent);
    virtual ~TextFieldWidget();
    // FIXME в private
    virtual void setEnabled(bool);
    void setField(TextField *field);
    void setFormatting(TextRenderInfo *format);
signals:
    void textChanged(QString);
    void settingsSetToLocal(bool isLocal);
protected slots:
    void textChanged_(const QString &newText);
    void settingsSwitched(bool toLocal);
private:
    TextFieldController *controller_;
    QCheckBox *localSettingsSelector_;
    FormatFieldSettingsWidget *formatSettings_;
    QLineEdit *text_;
};

#endif // TEXTFIELDWIDGET_H
