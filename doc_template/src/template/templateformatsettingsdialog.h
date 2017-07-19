#ifndef TEMPLATEFORMATSETTINGSDIALOG_H
#define TEMPLATEFORMATSETTINGSDIALOG_H

#include <QDialog>

class QDialogButtonBox;
class QWidget;
class FormatFieldSettingsWidget;

class TemplateFormatSettingsDialog : public QDialog {

    Q_OBJECT

public:
    TemplateFormatSettingsDialog(QWidget *parent);
    virtual ~TemplateFormatSettingsDialog() {}
private:
    QDialogButtonBox *buttons_;
    FormatFieldSettingsWidget *widget_;
};

#endif // TEMPLATEFORMATSETTINGSDIALOG_H
