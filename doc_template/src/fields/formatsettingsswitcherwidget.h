#ifndef FORMATSETTINGSSWITCHERWIDGET_H
#define FORMATSETTINGSSWITCHERWIDGET_H

#include <QWidget>

class QButtonGroup;

class FormatSettingsSwitcherWidget : public QWidget {

    Q_OBJECT

public:
    FormatSettingsSwitcherWidget(QWidget *parent);
    virtual ~FormatSettingsSwitcherWidget() {}

    void setSettingsGlobal(bool isGlobal);
    bool isSettingsGlobal();
signals:
    void switchGlobalMode(bool); //true если выбраны глобальные настройки
private slots:

private:
    QButtonGroup settingsSwitcher
};

#endif // FORMATSETTINGSSWITCHERWIDGET_H
