#ifndef XMLTEXTSOURCEDIALOG_H
#define XMLTEXTSOURCEDIALOG_H

#include "../textsourcedialog.h"

class XMLTextSource;
class QLineEdit;

class XMLTextSourceDialog : public TextSourceDialog {

    Q_OBJECT

public:
    XMLTextSourceDialog(SourceNameValidator validator,
        QWidget *parent);
    XMLTextSourceDialog(SourceNameValidator validator,
        XMLTextSource *source, QWidget *parent);
    virtual ~XMLTextSourceDialog() {}

    virtual std::unique_ptr<TextSource> createdSource() const;

protected slots:
    virtual void accept();
protected:
    virtual Source *source();
    virtual bool checkNonEmptyInput();
    virtual bool checkData();
private:
    void createWidgets();

    XMLTextSource *edited_;

    QLineEdit *fileName_;
    QLineEdit *query_;
};

#endif // XMLTEXTSOURCEDIALOG_H
