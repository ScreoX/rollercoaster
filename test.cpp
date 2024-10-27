#include <gtest/gtest.h>
#include <vector>
#include <fstream>
#include <string>
#include <utility>

std::vector<float> readBinaryFile(const std::string& filename);
float findAvg(std::vector<float> numbers);
float findFloor(std::vector<float>& numbers, float avg);
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
        throw std::runtime_error("Failed to open expected results file.");
    }

    return expectedResults;
}

TEST(RollercoasterDetectionTest, DetectRollercoasters) {
    std::vector<float> data = readBinaryFile(INPUT_FILE);
    std::vector<std::pair<int, int>> expectedResults = readExpectedResults(EXPECTED_OUTPUT_FILE);

    float avg = findAvg(data);
    float floor = findFloor(data, avg);

    std::vector<std::pair<int, int>> results = detectRollercoasters(data, MIN_WIDTH, MAX_WIDTH, floor);

    ASSERT_EQ(results.size(), expectedResults.size());

    for (size_t i = 0; i < expectedResults.size(); ++i) {
        EXPECT_EQ(results[i].first, expectedResults[i].first);
        EXPECT_EQ(results[i].second, expectedResults[i].second);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
