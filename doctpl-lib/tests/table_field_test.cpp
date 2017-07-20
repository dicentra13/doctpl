#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>

#include "app_fixture.h"
#include "helpers.h"

#include <doctpl/table_field.h>
#include <doctpl/page.h>
#include <doctpl/layout.h>
#include <doctpl/template.h>

#include <vector>
#include <sstream>

using namespace doctpl;

BOOST_FIXTURE_TEST_SUITE(table_field_tests, AppFixture)

const std::string FORMAT_1 =
        "<formatting>"
         "<font style=\"Обычный\" family=\"Times New Roman\" size=\"11\"/>"
          "<alignment valign=\"bottom\" halign=\"center\"/>"
          "<margins left=\"2\" top=\"1\" bottom=\"1\" right=\"1\"/>"
        "</formatting>";

const QString NAME_1 = "Table 1";
const QString NAME_2 = "Table 2";

typedef std::vector<double> Sizes;

void checkField(const TableField* f, const Sizes& w, const Sizes& h)
{
    BOOST_REQUIRE(f->rowsCount() == h.size());
    BOOST_CHECK(f->rowY(0) == 0.0);
    double th = 0.0;
    for (size_t i = 0; i < f->rowsCount(); ++i) {
        BOOST_CHECK(f->rowHeight(i) == h[i]);
        BOOST_CHECK(i == 0 || f->rowY(i) == f->rowY(i - 1) + f->rowHeight(i - 1));
        th += f->rowHeight(i);
    }
    BOOST_CHECK(f->height() == th);

    BOOST_REQUIRE(f->columnsCount() == w.size());
    BOOST_CHECK(f->columnX(0) == 0.0);
    double tw = 0.0;
    for (size_t j = 0; j < f->columnsCount(); ++j) {
        BOOST_CHECK(f->columnWidth(j) == w[j]);
        BOOST_CHECK(j == 0 || f->columnX(j) == f->columnX(j - 1) + f->columnWidth(j - 1));
        tw += f->columnWidth(j);
    }
    BOOST_CHECK(f->width() == tw);
}


BOOST_AUTO_TEST_CASE(test_create_and_delete)
{
    Template doc;
    Layout* l = doc.layout();
    Page* p = l->append({{100.0, 100.0}, defaultStyle()});
    TableField* f = new TableField(
        NAME_1, {10.0, 5.0}, {0.0, 0.0}, 5, 2, createFormatting(FORMAT_1), defaultStyle(), p);
    BOOST_CHECK(f->page() == p);
    BOOST_CHECK(f->fieldType() == "table");
    BOOST_CHECK(f->name() == NAME_1);
    checkField(f, Sizes(2, 5.0), Sizes(5, 1.0));

    const auto& df = doc.fields()->as<TableField>();
    BOOST_CHECK(df->contains(f->id()));
    BOOST_CHECK(df->find(NAME_1) == f);
    BOOST_CHECK(df->find(f->id()) == f);

    const auto& pf = p->fields();
    BOOST_CHECK(pf.size() == 1 && pf.front() == f);

    auto id = f->id();

    p->deleteField(f);

    BOOST_CHECK(!df->contains(id));
    BOOST_CHECK_THROW(df->find(NAME_1), Exception);
    BOOST_CHECK_THROW(df->find(id), Exception);
}

BOOST_AUTO_TEST_CASE(test_create_and_delete_2)
{
    Template doc;
    Layout* l = doc.layout();
    Page* p = l->append({{100.0, 100.0}, defaultStyle()});

    Sizes w = {5.0, 6.0};
    Sizes h = {1.0, 2.0, 3.0};
    std::vector<FormattingSettingsPtr> fs(2, createFormatting(FORMAT_1));

    TableField* f = new TableField(NAME_2, w, h, {0.0, 0.0}, fs, defaultStyle(), p);

    BOOST_CHECK(f->page() == p);
    BOOST_CHECK(f->fieldType() == "table");
    BOOST_CHECK(f->name() == NAME_2);
    checkField(f, w, h);

    const auto& df = doc.fields()->as<TableField>();
    BOOST_CHECK(df->contains(f->id()));
    BOOST_CHECK(df->find(NAME_2) == f);
    BOOST_CHECK(df->find(f->id()) == f);

    const auto& pf = p->fields();
    BOOST_CHECK(pf.size() == 1 && pf.front() == f);

    auto id = f->id();

    p->deleteField(f);

    BOOST_CHECK(!df->contains(id));
    BOOST_CHECK_THROW(df->find(NAME_2), Exception);
    BOOST_CHECK_THROW(df->find(id), Exception);
}

// TODO
BOOST_AUTO_TEST_CASE(test_create_DOCTPL_ERRORs)
{
    // widths or heights mismatch
    // nullptrs in page, formatting
}

const Sizes W = {3.0, 3.0, 3.0};
const Sizes H = {1.0, 1.0, 1.0, 1.0};

std::unique_ptr<Template> createTestDoc()
{
    std::unique_ptr<Template> doc(new Template);
    Layout* l = doc->layout();
    Page* p = l->append({{100.0, 100.0}, defaultStyle()});
    new TableField(
        NAME_1,
        W, H,
        {0.0, 0.0},
        std::vector<FormattingSettingsPtr>(W.size(), createFormatting(FORMAT_1)),
        defaultStyle(),
        p);
    return doc;
}

BOOST_AUTO_TEST_CASE(test_resize)
{
    auto doc = createTestDoc();
    TableField* f = doc->fields()->as<TableField>()->find(NAME_1);

    // negative width
    BOOST_CHECK_THROW(f->setWidth(-1.0), Exception);
    checkField(f, W, H);
    // negative last column width
    BOOST_CHECK_THROW(f->setWidth(5.0), Exception);
    checkField(f, W, H);

    f->setWidth(6.0);
    Sizes wn = W; wn[2] = 0.0;
    checkField(f, wn, H);
    f->setWidth(9.0);
    checkField(f, W, H);

    // negative height
    BOOST_CHECK_THROW(f->setHeight(-1.0), Exception);
    checkField(f, W, H);
    // negative last row height
    BOOST_CHECK_THROW(f->setHeight(2.0), Exception);
    checkField(f, W, H);

    f->setHeight(3.0);
    Sizes hn = H; hn[3] = 0.0;
    checkField(f, W, hn);

    f->setHeight(4.0);
    checkField(f, W, H);
}

BOOST_AUTO_TEST_CASE(test_row_resize)
{
    auto doc = createTestDoc();
    TableField* f = doc->fields()->as<TableField>()->find(NAME_1);

    Sizes hn = H;

    auto CHECK_ROW_RESIZE = [f, &hn] (size_t i, double size)
    {
        f->resizeRow(i, size);
        hn[i] = size;
        checkField(f, W, hn);
    };

    CHECK_ROW_RESIZE(0, 2.0);
    CHECK_ROW_RESIZE(1, 0.0);
    CHECK_ROW_RESIZE(2, 1.0);
    CHECK_ROW_RESIZE(3, 0.5);

    // DOCTPL_ERRORs

    // invalid index
    BOOST_CHECK_THROW(f->resizeRow(4, 1.0), Exception);
    checkField(f, W, hn);
    // size negative
    BOOST_CHECK_THROW(f->resizeRow(0, -1.0), Exception);
    checkField(f, W, hn);
}

BOOST_AUTO_TEST_CASE(test_row_add)
{
    auto doc = createTestDoc();
    TableField* f = doc->fields()->as<TableField>()->find(NAME_1);

    Sizes hn = H;

    auto CHECK_ROW_ADD = [f, &hn] (size_t i, double size)
    {
        f->addRow(i, size);
        hn.insert(hn.begin() + i, size);
        checkField(f, W, hn);
    };

    f->setText(0, 0, "test");

    CHECK_ROW_ADD(0, 3.0);
    BOOST_CHECK(f->text(1, 0) == "test");

    CHECK_ROW_ADD(5, 4.0);
    BOOST_CHECK(f->text(1, 0) == "test");

    // DOCTPL_ERRORs

    // invalid index
    BOOST_CHECK_THROW(f->addRow(7, 1.0), Exception);
    checkField(f, W, hn);
    // size negative
    BOOST_CHECK_THROW(f->addRow(0, -1.0), Exception);
    checkField(f, W, hn);
}

BOOST_AUTO_TEST_CASE(test_row_delete)
{
    auto doc = createTestDoc();
    TableField* f = doc->fields()->as<TableField>()->find(NAME_1);

    Sizes hn = H;

    auto CHECK_ROW_DELETE = [f, &hn] (size_t i)
    {
        f->deleteRow(i);
        hn.erase(hn.begin() + i);
        checkField(f, W, hn);
    };

    f->setText(2, 0, "test");

    CHECK_ROW_DELETE(0);
    BOOST_CHECK(f->text(1, 0) == "test");

    CHECK_ROW_DELETE(2);
    BOOST_CHECK(f->text(1, 0) == "test");

    CHECK_ROW_DELETE(0);
    BOOST_CHECK(f->text(0, 0) == "test");

    // DOCTPL_ERRORs

    // invalid index
    BOOST_CHECK_THROW(f->deleteRow(2), Exception);
    checkField(f, W, hn);
    // last row
    BOOST_CHECK_THROW(f->deleteRow(0), Exception);
    checkField(f, W, hn);
}

BOOST_AUTO_TEST_CASE(test_column_resize)
{
    auto doc = createTestDoc();
    TableField* f = doc->fields()->as<TableField>()->find(NAME_1);

    Sizes wn = W;

    auto CHECK_COLUMN_RESIZE = [f, &wn] (size_t i, double size)
    {
        f->resizeColumn(i, size);
        wn[i] = size;
        checkField(f, wn, H);
    };

    CHECK_COLUMN_RESIZE(0, 2.0);
    CHECK_COLUMN_RESIZE(2, 4.0);

    // DOCTPL_ERRORs

    // invalid index
    BOOST_CHECK_THROW(f->resizeColumn(3, 1.0), Exception);
    checkField(f, wn, H);
    // size negative
    BOOST_CHECK_THROW(f->resizeColumn(0, -1.0), Exception);
    checkField(f, wn, H);
}

BOOST_AUTO_TEST_CASE(test_column_add)
{
    auto doc = createTestDoc();
    TableField* f = doc->fields()->as<TableField>()->find(NAME_1);

    Sizes wn = W;

    auto CHECK_COLUMN_ADD = [f, &wn] (size_t i, double size)
    {
        f->addColumn(i, size, createFormatting(FORMAT_1));
        wn.insert(wn.begin() + i, size);
        checkField(f, wn, H);
    };

    f->setText(0, 0, "test");

    CHECK_COLUMN_ADD(0, 3.0);
    BOOST_CHECK(f->text(0, 1) == "test");
    CHECK_COLUMN_ADD(4, 2.0);
    BOOST_CHECK(f->text(0, 1) == "test");

    // DOCTPL_ERRORs

    // invalid index
    BOOST_CHECK_THROW(f->addColumn(6, 1.0, createFormatting(FORMAT_1)), Exception);
    checkField(f, wn, H);
    // size negative
    BOOST_CHECK_THROW(f->addColumn(0, -1.0, createFormatting(FORMAT_1)), Exception);
    checkField(f, wn, H);
    // nullptr formatting
    BOOST_CHECK_THROW(f->addColumn(0, 1.0, FormattingSettingsPtr()), Exception);
    checkField(f, wn, H);
}

BOOST_AUTO_TEST_CASE(test_column_delete)
{
    auto doc = createTestDoc();
    TableField* f = doc->fields()->as<TableField>()->find(NAME_1);

    Sizes wn = W;

    auto CHECK_COLUMN_DELETE = [f, &wn] (size_t i)
    {
        f->deleteColumn(i);
        wn.erase(wn.begin() + i);
        checkField(f, wn, H);
    };

    f->setText(0, 1, "test");

    CHECK_COLUMN_DELETE(0);
    BOOST_CHECK(f->text(0, 0) == "test");
    CHECK_COLUMN_DELETE(1);
    BOOST_CHECK(f->text(0, 0) == "test");

    // DOCTPL_ERRORs

    // invalid index
    BOOST_CHECK_THROW(f->deleteColumn(1), Exception);
    // last column
    BOOST_CHECK_THROW(f->deleteColumn(0), Exception);
}

BOOST_AUTO_TEST_CASE(test_access_out_of_range)
{
    auto doc = createTestDoc();
    TableField* f = doc->fields()->as<TableField>()->find(NAME_1);

    BOOST_CHECK_THROW(f->columnWidth(4), Exception);
    BOOST_CHECK_THROW(f->columnFormatting(4), Exception);
    BOOST_CHECK_THROW(f->setColumnFormatting(4, createFormatting(FORMAT_1)), Exception);
    BOOST_CHECK_THROW(f->setText(0, 4, ""), Exception);
    BOOST_CHECK_THROW(f->text(0, 4), Exception);

    BOOST_CHECK_THROW(f->rowHeight(5), Exception);
    BOOST_CHECK_THROW(f->setText(5, 0, ""), Exception);
    BOOST_CHECK_THROW(f->text(5, 0), Exception);
}

BOOST_AUTO_TEST_CASE(test_text)
{
    auto doc = createTestDoc();
    TableField* f = doc->fields()->as<TableField>()->find(NAME_1);

    for (size_t i = 0; i < f->rowsCount(); ++i) {
        for (size_t j = 0; j < f->columnsCount(); ++j) {
            std::ostringstream os;
            os << i << ":" << j;
            QString text(os.str().c_str());
            f->setText(i, j, text);
            BOOST_CHECK(f->text(i, j) == text);
        }
    }
    f->clear();
    for (size_t i = 0; i < f->rowsCount(); ++i) {
        for (size_t j = 0; j < f->columnsCount(); ++j) {
            BOOST_CHECK(f->text(i, j).isEmpty());
        }
    }
}

BOOST_AUTO_TEST_CASE(test_formatting_settings)
{
    // TODO
}

BOOST_AUTO_TEST_SUITE_END()
