#ifndef TYPESELECTIONDIALOG_H
#define TYPESELECTIONDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>

class QComboBox;

class TypeSelectionDialog : public QDialog {

    Q_OBJECT

public:
    TypeSelectionDialog(const QString &title, QStringList types,
        QWidget *parent);
    virtual ~TypeSelectionDialog();
    QString selectedType() const;
private:
    TypeSelectionDialog(const TypeSelectionDialog &);
    TypeSelectionDialog operator=(const TypeSelectionDialog &);

    QComboBox *typeSelector_;
};

#endif // TYPESELECTIONDIALOG_H
