
#include <iostream>
#include <gtest/gtest.h>
#include "hw1.h"

int main(int argc, char **argv)
{
    if (false) // make false to run unit-tests
    {
        // debug section
        Matrix matrix1{algebra::random(2, 3, 0, 1)};
        Matrix matrix2{algebra::random(4, 3, 0, 1)};
        Matrix matrix{algebra::concatenate(matrix1, matrix2, 0)};
        algebra::show(matrix);
    }
    else
    {
        ::testing::InitGoogleTest(&argc, argv);
        std::cout << "RUNNING TESTS ..." << std::endl;
        int ret{RUN_ALL_TESTS()};
        if (!ret)
            std::cout << "<<<SUCCESS>>>" << std::endl;
        else
            std::cout << "FAILED" << std::endl;
    }
    return 0;
}