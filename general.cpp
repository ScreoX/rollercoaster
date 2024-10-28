#include <iostream>
#include <fstream>
#include <vector>
#include "algorithm"

float findAvg(std::vector<float> numbers) {
    std::sort(numbers.begin(), numbers.end());

    size_t size = numbers.size();
    size_t elementsNumber = size * 0.33;

    float sum = 0.0;
    for (int i = elementsNumber; i < size - elementsNumber; ++i) {
        sum += numbers[i];
    }

    int count = size - 2 * elementsNumber;
    return sum / count;
}

float findFloor(std::vector<float>& numbers, float avg) {

    std::vector<float> up;

    for (auto i : numbers) {
        if (i > avg) {
            up.push_back(i);
        }
    }

    std::sort(up.begin(), up.end());

    size_t size = up.size();
    size_t lowElementsNumber = size * 0.5;
    size_t highElementsNumber = size * 0.33;

    float sum = 0.0;
    for (int i = lowElementsNumber; i < size - highElementsNumber; ++i) {
        sum += up[i];
    }

    int count = size - (lowElementsNumber + highElementsNumber);
    return sum / count;
}

std::vector<float> readBinaryFile(const std::string& filename) {
    std::vector<float> data;
    std::ifstream file(filename, std::ios::binary | std::ios::in);

    if (!file.is_open()) {
        std::cout << "Error with opening file\n";
        return data;
    }

    float value;
    while (file.read(reinterpret_cast<char*>(&value), sizeof(value))) {
        data.push_back(value);
    }

    file.close();
    return data;
}

std::vector<std::pair<int, int>> detectRollercoasters(const std::vector<float>& data, int minWidth, int maxWidth, float noiseLevel) {
    std::vector<std::pair<int, int>> results;
    bool flag = false;
    int startIndex = 0;

    for (size_t i = 1; i < data.size(); ++i) {
        if (!flag && data[i] > noiseLevel && data[i - 1] <= noiseLevel) {
            startIndex = i;
            flag = true;
        }

        if (flag && data[i] <= noiseLevel && data[i - 1] > noiseLevel) {
            int width = i - startIndex;
            if (width >= minWidth && width <= maxWidth) {
                results.emplace_back(startIndex, i);
            }
            flag = false;
        }
    }

    return results;
}

void writeResults(const std::vector<std::pair<int, int>>& results, const std::string& filename) {
    std::ofstream outFile(filename);

    if (outFile.is_open()) {
        for (const auto& rollercoaster : results) {
            outFile << rollercoaster.first << " " << rollercoaster.second << "\n";
        }
        outFile.close();
    } else {
        std::cout << "Error with opening file\n";
    }
}

#ifndef TESTING
int main(int argc, char* argv[]) {

    if (argc != 5) {
        std::cout << "Incorrect count of arguments\n";
        return 0;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    int minWidth = std::stoi(argv[3]);
    int maxWidth = std::stoi(argv[4]);

    std::vector<float> data = readBinaryFile(inputFile);

    float avg = findAvg(std::vector<float>(data));

    float floor = findFloor(data, avg);

    std::vector<std::pair<int,int>> results = detectRollercoasters(data, minWidth, maxWidth, floor);

    writeResults(results, outputFile);

    return 0;
}
#endif