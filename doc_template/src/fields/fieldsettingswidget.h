#ifndef FIELDSETTINGSWIDGET_H
#define FIELDSETTINGSWIDGET_H

#include <QWidget>
#include <QString>

class CommonFieldSettingsWidget;

class FieldSettingsWidget : public QWidget {

    Q_OBJECT

public:
    virtual ~FieldSettingsWidget();
    QWidget *commonSettingsWidget();
    virtual void setEnabled(bool);
    void resetName();
    QString name() const;
    bool isNameClear() const;
    int pageNumber() const;
    void resetX();
    double x() const;
    bool isXClear() const;
    void resetY();
    double y() const;
    bool isYClear() const;
    void resetWidth();
    double width() const;
    bool isWidthClear() const;
    void resetHeight();
    double height() const;
    bool isHeightClear() const;
    bool setName(const QString &name);
    void setPages(size_t count);
    bool setPageNumber(int pageNumber);
    bool setX(double x);
    bool setY(double y);
    bool setWidth(double width);
    bool setHeight(double height);
signals:
    void nameChanged(QString);
    void pageChanged(size_t number);
    void xPosChanged(double x);
    void yPosChanged(double y);
    void widthChanged(double w);
    void heightChanged(double h);
protected:
    explicit FieldSettingsWidget(QWidget *parent);
protected slots:
    void nameChanged_(QString);
    void pageChanged_(int);
    void xPosChanged_(double);
    void yPosChanged_(double);
    void widthChanged_(double);
    void heightChanged_(double);
private:
    /**
      Non-copyable
      */
    FieldSettingsWidget(const FieldSettingsWidget &);
    FieldSettingsWidget &operator=(const FieldSettingsWidget &);

    CommonFieldSettingsWidget *widget_;
};

#endif // FIELDSETTINGSWIDGET_H
