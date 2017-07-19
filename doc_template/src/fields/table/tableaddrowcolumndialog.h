#ifndef TABLEADDROWCOLUMNDIALOG_H
#define TABLEADDROWCOLUMNDIALOG_H

#include <QDialog>

class QDialogButtonBox;
class QSpinBox;
class QLineEdit;

class TableAddRowColumnDialog : public QDialog {

    Q_OBJECT

public:
    // FIXME Константы лучше объявлять заглавнми: ROW, а так их можно спутать с именами типов
    enum Mode {Row, Column};
    TableAddRowColumnDialog(Mode m, size_t maxPos, QWidget *parent);
    virtual ~TableAddRowColumnDialog();
    size_t pos() const;
    double size() const;
protected slots:
    virtual void accept();
private:
    Q_DISABLE_COPY(TableAddRowColumnDialog);

    QSpinBox *posSelector_;
    QLineEdit *size_;
    QDialogButtonBox *buttons_;
};

#endif // TABLEADDROWCOLUMNDIALOG_H
