#ifndef PAGESETTINGSDIALOG_H
#define PAGESETTINGSDIALOG_H

#include <QDialog>

//#include <memory>

class QLineEdit;
class QComboBox;
class QDialogButtonBox;
//class Page;

class PageDialog : public QDialog {

    Q_OBJECT

public:

    enum Mode {Create, Edit};

    PageDialog(QWidget *parent, Mode mode = Create);
    virtual ~PageDialog() {};
    void setPages(size_t count);
    size_t pageNum() const;
    void setPageNum(size_t num);
    double width() const;
    void setWidth(double width);
    double height() const;
    void setHeight(double height);
    double dx() const;
    void setDx(double dx);
    double dy() const;
    void setDy(double dy);
    //void loadDataFromPage(Page *p);
    //std::unique_ptr<Page> createPage();
protected slots:
    virtual void accept();
private:
    void generateLayout();
    QComboBox *pageNumber_;
    QLineEdit *width_;
    QLineEdit *height_;
    QLineEdit *dx_;
    QLineEdit *dy_;
    QDialogButtonBox *buttons_;
};

#endif // PAGESETTINGSDIALOG_H
