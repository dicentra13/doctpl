#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>

#include "app_fixture.h"
#include "helpers.h"

#include <doctpl/template.h>
#include <doctpl/layout.h>
#include <doctpl/page.h>
#include <doctpl/text_field.h>
#include <doctpl/table_field.h>

#include <initializer_list>

using namespace doctpl;

void checkPages(const Layout* l, std::initializer_list<const Page*> pages)
{
    BOOST_REQUIRE(l->pagesCount() == pages.size());

    if (l->pagesCount() == 0) {
        return;
    }
    const Page* p = l->page(0);
    auto pi = pages.begin();
    BOOST_CHECK(*pi++ == p);
    BOOST_CHECK(p->x() == - p->width() / 2.0);
    BOOST_CHECK(p->y() == 0.0);
    for (size_t i = 1; i < l->pagesCount(); ++i, ++pi) {
        const Page* c = l->page(i);
        BOOST_CHECK(*pi == c);
        BOOST_CHECK(c->x() == - c->width() / 2.0);
        BOOST_CHECK(c->y() == p->y() + p->height() + l->pageSeparator());
        p = c;
    }

    for (size_t i = 0; i < l->pagesCount(); ++i) {
        BOOST_CHECK(l->pageIndex(l->page(i)) == i);
    }
}

BOOST_FIXTURE_TEST_SUITE(layout_tests, AppFixture)

BOOST_AUTO_TEST_CASE(test_insert)
{
    Template doc;
    Layout* l = doc.layout();
    BOOST_CHECK(l->pagesCount() == 0);

    BOOST_CHECK(l->pageSeparator() == 0.0);
    l->setPageSeparator(5.0);
    BOOST_CHECK(l->pageSeparator() == 5.0);

    const Page* p = l->append({200.0, 100.0});
    BOOST_CHECK(p == l->page(0));
    BOOST_CHECK(p->width() == 200.0 && p->height() == 100.0);
    BOOST_CHECK(p->dx() == 0.0 && p->dy() == 0.0);
    checkPages(l, {p});

    const Page* p2 = l->insert(0, {300.0, 200.0}, 2.0, 3.0);
    BOOST_CHECK(p2 == l->page(0));
    BOOST_CHECK(p2->width() == 300.0 && p2->height() == 200.0);
    BOOST_CHECK(p2->dx() == 2.0 && p2->dy() == 3.0);
    checkPages(l, {p2, p});

    const Page* p3 = l->insert(2, {400.0, 300.0});
    BOOST_CHECK(p3 == l->page(2));
    BOOST_CHECK(p3->dx() == 0.0 && p3->dy() == 0.0);
    checkPages(l, {p2, p, p3});

    const Page* p4 = l->append({400.0, 300.0}, 2.0, 3.0);
    BOOST_CHECK(p4 == l->page(3));
    BOOST_CHECK(p4->dx() == 2.0 && p4->dy() == 3.0);
    checkPages(l, {p2, p, p3, p4});

    const Page* p5 = l->insert(2, {400.0, 300.0});
    BOOST_CHECK(p5 == l->page(2));
    checkPages(l, {p2, p, p5, p3, p4});

    // index out of range
    BOOST_CHECK_THROW(l->insert(6, {400.0, 300.0}), Exception);
}

BOOST_AUTO_TEST_CASE(test_page_access)
{
    // page from other doc
    Template doc;
    Layout* l = doc.layout();
    Template doc2;
    const Page* op = doc2.layout()->append({200.0, 100.0});
    BOOST_CHECK(l->pageIndex(op) == l->end());

    // invalid index
    BOOST_CHECK_THROW(l->page(0), Exception);
    BOOST_CHECK_THROW(doc2.layout()->page(1), Exception);
}

BOOST_AUTO_TEST_CASE(test_page_move)
{
    Template doc;
    Layout* l = doc.layout();

    const Page* p1 = l->append({200.0, 100.0});
    checkPages(l, {p1});
    const Page* p2 = l->append({200.0, 100.0});
    checkPages(l, {p1, p2});
    const Page* p3 = l->append({200.0, 100.0});
    checkPages(l, {p1, p2, p3});
    const Page* p4 = l->append({200.0, 100.0});
    checkPages(l, {p1, p2, p3, p4});

    auto CHECK_MOVE = [l] (
        Layout::Index from,
        Layout::Index to,
        std::initializer_list<const Page*> pages)
    { l->move(from, to); checkPages(l, pages); };

    CHECK_MOVE(0, 1, {p2, p1, p3, p4});
    CHECK_MOVE(1, 0, {p1, p2, p3, p4});
    CHECK_MOVE(0, 2, {p2, p3, p1, p4});
    CHECK_MOVE(2, 0, {p1, p2, p3, p4});
    CHECK_MOVE(0, 3, {p2, p3, p4, p1});
    CHECK_MOVE(3, 0, {p1, p2, p3, p4});
    CHECK_MOVE(1, 2, {p1, p3, p2, p4});
    CHECK_MOVE(2, 1, {p1, p2, p3, p4});
    CHECK_MOVE(1, 3, {p1, p3, p4, p2});
    CHECK_MOVE(3, 1, {p1, p2, p3, p4});
    CHECK_MOVE(2, 3, {p1, p2, p4, p3});
    CHECK_MOVE(3, 2, {p1, p2, p3, p4});

    // no changes

    CHECK_MOVE(0, 0, {p1, p2, p3, p4});
    CHECK_MOVE(1, 1, {p1, p2, p3, p4});
    CHECK_MOVE(2, 2, {p1, p2, p3, p4});
    CHECK_MOVE(3, 3, {p1, p2, p3, p4});

    // invalid index
    BOOST_CHECK_THROW(l->move(0, 4), Exception);
    BOOST_CHECK_THROW(l->move(4, 0), Exception);
}

BOOST_AUTO_TEST_CASE(test_erase)
{
    Template doc;
    Layout* l = doc.layout();

    /*const Page* p1 =*/ l->append({200.0, 100.0});
    const Page* p2 = l->append({200.0, 100.0});
    const Page* p3 = l->append({200.0, 100.0});
    const Page* p4 = l->append({200.0, 100.0});

    // invalid index
    BOOST_CHECK_THROW(l->erase(4), Exception);

    l->erase(0);
    checkPages(l, {p2, p3, p4});
    l->erase(1);
    checkPages(l, {p2, p4});
    l->erase(1);
    checkPages(l, {p2});
    l->erase(0);
    checkPages(l, {});
}

const std::string FORMAT =
        "<formatting>"
         "<font style=\"Обычный\" family=\"Times New Roman\" size=\"11\"/>"
          "<alignment valign=\"bottom\" halign=\"center\"/>"
          "<margins left=\"2\" top=\"1\" bottom=\"1\" right=\"1\"/>"
        "</formatting>";

const QString NAME_1 = "Field 1";
const QString NAME_2 = "Field 2";

BOOST_AUTO_TEST_CASE(test_erase_fields)
{
    Template doc;
    Layout* l = doc.layout();

    Page* p = l->append({200.0, 100.0});
    TableField* f1 = new TableField(
        NAME_1, {10.0, 5.0}, {0.0, 0.0}, 5, 2, createFormatting(FORMAT), p);
    TextField* f2 = new TextField(
        NAME_2, {10.0, 5.0}, {0.0, 0.0}, createFormatting(FORMAT), p);
    auto fields = doc.fields();
    BOOST_CHECK(fields->as<TextField>()->find(NAME_2) == f2);
    BOOST_CHECK(fields->as<TableField>()->find(NAME_1) == f1);

    l->erase(0);
    BOOST_CHECK_THROW(fields->as<TextField>()->find(NAME_2), Exception);
    BOOST_CHECK_THROW(fields->as<TableField>()->find(NAME_1), Exception);
}

BOOST_AUTO_TEST_CASE(test_page_fields_manip)
{
    Template doc;
    Layout* l = doc.layout();

    Page* p = l->append({200.0, 100.0});
    TableField* f1 = new TableField(
        NAME_1, {10.0, 5.0}, {0.0, 0.0}, 5, 2, createFormatting(FORMAT), p);
    TextField* f2 = new TextField(
        NAME_2, {10.0, 5.0}, {0.0, 0.0}, createFormatting(FORMAT), p);
    auto fields = doc.fields();
    BOOST_CHECK(fields->as<TextField>()->find(NAME_2) == f2);
    BOOST_CHECK(fields->as<TableField>()->find(NAME_1) == f1);
    auto pf = p->fields();
    BOOST_CHECK(std::find(pf.begin(), pf.end(), f1) != pf.end());
    BOOST_CHECK(std::find(pf.begin(), pf.end(), f2) != pf.end());

    Page* p2 = l->append({200.0, 100.0});
    p2->addField(f1, {0.0, 0.0});
    p2->addField(f2, {0.0, 0.0});
    BOOST_CHECK(fields->as<TextField>()->find(NAME_2) == f2);
    BOOST_CHECK(fields->as<TableField>()->find(NAME_1) == f1);
    pf = p->fields();
    auto pf2 = p2->fields();
    BOOST_CHECK(std::find(pf.begin(), pf.end(), f1) == pf.end());
    BOOST_CHECK(std::find(pf.begin(), pf.end(), f2) == pf.end());
    BOOST_CHECK(std::find(pf2.begin(), pf2.end(), f1) != pf2.end());
    BOOST_CHECK(std::find(pf2.begin(), pf2.end(), f2) != pf2.end());

    p2->deleteField(f1);
    p2->deleteField(f2);
    pf2 = p2->fields();
    BOOST_CHECK(std::find(pf2.begin(), pf2.end(), f1) == pf2.end());
    BOOST_CHECK(std::find(pf2.begin(), pf2.end(), f2) == pf2.end());
    BOOST_CHECK_THROW(fields->as<TextField>()->find(NAME_2), Exception);
    BOOST_CHECK_THROW(fields->as<TableField>()->find(NAME_1), Exception);
}

BOOST_AUTO_TEST_CASE(test_page_resize)
{
    Template doc;
    Layout* l = doc.layout();

    Page* p1 = l->append({200.0, 100.0});
    Page* p2 = l->append({200.0, 100.0});
    Page* p3 = l->append({200.0, 100.0});
    Page* p4 = l->append({200.0, 100.0});

    p1->setHeight(200.0);
    BOOST_CHECK(p1->height() == 200.0);
    checkPages(l, {p1, p2, p3, p4});
    p2->setHeight(200.0);
    checkPages(l, {p1, p2, p3, p4});
    p3->setHeight(200.0);
    checkPages(l, {p1, p2, p3, p4});
    p4->setHeight(200.0);
    checkPages(l, {p1, p2, p3, p4});

    p1->setWidth(250.0);
    BOOST_CHECK(p1->width() == 250.0);
    checkPages(l, {p1, p2, p3, p4});

    p1->setDx(1.0);
    p1->setDy(2.0);
    BOOST_CHECK(p1->dx() == 1.0 && p1->dy() == 2.0);
}

BOOST_AUTO_TEST_CASE(test_invalid_page_dimensions)
{
    Template doc;
    Layout* l = doc.layout();

    Page* p = l->append({200.0, 100.0});

    BOOST_CHECK_THROW(p->setWidth(-1.0), Exception);
    BOOST_CHECK_THROW(p->setHeight(-1.0), Exception);

    BOOST_CHECK_THROW(l->append({-200.0, 100.0}), Exception);
    BOOST_CHECK_THROW(l->append({200.0, -100.0}), Exception);
    BOOST_CHECK_THROW(l->append({-200.0, 100.0}, 1.0, 1.0), Exception);
    BOOST_CHECK_THROW(l->append({200.0, -100.0}, 1.0, 1.0), Exception);
    BOOST_CHECK_THROW(l->insert(0, {-200.0, 100.0}), Exception);
    BOOST_CHECK_THROW(l->insert(0, {200.0, -100.0}), Exception);
    BOOST_CHECK_THROW(l->insert(0, {-200.0, 100.0}, 1.0, 1.0), Exception);
    BOOST_CHECK_THROW(l->insert(0, {200.0, -100.0}, 1.0, 1.0), Exception);
}

BOOST_AUTO_TEST_CASE(test_page_orientation)
{
    Template doc;
    Layout* l = doc.layout();

    Page* p = l->append({200.0, 100.0});
    BOOST_CHECK(p->orientation() == QPrinter::Orientation::Landscape);
    p->setOrientation(QPrinter::Orientation::Landscape);
    BOOST_CHECK(p->width() == 200.0 && p->height() == 100.0
        && p->orientation() == QPrinter::Orientation::Landscape);
    p->setOrientation(QPrinter::Orientation::Portrait);
    BOOST_CHECK(p->width() == 100.0 && p->height() == 200.0
        && p->orientation() == QPrinter::Orientation::Portrait);
}

BOOST_AUTO_TEST_SUITE_END()
