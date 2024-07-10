#include "minunit.h"
#include "../include/game.h"

MU_TEST(test_calculateScore)
{
    mu_assert_int_eq(0, calculateScore(0));
    mu_assert_int_eq(1, calculateScore(1));
    mu_assert_int_eq(4, calculateScore(2));
    mu_assert_int_eq(9, calculateScore(3));
    mu_assert_int_eq(16, calculateScore(4));
}

MU_TEST_SUITE(test_suite)
{
    MU_RUN_TEST(test_calculateScore);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return MU_EXIT_CODE;
}