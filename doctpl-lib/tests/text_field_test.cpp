#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>

#include "app_fixture.h"
#include "helpers.h"

#include <doctpl/text_field.h>
#include <doctpl/page.h>
#include <doctpl/layout.h>
#include <doctpl/template.h>

using namespace doctpl;

BOOST_FIXTURE_TEST_SUITE(text_field_tests, AppFixture)

const std::string FORMAT_1 =
        "<formatting>"
         "<font style=\"Обычный\" family=\"Times New Roman\" size=\"11\"/>"
          "<alignment valign=\"bottom\" halign=\"center\"/>"
          "<margins left=\"2\" top=\"1\" bottom=\"1\" right=\"1\"/>"
        "</formatting>";


BOOST_AUTO_TEST_CASE(test_create_and_delete)
{
    Template doc;
    Layout* l = doc.layout();
    Page* p = l->append({{100.0, 100.0}, defaultStyle()});
    TextField* f = new TextField(
        "Field1", {10.0, 5.0}, {0.0, 0.0}, createFormatting(FORMAT_1), defaultStyle(), p);
    BOOST_CHECK(f->page() == p);
    BOOST_CHECK(f->fieldType() == "text");
    BOOST_CHECK(f->name() == "Field1");
    BOOST_CHECK(f->width() == 10.0 && f->height() == 5.0);

    f->setText("text1");
    BOOST_CHECK(f->text() == "text1");
    f->clear();
    BOOST_CHECK(f->text().isEmpty());

    const auto& df = doc.fields()->as<TextField>();
    BOOST_CHECK(df->contains(f->id()));
    BOOST_CHECK(df->find("Field1") == f);
    BOOST_CHECK(df->find(f->id()) == f);

    const auto& pf = p->fields();
    BOOST_CHECK(pf.size() == 1 && pf.front() == f);

    auto id = f->id();

    p->deleteField(f);

    BOOST_CHECK(!df->contains(id));
    BOOST_CHECK_THROW(df->find("Field1"), Exception);
    BOOST_CHECK_THROW(df->find(id), Exception);
}

BOOST_AUTO_TEST_SUITE_END()
