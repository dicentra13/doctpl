#pragma once

#include <boost/test/unit_test.hpp>

#include <QApplication>

struct AppFixture {
    AppFixture()
        : app(
              boost::unit_test::framework::master_test_suite().argc,
              boost::unit_test::framework::master_test_suite().argv)
    {}

    QApplication app;
};
