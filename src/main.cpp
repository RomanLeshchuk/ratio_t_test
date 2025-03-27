#include <iostream>

#include "Test.h"

using namespace std;

int main()
{
    bool isCorrect = Test::isCorrectTest();
    std::cout << "Library is " << (isCorrect ? "" : "in") << "correct\n";

    std::random_device device{};
    std::mt19937_64 engine(device());
    Test::performanceTest(engine);

    return 0;
}
