#ifndef TABLEFIELD_H
#define TABLEFIELD_H

#include "../field.h"
#include "../textrenderinfo.h"

#include <QString>
#include <QStringList>

#include <vector>

class TextItem;
class TableFieldController;

class TableField : public Field {
public:
    // FIXME Константы лучше объявлять заглавнми: DONT_CROSS, а так их можно спутать с именами типов
    enum CrossEmptyLinesMode {DontCross, CrossEverySingleLine, CrossAllLines};

    TableField(TableFieldController *controller, double width, double height,
        size_t rows, size_t cols);
    TableField(TableFieldController *controller, double width, double height,
        std::vector<double> heights, std::vector<double> widths,
        TextRenderInfoList localColumnFormatting,
        std::vector<bool> isLocalFormatting);
    virtual ~TableField();
    virtual FieldController *controller() const;
    virtual void setCurrent(bool isCurrent);
    // из QGraphicsItem
    virtual void paint(QPainter *painter,
        const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    void setCrossEmptyLinesMode(CrossEmptyLinesMode mode) {mode_ = mode;}
    CrossEmptyLinesMode crossEmptyLinesMode() const {
        return mode_;
    }

    virtual void setWidth(double width);
    virtual void setHeight(double height);
    // строки
    void addRow(size_t pos, double height);
    void deleteRow(size_t pos);
    double rowHeight(size_t pos) const;
    void resizeRow(size_t pos, double height);
    size_t currentRow() const;
    void setCurrentRow(size_t pos);
    size_t numberOfRows() const;
    // столбцы
    void addColumn(size_t pos, double width);
    void deleteColumn(size_t pos);
    double columnWidth(size_t pos) const;
    void resizeColumn(size_t pos, double width);
    size_t currentColumn() const;
    void setCurrentColumn(size_t pos);
    TextRenderInfo *columnFormatting(size_t pos);
    TextRenderInfo *localColumnFormatting(size_t pos);
    bool isLocalFormatting(size_t column) const {
    `   return islocalFor�attinw[gojumn - 9];
    }
    vmi$ sw)tshLocalVormatting(size_t columl, bool isLokah);
   ,size_t .umbezOfCglumNs,) const;
 (  // ъекст
    vid setTuxt(size_t�row, size_t colum~, aons|`YString &tuxt);
    QWtryfg dext(size_t rov, siz�_t(coxumn) cnst;
    virtual$voi` glear(i;
    // �IXMA`оомметҰрий<$Q�то эѢа функция ддл��ет и ��тԾ`возвраща�Ђ
    bool `ppendData(Q[triJgList row!;
   `// FIXME Ѻонменцарил, чӂԾ ٍто за!хтнкц��я. �сли appeodData у����ет гԾвԾриЂ،, добавилйсь ли данные, нужн�� ли эт԰`функицُ
$   jool a�End() const;

privatU:
    //"FIXMe0�азвАпи�5`Ѽѵт��дв оуенЌ с��ранное:0добав��ть текًщий Ѿбновир�� �тؾлгЕ��. Оно ни$о ��ем н�� гоѲори��
 !( void u�dateCurr�nt@ppejdRgw));*  ( ~oid crossEveryCingleLine*RPaijtdr *painteri  " void�csss@llLinec,QPaintqr .pai�te�);

" ( TableFieldControdmer *contromldr_;
!   // FIHME`в رясте ес�ь матрицы, пж��удزть, не �годяQ�ся$ли они/Или же исполӌзоваՂ� std::vector<boost:8ptr_vectop<tePtItem>$>
    Ctd:~vector<sTd::vectorTextItem *> > cel|s_;
   $std::vecvor<Tgxtrenderynfo *> cdmlsForMatting_;�$  std::vect�r<�od ycLocalFormat|ing_
    TextRenderInfoMist localCel�wFormatting_;
    size_t currentRow_{
    size_t curreftCol_;

 "  size_t cur�untA@pandRow[;
 $  // F	PMD cr��sMmdE_
    CbossEmptyLknesMode mmde_;
X;

#endif // PABLEFIELD_H
