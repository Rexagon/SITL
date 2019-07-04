//
// Created by i.kalinin on 04.07.2019.
//

#define CATCH_CONFIG_MAIN

#include <catch.hpp>

#include "sitl.h"

TEST_CASE("Some global cast")
{
    SECTION("Some local section")
    {
        std::vector<int> test = {1, 2, 3};

        REQUIRE(test.size() == 3);
    }
}
