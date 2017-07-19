#ifndef TABLESOURCEDIALOG_H
#define TABLESOURCEDIALOG_H

#include "../sourcedialog.h"

#include <memory>

class TableSource;
class QSpinBox;
class QWidget;

class TableSourceDialog : public SourceDialog {

    Q_OBJECT

public:
    virtual ~TableSourceDialog() {}

    virtual std::unique_ptr<TableSource> createdSource() const = 0;

protected slots:
    virtual void accept() = 0;
protected:
    TableSourceDialog(SourceNameValidator validator,
        QWidget *parent);
    TableSourceDialog(SourceNameValidator validator,
        TableSource *source, QWidget *parent);

    virtual bool checkNonEmptyInput();
    virtual bool checkData();

};

#endif // TABLESOURCEDIALOG_H
