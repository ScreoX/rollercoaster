#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include <stdexcept>

std::vector<float> readBinaryFile(const std::string& filename);
float findAvg(std::vector<float> numbers);
float findFloor(const std::vector<float>& numbers, float avg);
std::vector<float> smoothData(const std::vector<float>& data, float alpha);
std::vector<std::pair<int, int>> detectRollercoasters(const std::vector<float>& data, int minWidth, int maxWidth, float noiseLevel);

const int MIN_WIDTH = 3;
const int MAX_WIDTH = 1000;
const std::string INPUT_FILE = "D:/InternShip/internship_data/rollercoaster/detector_src_32f.bin";
const std::string EXPECTED_OUTPUT_FILE = "D:/InternShip/internship_data/rollercoaster/expected_results.txt";

std::vector<std::pair<int, int>> readExpectedResults(const std::string& filename) {
    std::vector<std::pair<int, int>> expectedResults;
    std::ifstream file(filename);

    if (file.is_open()) {
        int start, end;
        while (file >> start >> end) {
            expectedResults.emplace_back(start, end);
        }
        file.close();
    } else {
        throw std::runtime_error("Failed to open expected results file: " + filename);
    }

    return expectedResults;
}

TEST(RollercoasterDetectionTest, DetectRollercoasters) {
    std::vector<float> data = readBinaryFile(INPUT_FILE);
    ASSERT_FALSE(data.empty()) << "Data should not be empty after reading the binary file.";

    data = smoothData(data, 0.3f);
    ASSERT_FALSE(data.empty()) << "Smoothed data should not be empty.";

    float avg = findAvg(data);
    float floor = findFloor(data, avg);

    std::vector<std::pair<int, int>> results = detectRollercoasters(data, MIN_WIDTH, MAX_WIDTH, floor);

    std::vector<std::pair<int, int>> expectedResults = readExpectedResults(EXPECTED_OUTPUT_FILE);

    ASSERT_EQ(results.size(), expectedResults.size()) << "Number of detected rollercoasters does not match expected.";

    for (size_t i = 0; i < expectedResults.size(); ++i) {
        ASSERT_EQ(results[i].first, expectedResults[i].first) << "Mismatch in start index at rollercoaster " << i;
        ASSERT_EQ(results[i].second, expectedResults[i].second) << "Mismatch in end index at rollercoaster " << i;
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
