#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <cmath>

#include "Test.h"
#include "ratio_t.h"

bool Test::isCorrectTest()
{
    bool correctConversion = isCorrectConversion();
    bool correctArithmetic = isCorrectArithmetic();
    bool correctComparison = isCorrectComparison();
    bool correctOperations = isCorrectOperations();
    bool correctIO = isCorrectIO();

    std::cout << "Correct initialization/convertion: " << (
          correctConversion ? "ok" : "ERROR"
    ) << '\n';

    std::cout << "Correct arithmetic: " << (
          correctArithmetic ? "ok" : "ERROR"
    ) << '\n';

    std::cout << "Correct comparison: " << (
          correctComparison ? "ok" : "ERROR"
    ) << '\n';

    std::cout << "Correct operations: " << (
          correctOperations ? "ok" : "ERROR"
    ) << '\n';

    std::cout << "Correct input/output: " << (
          correctIO ? "ok" : "ERROR"
    ) << '\n';

    return correctConversion
        && correctArithmetic
        && correctComparison
        && correctOperations
        && correctIO;
}

void Test::performanceTest(std::mt19937_64& engine)
{
    std::cout << "Testing performance...\n";

    double standardTime = Test::standardPerformance(engine);
    double customTime = Test::customPerformance(engine);

    std::cout << "Standard double time spent: " << standardTime << " milliseconds\n";
    std::cout << "Custom ratio time spent: " << customTime << " milliseconds\n";
    std::cout << "Spent in " <<
        std::round(customTime * 1000 / standardTime) / 1000
    << " more time with custom implementation\n";
}

bool Test::isCorrectConversion()
{
    bool ok = ratio_t{} == ratio_t(0ll);
    ok = ok && (static_cast<double>(ratio_t(2.02345)) == 2.02345);
    ok = ok && (ratio_t(2ll) == ratio_t(2.0));
    ok = ok && (ratio_t(2ll) != ratio_t(2.1));
    ok = ok && (ratio_t(2ll) == ratio_t(10, 5));
    ok = ok && (ratio_t(-2ll) == ratio_t(-10, 5));
    ok = ok && (ratio_t(-2ll) == ratio_t(10, -5));
    ok = ok && (ratio_t(2ll) == ratio_t(-10, -5));
    ok = ok && (ratio_t(2, 3) == ratio_t(4, 6));
    ok = ok && (ratio_t(2, 3) != ratio_t(4, 5));
    ok = ok && (ratio_t(2, 3) != ratio_t(0.6666));
    ok = ok && (ratio_t(150, 200) == ratio_t(0.75));

    return ok;
}

bool Test::isCorrectArithmetic()
{
    bool ok = ratio_t(2, 3) + ratio_t(2, 3) == ratio_t(4, 3);
    ok = ok && (ratio_t(2ll) - ratio_t(2, 3) == ratio_t(4, 3));
    ok = ok && (ratio_t(2.1) - ratio_t(1.8) == ratio_t(3, 10));
    ok = ok && (ratio_t(2.1) + ratio_t(-1.8) == ratio_t(3, 10));
    ok = ok && (ratio_t(2ll) * ratio_t(-9, -3) == ratio_t(6ll));
    ok = ok && (ratio_t(2ll) * ratio_t(-3.5) == ratio_t(7, -1));
    ok = ok && (ratio_t(2.0) / ratio_t(3ll) == ratio_t(2, 3));
    ok = ok && (ratio_t(-2ll) / ratio_t(3.0) == ratio_t(2, -3));
    ok = ok && (ratio_t(-2ll) / ratio_t(-3ll) == ratio_t(2, 3));

    ratio_t testRatio(2, 9);
    testRatio += testRatio * ratio_t(2ll);
    ok = ok && (testRatio == ratio_t(20, 30));
    ratio_t anotherRatio = testRatio;
    anotherRatio -= testRatio * ratio_t(2ll);
    ok = ok && (anotherRatio == -testRatio);
    ok = ok && (-(-testRatio) == testRatio);
    ok = ok && (+testRatio == testRatio);
    anotherRatio *= anotherRatio;
    ok = ok && (anotherRatio == ratio_t(4, 9));
    anotherRatio /= anotherRatio;
    ok = ok && (anotherRatio == ratio_t(1ll));

    ok = ok && (ratio_t(1, 0) - ratio_t(1, 0) == ratio_t(0ll));
    ok = ok && (ratio_t(-1, 0) + ratio_t(1, 0) == ratio_t(0ll));
    ok = ok && (ratio_t(1, 0) + ratio_t(-1, 0) == ratio_t(0ll));
    ok = ok && (ratio_t(-1, 0) + ratio_t(99999ll) == ratio_t(-1, 0));
    ok = ok && (ratio_t(-1, 0) * ratio_t(1, 0) == ratio_t(-1, 0));
    ok = ok && (ratio_t(-1, 0) / ratio_t(1, 0) == ratio_t(-1ll));

    ok = ok && (ratio_t(1ll) + ratio_t(0, 0) == ratio_t(0, 0));
    ok = ok && (ratio_t(0, 0) - ratio_t(1ll) == ratio_t(0, 0));
    ok = ok && (ratio_t(0, 0) * ratio_t(1ll) == ratio_t(0, 0));
    ok = ok && (ratio_t(1ll) / ratio_t(0, 0) == ratio_t(0, 0));

    ok = ok && (ratio_t(1, 0) + ratio_t(0, 0) == ratio_t(0, 0));
    ok = ok && (ratio_t(0, 0) + ratio_t(0, 0) == ratio_t(0, 0));

    return ok;
}

bool Test::isCorrectComparison()
{
    bool ok = ratio_t(2ll) < ratio_t(7, 2);
    ok = ok && (ratio_t(7, 2) > ratio_t(2ll));
    ok = ok && (ratio_t(2.0) == ratio_t(2ll));
    ok = ok && (ratio_t(2.0) <= ratio_t(2ll));
    ok = ok && (ratio_t(2.0) >= ratio_t(2ll));
    ok = ok && (ratio_t(1.9) <= ratio_t(2ll));
    ok = ok && (ratio_t(2.1) >= ratio_t(2ll));
    ok = ok && (ratio_t(2, 3) > ratio_t(0.6666));
    ok = ok && (ratio_t(2, 3) < ratio_t(0.6667));
    ok = ok && (ratio_t(2, 3) > ratio_t(0.6666));
    ok = ok && (ratio_t(2, 3) < ratio_t(0.6667));
    ok = ok && (ratio_t(1, 0) > ratio_t(99999ll));
    ok = ok && (ratio_t(-1, 0) < ratio_t(-99999ll));
    ok = ok && (ratio_t(1, 0) == ratio_t(100, 0));
    ok = ok && (ratio_t(-1, 0) == ratio_t(-100, 0));
    ok = ok && (ratio_t(-1, 0) != ratio_t(1, 0));
    ok = ok && (ratio_t(-1, 0) < ratio_t(1, 0));
    ok = ok && (ratio_t(0, 0) < ratio_t(1, 0));
    ok = ok && (ratio_t(0, 0) > ratio_t(-1, 0));
    ok = ok && (ratio_t(0, 0) < ratio_t(99999ll));
    ok = ok && (ratio_t(0, 0) < ratio_t(-99999ll));
    ok = ok && (ratio_t(0, 0) == ratio_t(0, 0));

    return ok;
}

bool Test::isCorrectOperations()
{
    ratio_t testRatio(7, 9);
    ratio_t anotherRatio(2, 9);

    bool ok = ratio_t(testRatio).ceil() == ratio_t(1ll);
    ok = ok && (ratio_t(anotherRatio).ceil() == ratio_t(1ll));

    ok = ok && (ratio_t(testRatio).floor() == ratio_t(0ll));
    ok = ok && (ratio_t(anotherRatio).floor() == ratio_t(0ll));

    ok = ok && (ratio_t(testRatio).round() == ratio_t(1ll));
    ok = ok && (ratio_t(anotherRatio).round() == ratio_t(0ll));

    ok = ok && (ratio_t(0, 0).ceil() == ratio_t(0, 0));
    ok = ok && (ratio_t(1, 0).ceil() == ratio_t(1, 0));
    ok = ok && (ratio_t(-1, 0).ceil() == ratio_t(-1, 0));

    ok = ok && (ratio_t(0, 0).floor() == ratio_t(0, 0));
    ok = ok && (ratio_t(1, 0).floor() == ratio_t(1, 0));
    ok = ok && (ratio_t(-1, 0).floor() == ratio_t(-1, 0));

    ok = ok && (ratio_t(0, 0).round() == ratio_t(0, 0));
    ok = ok && (ratio_t(1, 0).round() == ratio_t(1, 0));
    ok = ok && (ratio_t(-1, 0).round() == ratio_t(-1, 0));

    ok = ok && (ratio_t(testRatio).pow(0) == ratio_t(1ll));
    ok = ok && (ratio_t(testRatio).pow(1) == testRatio);
    ok = ok && (ratio_t(testRatio).pow(2) == ratio_t(49, 81));
    ok = ok && (ratio_t(testRatio).pow(5) == ratio_t(16807, 59049));
    ok = ok && (ratio_t(testRatio).pow(-1) == ratio_t(9, 7));
    ok = ok && (ratio_t(testRatio).pow(-5) == ratio_t(59049, 16807));

    ok = ok && (ratio_t(0, 0).pow(0) == ratio_t(0, 0));
    ok = ok && (ratio_t(0, 0).pow(2) == ratio_t(0, 0));
    ok = ok && (ratio_t(0, 0).pow(-9) == ratio_t(0, 0));
    ok = ok && (ratio_t(1, 0).pow(42) == ratio_t(1, 0));
    ok = ok && (ratio_t(1, 0).pow(0) == ratio_t(1ll));
    ok = ok && (ratio_t(1, 0).pow(-7) == ratio_t(0ll));
    ok = ok && (ratio_t(-1, 0).pow(3) == ratio_t(-1, 0));
    ok = ok && (ratio_t(-1, 0).pow(0) == ratio_t(1ll));
    ok = ok && (ratio_t(-1, 0).pow(-79) == ratio_t(0ll));

    return ok;
}

bool Test::isCorrectIO()
{
    std::stringstream stream{};
    ratio_t testRatio{};

    stream << ratio_t(6, 7);
    stream >> testRatio;
    stream.clear();
    bool ok = testRatio == ratio_t(6, 7);

    stream << ratio_t(0, 0);
    stream >> testRatio;
    stream.clear();
    ok = ok && (testRatio == ratio_t(0, 0));

    stream << ratio_t(103, 0);
    stream >> testRatio;
    stream.clear();
    ok = ok && (testRatio == ratio_t(1, 0));

    stream << ratio_t(-42, 0);
    stream >> testRatio;
    stream.clear();
    ok = ok && (testRatio == ratio_t(-1, 0));

    std::string strRepr{};

    stream << ratio_t(5, 10);
    stream >> strRepr;
    stream.clear();
    ok = ok && (strRepr == "1/2");

    stream << ratio_t(0, 0);
    stream >> strRepr;
    stream.clear();
    ok = ok && (strRepr == "NaN");

    stream << ratio_t(6357, 0);
    stream >> strRepr;
    stream.clear();
    ok = ok && (strRepr == "Inf");

    stream << ratio_t(-973, 0);
    stream >> strRepr;
    stream.clear();
    ok = ok && (strRepr == "-Inf");

    return ok;
}

double Test::standardPerformance(std::mt19937_64& engine)
{
    std::uniform_int_distribution<long long> numeratorDistribution(-1000, 1000);
    std::uniform_int_distribution<long long> denominatorDistribution(1000, 2000);

    auto startTime = std::chrono::steady_clock::now();

    double ratioSum = 0.0;
    for (size_t i = 0; i < s_performanceTestIters; i++)
    {
        ratioSum += (i & 1 ? -1.0 : 1.0)
            * (static_cast<double>(numeratorDistribution(engine)) / static_cast<double>(denominatorDistribution(engine)))
            * (static_cast<double>(numeratorDistribution(engine)) / static_cast<double>(denominatorDistribution(engine)));
    }

    return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - startTime).count()) / 1000.0;
}

double Test::customPerformance(std::mt19937_64& engine)
{
    std::uniform_int_distribution<long long> numeratorDistribution(-1000, 1000);
    std::uniform_int_distribution<long long> denominatorDistribution(1000, 2000);

    auto startTime = std::chrono::steady_clock::now();

    ratio_t ratioSum;
    for (size_t i = 0; i < s_performanceTestIters; i++)
    {
        ratioSum += (i & 1 ? ratio_t(-1ll) : ratio_t(1ll))
            * ratio_t(numeratorDistribution(engine), denominatorDistribution(engine))
            * ratio_t(numeratorDistribution(engine), denominatorDistribution(engine));

    }

    return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - startTime).count()) / 1000.0;
}
