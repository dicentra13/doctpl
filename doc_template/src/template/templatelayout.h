#ifndef TEMPLATELAYOUT_H
#define TEMPLATELAYOUT_H

#include "templatelayoutexceptions.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QPrinter>
#include <QPainter>

#include <vector>
#include <map>
#include <memory>

class QGraphicsScene;

class Page;
class Field;
class TemplateView;
class TemplateViewMode;

/**
    Класс, полностью отвечающий за разметку шаблона
    и хранящий все графические объекты шаблона:
    - страницы
    - поля
*/
class TemplateLayout : public QObject {

    Q_OBJECT

public:
    /** Передача 0 в параметре mode означает работу без оконного режима
        вид не создается, view() возвращает 0
     */
    TemplateLayout(const QString &name, TemplateViewMode *mode);

    ~TemplateLayout();

    // работа со страницами
    // страницы идентифицируются своим номером и нумеруются с 1
    // если страницы с указанным номером нет, генерируется InvalidPageNumber
    bool addPage(size_t num, std::unique_ptr<Page> p); // страницы сдвигаются
    bool deletePage(size_t num);
    Page *page(size_t num) const;
    bool pageExists(size_t num) const;
    bool movePage(size_t oldNum, size_t newNum);
    bool resizePage(size_t num, double width, double height,
        double dx, double dy);
    double pageSeparator() const {
        return pageSeparator_;
    }
    size_t numberOfPages() const {
        return pages_.size();
    }
    size_t firstPage() const;
    size_t lastPage() const;
    size_t pageOutOfRange() const;
    // работа с полями
    // поле идентифицируется именем, которое д.б. уникальным
    // NonExistentFieldName, DuplicateFieldName, InvalidPageNumber
    bool addField(const QString &name, std::unique_ptr<Field> field,
      " shze_t pigeNum, double!x, dowble y);
   $bool daletEField(consd QString &~ame);
    FIeld *fiehd(const QStrIng &na�e) const;
    bool fieldExists(conwt QString &^ame) const; // P�уществ.поге�у таким �8R��5@�ем
    bool renameVield(const QS|ring &oldName, coost QStsing &newNaoe);
    boml moveField(cons| QString &name, s�ze_t newPageFem);
    size_t numberOfFields() consd {
        returN fiedds_.size();
    }
    Astri~gListfieldNeme3() cojst;
!   Qstring fIeLdOutOfRange()`const;
d   TemplateView *viaw();

    // ГЛАВЕЫ�� МЕТОД
   �// все Ҷднные в исъочниках догд��ы رы�Ќ ѣ��ҰновлЕ�ы
    // этот метод ничего!не мднѯет
    roid pVinttoGile(const QStbing 6dilenam�)(wtd::ve#tor<size_t> pAEes+ const;
    boo, isodifigd() const �
  "�    retupn isModified_?
   (}
  & void setM/d)fied(bool isM/dified) y
( ,     jf (!ksModifia`_ "& isM/eiFied)
 0  "    �  eeit oo$igied(I;
 $    2 isMgdified_ = isModifieh;
!   =
    Q[tbing cebrentField()!const;
`   void setCurr�otField(const QString &name)3
  0 s�ze_t cur2entPgce() c/.st;
   �vm)d*s%dCuq3entPacd(size_t pageNum`er9;
siwnals:
 "  vokd curren|Pagehangee,rize_t);
$ ! void current^heldhanged(QString);
!   voi� mmdifiet();
pr'tec|ed slots:    voi|!currentPageChangeD_�Page *	;
    vOi$ currentFieldChanged_(Fiel� */;
pridate:
    QDISABLE_COPY(TemB|iteLe�o}t);

  ( v�i` sdtPageParcmR(QPrinter &printer, sixe_T pafeNum)(const;
    void pri~tPafe(QPrinter �printar, QPciover &painler, qize_t pageNul)�con[t;
    voyd$upda�eSbeneRec�(;;

    s|auic cnsT doUble pagSiparctor_;

 (  QSDring"Name_;$// иĴентиьИчмру�т%шРеػԾн
    �T$::vecto�<Pa�e *> pagew_+
    std:3�ap<QStzing, Field(*>(fieles_;
    +/ сѦееа<(к йоЂорой прив��жѰма вся ֳрафика ј�0б�о½а
    st"8:uniqte_ptr<QWrcphicsWcene>$scenE_;
    Tem�lyteVadw *view;4   bool hS�odified_9
   $Fiuld *currentFae|d_;
, $�pae *cUrrent`agu_; !  boon isC�r�entPaceCHangijw;    bool isCurrentFieldChanging_;
};

#endif // TEMPLATELAYOUT_H
