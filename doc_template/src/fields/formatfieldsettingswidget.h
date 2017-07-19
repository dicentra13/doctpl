#ifndef FORMATFIELDSETTINGSWIDGET_H
#define FORMATFIELDSETTINGSWIDGET_H

#include <QWidget>

class QFontComboBox;
class QComboBox;
class QDoubleSpinBox;
class QButtonGroup;
class QVBoxLayout;
class TextRenderInfo;

class FormatFieldSettingsWidget : public QWidget {

    Q_OBJECT

public:
    explicit FormatFieldSettingsWidget(QWidget *parent = 0,
        TextRenderInfo *data = 0);

    // передача 0 делает все поля неактивными
    void setData(TextRenderInfo *data = 0);
    void setEnabled(bool isEditable);
public slots:
    void fontFamilyChanged(const QFont &);
    void fontStyleChanged(const QString &);
    void fontSizeChanged(const QString &);
    void alignChanged(int id);
    void identChanged(double m);
private:
    TextRenderInfo *data_;
    QFontComboBox *fontFamilies_;
    QComboBox *fontStyles_;
    QComboBox *fontSizes_;
    QButtonGroup *halign_;
    QButtonGroup *valign_;
    QDoubleSpinBox *leftMargin_, *rightMargin_, *bottomMargin_, *topMargin_;
    QVBoxLayout *mainLayout_;

    void initFontBlock();
    void initAlignBlock();
    void initIdentBlock();
    void setAlignButtonsEnabled(bool isEnabled);
    void setAlignButtonsValues(int flags);

    bool isSignalsBlocked_;
};

#endif // FORMATFIELDSETTINGSWIDGET_H
