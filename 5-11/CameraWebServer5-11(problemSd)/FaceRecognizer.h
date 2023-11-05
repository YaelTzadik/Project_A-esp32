//
// Created by ASUS on 28/08/2023.
//

#ifndef FACERECOGNIZER_FACERECOGNIZER_H
#define FACERECOGNIZER_FACERECOGNIZER_H

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class FaceRecognizer {

private:
    pair<uint16_t, uint16_t> size = std::make_pair(300, 300);

    vector<pair<uint16_t,uint16_t>> stretch_and_align_points(const vector<pair<uint16_t, uint16_t>> *points);

    vector<double> distances_from_point(const vector<pair<uint16_t,uint16_t>> *matrix, int index);

    std::vector<double> get_distance_faces(const vector<pair<uint16_t, uint16_t>> *points, const vector<pair<uint16_t, uint16_t>> *target_points);

public:
    string identify(const vector<string> *ids, const vector<vector<pair<uint16_t, uint16_t>>> *target_points, const vector<pair<uint16_t, uint16_t>> *face);

};

#endif //FACERECOGNIZER_FACERECOGNIZER_H
