#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>

float findAvg(std::vector<float> numbers) {
    std::sort(numbers.begin(), numbers.end());

    size_t size = numbers.size();
    size_t elementsNumber = size * 0.33;

    float sum = 0.0;
    for (size_t i = elementsNumber; i < size - elementsNumber; ++i) {
        sum += numbers[i];
    }

    size_t count = size - 2 * elementsNumber;
    return sum / count;
}

float findFloor(const std::vector<float>& numbers, float avg) {
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
    for (size_t i = lowElementsNumber; i < size - highElementsNumber; ++i) {
        sum += up[i];
    }

    size_t count = size - (lowElementsNumber + highElementsNumber);
    float floor = sum / count;

    float variance = 0.0;

    for (size_t i = lowElementsNumber; i < size - highElementsNumber; ++i) {
        variance += std::pow(up[i] - floor, 2);
    }

    variance /= count;
    float stddev = std::sqrt(variance);

    floor += 0.3 * stddev;

    return floor;
}

std::vector<float> readBinaryFile(const std::string& filename) {
    std::vector<float> data;
    std::ifstream file(filename, std::ios::binary | std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Error opening file";
        return data;
    }

    float value;
    while (file.read(reinterpret_cast<char*>(&value), sizeof(value))) {
        data.push_back(value);
    }

    file.close();
    return data;
}

std::vector<float> smoothData(const std::vector<float>& data, float alpha) {
    std::vector<float> smoothed;

    if (data.empty()) {
        return smoothed;
    }

    smoothed.push_back(data[0]);

    for (size_t i = 1; i < data.size(); ++i) {
        float s = alpha * data[i] + (1 - alpha) * smoothed.back();
        smoothed.push_back(s);
    }

    return smoothed;
}

std::vector<std::pair<int, int>> detectRollercoasters(
        const std::vector<float>& data,
        int minWidth,
        int maxWidth,
        float floor) {

    std::vector<std::pair<int, int>> results;
    bool flag = false;
    int startIndex = 0;

    float mean = std::accumulate(data.begin(), data.end(), 0.0f) / data.size();

    float variance = 0.0;
    for (auto val : data) {
        variance += std::pow(val - mean, 2);
    }

    variance /= data.size();
    float stddev = std::sqrt(variance);

    float amplitudeThreshold = 0.3 * stddev;

    for (size_t i = 1; i < data.size(); ++i) {
        if (!flag && data[i] > floor && data[i - 1] <= floor) {
            startIndex = i;
            flag = true;
        }

        if (flag && data[i] <= floor && data[i - 1] > floor) {
            int width = i - startIndex;
            if (width >= minWidth && width <= maxWidth) {
                float maxVal = *std::max_element(data.begin() + startIndex, data.begin() + i);
                if ((maxVal - floor) >= amplitudeThreshold) {
                    results.emplace_back(startIndex, i);
                }
            }
            flag = false;
        }
    }

    if (flag) {
        int width = data.size() - startIndex;
        if (width >= minWidth && width <= maxWidth) {
            float maxVal = *std::max_element(data.begin() + startIndex, data.end());
            if ((maxVal - floor) >= amplitudeThreshold) {
                results.emplace_back(startIndex, data.size());
            }
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
        std::cerr << "Error opening file for writing";
    }
}

#ifndef TESTING
int main(int argc, char* argv[]) {

    if (argc != 5) {
        std::cout << "Invalid number of arguments";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    int minWidth, maxWidth;

    try {
        minWidth = std::stoi(argv[3]);
        maxWidth = std::stoi(argv[4]);

        if (minWidth < 0 || maxWidth < 0) {
            throw std::out_of_range("Width cannot be negative");
        }
        if (minWidth > maxWidth) {
            throw std::invalid_argument("minWidth cannot be greater than maxWidth");
        }
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "One of the arguments is not a valid number: " << e.what() << "\n";
        return 1;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Argument out of range: " << e.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "An unknown error occurred while processing arguments.\n";
        return 1;
    }

    std::vector<float> data = readBinaryFile(inputFile);

    if (data.empty()) {
        std::cerr << "No data to process.\n";
        return 1;
    }

    data = smoothData(data, 0.3f);

    float avg = findAvg(data);
    float floor = findFloor(data, avg);

    std::vector<std::pair<int, int>> results = detectRollercoasters(data, minWidth, maxWidth, floor);

    writeResults(results, outputFile);

    return 0;
}
#endif
