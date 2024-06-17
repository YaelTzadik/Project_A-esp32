
#ifndef FACERECOGNIZER_H_SMILEDETECTOR_H
#define FACERECOGNIZER_H_SMILEDETECTOR_H

#include <iostream>
#include <vector>
#include <cmath>

class SmileDetector {
private:
    std::vector<int> corners_distances;
    std::vector<int> lips_distances;
    int counter;
    const int MAX_FRAMES = 10;
    const int LEFT_CORNER = 61;
    const int RIGHT_CORNER = 291;
    const int TOP_LIP = 13;
    const int BOTTOM_LIP = 15;
    const int LEFT_NORM = 44;
    const int RIGHT_NORM = 274;

public:
    SmileDetector();

    void no_face();

    void face_detected(std::vector<std::vector<int>> face);

    bool is_smile();

private:
    double distance(std::vector<int> point1, std::vector<int> point2);

    double mean(std::vector<int>::iterator begin, std::vector<int>::iterator end);
};

#endif //FACERECOGNIZER_H_SMILEDETECTOR_H
