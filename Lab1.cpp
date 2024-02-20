#include <iostream>
#include <thread>
#include <vector>
#include <chrono>


unsigned long long calcTheFact(int start, int end) {
    unsigned long long result = 1;
    for (int i = start; i <= end; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int number;
    bool validInput = false;

    while (!validInput) {
        std::cout << "Введите число для вычисления факториала: ";
        if (!(std::cin >> number)) {
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::cout << "Ошибка ввода. Введите число" << std::endl;
            continue;
        }
        if (number <= 0 || number > 20) {
            std::cout << "Некорректный ввод. Введите число от 1 до 20" << std::endl;
        }
        else {
            validInput = true;
        }
    }
    const int maxThreads = 4; 
    std::vector<std::thread> threads;
    std::vector<unsigned long long> results(maxThreads, 1);

    std::cout << "Вычисляем факториал числа: " << number << std::endl;

    int optimalThreads = 1; 
    auto minDuration = std::chrono::microseconds::max();
    for (int numThreads = 1; numThreads <= maxThreads; ++numThreads) {
        auto startTime = std::chrono::high_resolution_clock::now();
        const int range = number / numThreads;
        for (int i = 0; i < numThreads; i++) {
            int start = i * range + 1;
            int end = (i == numThreads - 1) ? number : (i + 1) * range;

            threads.emplace_back([start, end, &results, i]() {
                results[i] = calcTheFact(start, end);
                });
        }

        for (auto& thread : threads) {
            thread.join();
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
 
        std::cout << "Завершено с " << numThreads << " потоками. Время выполнения: " << duration.count() << " ìêñ" << std::endl;
        std::cout << "Факториал " << number << " равен " << calcTheFact(1, number) << std::endl;
\
        if (duration < minDuration) {
            minDuration = duration;
            optimalThreads = numThreads;
        }
        threads.clear();
    }
    std::cout << "Оптимальное количество потоков: " << optimalThreads << std::endl;

    return 0;
}
