#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_NO_MAIN

#include <boost/test/unit_test.hpp>

#include "app_fixture.h"

#include <doctpl/serialize.h>

using namespace doctpl;

BOOST_FIXTURE_TEST_SUITE(serialize_tests, AppFixture)

BOOST_AUTO_TEST_CASE(test_read)
{
    std::unique_ptr<Template> doc = xml::read("../../../doctpl-lib/tests/data/11kl_2016.xml");
    TextField* f = doc->fields()->as<TextField>()->find(QString::fromUtf8("Фамилия_1"));
    BOOST_CHECK(f->x() == 120.0 && f->y() == 23.0);
    doc->print("../../../doctpl-lib/tests/data/test.pdf", {0, 1, 2});
}

BOOST_AUTO_TEST_SUITE_END()
