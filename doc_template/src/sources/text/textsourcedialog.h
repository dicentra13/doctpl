#ifndef TEXTSOURCEDIALOG_H
#define TEXTSOURCEDIALOG_H

#include "../sourcedialog.h"

#include <memory>

class TextSource;
class QSpinBox;
class QWidget;

class TextSourceDialog : public SourceDialog {

    Q_OBJECT

public:
    virtual ~TextSourceDialog() {}

    virtual std::unique_ptr<TextSource> createdSource() const = 0;

protected slots:
    virtual void accept() = 0;
protected:
    TextSourceDialog(SourceNameValidator validator,
        QWidget *parent);
    TextSourceDialog(SourceNameValidator validator,
        TextSource *source, QWidget *parent);

    virtual bool checkNonEmptyInput();
    virtual bool checkData();

};

#endif // TEXTSOURCEDIALOG_H
