#ifndef SOURCEDIALOG_H
#define SOURCEDIALOG_H

#include "../template/sourcenamevalidator.h"

#include <QDialog>
#include <QString>

class QWidget;
class QDialogButtonBox;
class QLineEdit;
class Source;

class SourceDialog : public QDialog {

    Q_OBJECT

public:
    virtual ~SourceDialog() {}

    QString name() const;
    void setName(const QString &name);
    QWidget *nameWidget();
    QDialogButtonBox *buttons();
protected slots:
    virtual void accept() = 0;
protected:
    SourceDialog(SourceNameValidator validator,
        QWidget *parent);
    SourceDialog(SourceNameValidator validator,
        Source *source, QWidget *parent);

    virtual Source *source() = 0;
    virtual bool checkNonEmptyInput();
    virtual bool checkData();

    QLineEdit *name_;
    QWidget *nameWidget_;
    QDialogButtonBox *buttons_;
    SourceNameValidator validator_;
private:
    void createWidgets();
};

#endif // SOURCEDIALOG_H
