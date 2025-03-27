#ifndef TEST_H
#define TEST_H

#include <random>

class Test
{
public:
    Test() = delete;

    static bool isCorrectTest();
    static void performanceTest(std::mt19937_64& engine);

private:
    static bool isCorrectConversion();
    static bool isCorrectArithmetic();
    static bool isCorrectComparison();
    static bool isCorrectOperations();
    static bool isCorrectIO();

    static double standardPerformance(std::mt19937_64& engine);
    static double customPerformance(std::mt19937_64& engine);

    static constexpr size_t s_performanceTestIters = 1000000;
};

#endif
