
#ifndef FACERECOGNIZER_H_SMILEDETECTOR_H
#define FACERECOGNIZER_H_SMILEDETECTOR_H

#include <iostream>
#include <vector>
#include <cmath>
#include <Arduino.h>
using namespace std;
class SmileDetector {
private:
    vector<uint16_t>* corners_distances;
    vector<uint16_t>* lips_distances;
    uint16_t counter;
    const uint16_t MAX_FRAMES = 10;
    const uint16_t LEFT_CORNER = 61;
    const uint16_t RIGHT_CORNER = 291;
    const uint16_t TOP_LIP = 13;
    const uint16_t BOTTOM_LIP = 15;
    const uint16_t LEFT_NORM = 44;
    const uint16_t RIGHT_NORM = 274;

public:
    SmileDetector();

    void no_face();

    void face_detected(vector<pair<uint16_t,uint16_t>>* face);

    bool is_smile();

private:
    double distance(pair<uint16_t,uint16_t>& point1, pair<uint16_t,uint16_t>& point2);

    double mean(vector<uint16_t>::iterator begin, vector<uint16_t>::iterator end);

    ~SmileDetector();
};

#endif //FACERECOGNIZER_H_SMILEDETECTOR_H
