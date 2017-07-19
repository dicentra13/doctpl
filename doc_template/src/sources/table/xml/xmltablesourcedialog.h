#ifndef XMLTABLESOURCEDIALOG_H
#define XMLTABLESOURCEDIALOG_H

#include "../tablesourcedialog.h"

class XMLTableSource;
class QListWidget;
class QListWidgetItem;
class QLineEdit;
class QPushButton;

class XMLTableSourceDialog : public TableSourceDialog {

    Q_OBJECT

public:
    XMLTableSourceDialog(SourceNameValidator validator,
        QWidget *parent);
    XMLTableSourceDialog(SourceNameValidator validator,
        XMLTableSource *source, QWidget *parent);
    virtual ~XMLTableSourceDialog() {}

    virtual std::unique_ptr<TableSource> createdSource() const;

protected slots:
    virtual void accept();
protected:
    virtual Source *source();
    virtual bool checkNonEmptyInput();
    virtual bool checkData();
private slots:
    void addQuery();
    void removeQueries();
    void editItem(QListWidgetItem*);
private:
    void createWidgets();

    XMLTableSource *edited_;

    QLineEdit *fileName_;
    QListWidget *queryWidget_;
    QPushButton *addQueryButton_, *removeQueryButton_;
};

#endif // XMLTABLESOURCEDIALOG_H
