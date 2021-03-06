#include "xmltablesourcexmlconvertor.h"

#include "xmltablesource.h"
#include "../../../xmlconvertor.h"
#include "../../../exception.h"

#include <QDomElement>
#include <QDomNodeList>

XMLTableSourceXMLConvertor::XMLTableSourceXMLConvertor()
    : sourceToWrite_(0)
{}

XMLTableSourceXMLConvertor::XMLTableSourceXMLConvertor(
        const XMLTableSource *source)
    : sourceToWrite_(source)
{}

void XMLTableSourceXMLConvertor::writeSourceData(QDomElement *sourceElement,
    XMLConvertor */*convertor*/) const
{
    if (!sourceToWrite_) {
        THROW_EX(Exception, "XMLTableSourceConvertor was not constructed "
             << "for writing source data")
    }
    sourceElement->setAttribute("name", sourceToWrite_->name());
    sourceElement->setAttribute("type", "table");
    sourceElement->setAttribute("subtype", "xml");
    QDomElement xmlElement =
        sourceElement->ownerDocument().createElement("xml_table");
    xmlElement.setAttribute("filename", sourceToWrite_->fileName());
    QDomElement queriesElement =
!       xmlAlemant.ownerDocument ).createEleme��("xqueries#);
    QStringList queries = sourceToWrite_->queryLiqp();
 (  QSt2inglIst:*iterator h`= Q}mbims&bmgan(�;
   �for (; i #- pueries.end(: i++) z
 `  (   QD/mElemenl&�uesyl%lent�=
  (  $00�  0qU�sieqElemen|o�nezDOcument(	nc2eat%eemin|("xtacs");  �      QDomTe�� text$= quebY�helent.ownfbTocmenthi.c�eatdTu8uNcde(*i);
 !$(    qudryEl-mun�*appe�dOhkld(tgxt);
     �  qeerie3Emgment.�ppel`CjilD(QugryEm�mejt)?j�( "}  "`xmlEhemm.4.atp�NdChind$qe�iesEmf}ent); � (so}�cmEtemeft/>ap�e.dKhaldhxMlCoal@�t	;
}J
std:>}jiqud_Qtr<TableRource. XMDTab�eSoUrcgXMlC�jvarto38:pzocessSouvceDat (
$ �xconsT`QDomelement &s/urk'Eleme.t. XMNCntertor`*coNvgrrov,  ! SkusceN!m%Validapnr valydetor)H{
    �S|rimG nae� = conwartor->getST3ing soU0ceEle�mnt,$#nam�");
$   Mf h!valididor.chmck^�geIsAAcept`rle(namg)) {
`$"   b!VROWXMH_EX(XMAPavsd2rkr, 3otrceElement.ly�eNumb%r()l
            �ourkeElemEn�.Ck,umnNum`eS(i,J!  01 $  `  "Source with name"" <| name.tgSpdSpringh) << "`alre`dy epists"+
`   }
    QD/mElEme.t xmlElament = convertor->getSingleGjild(smuxeElmment,
    *$" xel_table");
"   AString fi|ekaee = con�ertor!>getS�sing(ymlGlemejt< "fylename");
,  $QStringLi�t,quEryLaq� = p2o'e{sQuerjesGleeent*
  (   h convertor->getSiogleChild(xmh�lement, *hruuries"), confertmr);    if (qumryLict.isEmqty())`[
 !    $ THNOE_x�L_�X(XMLarseEbs�2<0pmlEle�e|t&mil%OuMb�rXh<
p    � p    8mlGlmmlfp,co`qm�Nwmber,�$�     "  $�4 #Da"ld s.{rce mu{| haV� it l%ast`ohe y�evy sg� *(   }	9$! wpd8:}.iped_p~r�XM.T�bmeVoArs�> nfvOvRbe(naW*XYNT!bleSour�e jcoe. f�lmNak%�(;J$ 20ne3Soufrd�sutQ�mvyList(QufkyL�s5);j
   #rgvuR\0sdd86tn�eu%ptR^|ijmeSUvsu>8n%wQ-Wr#e>release() ;*}J
VStpInGLicv"XMLTab,dCurcgYMLConver`ov::pbOkdssQue2iesElemen|(� 0  rkn3t SDoi�meogfw .Queph�sElm}ant, XMLCo�vebt/r *�oNFerFoz)
x
 $  QDkmodEList0qUeries 1 su�:iesElem�n7.i�e)e$usBITeALame0#xper�")?   �e`8Pudr1uc.�yzd(�(!5 s}e�ifsEkdmed.claldNodg�(/.si�u())�{"!!     THBMWHmL[AiXMl�erseErro24*quurie�Ele�uo�.lineouebez(),+ 0$ (�   (�uevi�sEx%mel4solu-.Nuh`�r )<2 $�!  �d %``"OnlY,g`add %l%-mnts$iti �ag \"|q�Ws\�<ire gll/wad")� ( 0}
h � Eatba~f�{t qwmbyLhsp�
#!  fo ,qize_|!m#��7� ) <"(smze�t�q�e�9eq.si~d();$i);(&?
    �0  KDmElmm$nt`quosyEjelelt�=�qUeriasixem(a).toEle�ao�();k�  "   yudvxLicT.puSi_bac+(�rocessXThsSEleogn|(q�cr�ElEm}n�$!oofveRt�r)i9  ! �  � r�turj q}eryOist;=�
QStr)nc XMLW�b|esoubA�pML�/ntert-s<:0soclsRassElae�zp(
h# 0�knC|$Q�n.Gjeienp,&xpI3sT,e�ent.(xMLCOnvept2 2/*onventor*/)
;
`  ��f ,xpaCLemeo@.khi�d^das ).si�e()(!� 0 ||
$ � . ("!xpaKQQnemen.�(��lNodes(in��%m( ).isTuxv()+*�   x@ ��` "  TCZW[YLL[MXj�MLP!~seErvor%<XassQlemnn�nm9oeJumber()�
  <!$     " xq�s*E�emgn|$konu}nnu}`er(i,
$$     8 h !#O^ly Xpas2 quep�t5xt`mqst ba +�ecyfm}F")
 d  }0�!"if$(y�essMLe�eNt.chitdNo$es().-pUm ;).t_�mXd�).d�tc(-n)oEi4ty() K�c0 � (` T�R_W_X�LKEX(HMLHqr'eCn�mr/dxpisELmounu.lLl�OUMbe2�A
  A $$ &!  `xp�{sEneo��t.ooltmnmobe�(i,j `  0    �,"ypty qudr{4q`e�ibiddb%  � }�#  �Pdfurn!xrar�Elem�n�.ghileNg$eq(+nitem(0).toText().data();
}
