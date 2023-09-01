#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "FaceRecognizer.h"

std::string readAndConcatenateFile(const std::string &fileName)
{
    std::ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        return "";
    }

    std::string concatenatedString;
    std::string line;

    while (std::getline(inputFile, line)) {
        size_t position = line.find('\r');
        if (position != std::string::npos) {
            line.replace(position, 1, "");
        }
        concatenatedString += line;
    }

    inputFile.close();

    return concatenatedString;
}

std::vector<std::vector<std::pair<int, int>>> convert_strings_to_points(const std::vector<std::vector<std::string>> &matrix)
{
    std::vector<std::vector<std::pair<int, int>>> points_matrix;

    for (const auto &row : matrix) {
        std::vector<std::pair<int, int>> points_row;
        for (const auto &point_str : row) {
            std::istringstream iss(point_str);
            std::string token;
            std::getline(iss, token, ',');
            int x = std::stoi(token);
            std::getline(iss, token, ',');
            int y = std::stoi(token);
            points_row.push_back(std::make_pair(x, y));
        }
        points_matrix.push_back(points_row);
    }

    return points_matrix;
}

std::pair<std::vector<std::string>, std::vector<std::vector<std::pair<int, int>>>> read_text_to_array(const std::string &file_path)
{
    std::vector<std::vector<std::string>> string_points;
    std::vector<std::string> ids;

    std::ifstream textfile(file_path);
    if (!textfile.is_open()) {
        std::cerr << "Error opening file: " << file_path << std::endl;
        return std::make_pair(ids, std::vector<std::vector<std::pair<int, int>>>());
    }

    std::string line;
    while (std::getline(textfile, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;
        while (ss >> cell) {
            row.push_back(cell);
        }
        string_points.push_back(row);
    }

    textfile.close();

    ids.reserve(string_points.size());
    std::transform(string_points.begin(), string_points.end(), std::back_inserter(ids), [](const std::vector<std::string> &row) {
        return row[0];
    });

    // Remove the first element (IDs) from each row
    for (auto &row : string_points) {
        row.erase(row.begin());
    }

    std::vector<std::vector<std::pair<int, int>>> points = convert_strings_to_points(string_points);

    return std::make_pair(ids, points);
}

int main()
{
    std::string fileName = "../input.txt"; // Replace with your file's path
    std::string input = readAndConcatenateFile(fileName);


    std::vector<std::string> row;
    std::stringstream ss(input);
    std::string cell;
    while (ss >> cell) {
        row.push_back(cell);
    }
    std::vector<std::pair<int, int>> face_to_id;
    for (const auto &point_str : row) {
        std::istringstream iss(point_str);
        std::string token;
        std::getline(iss, token, ',');
        int x = std::stoi(token);
        std::getline(iss, token, ',');
        int y = std::stoi(token);
        face_to_id.emplace_back(x, y);
    }


    // Provide the file path to your CSV file
    std::string file_path = "../database.txt";

    // Call the function to read text and convert to array
    std::pair<std::vector<std::string>, std::vector<std::vector<std::pair<int, int>>>> result = read_text_to_array(file_path);

    // Access the ids and points vectors from the result
    std::vector<std::string> ids = result.first;
    std::vector<std::vector<std::pair<int, int>>> faces_options = result.second;

    /**
     * // Print the ids and points (for testing purposes)
    for (size_t i = 0; i < ids.size(); ++i) {
        std::cout << "ID: " << ids[i] << ", Points: ";
        for (const auto &point : points[i]) {
            std::cout << "(" << point.first << ", " << point.second << ") ";
        }
        std::cout << std::endl;
    }**/

    FaceRecognizer faceRecognizer = FaceRecognizer();
    faceRecognizer.identify(ids, faces_options, face_to_id);

    return 0;
}
