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
    pair<int, int> size = std::make_pair(300, 300);

    vector<pair<int,int>> stretch_and_align_points(const vector<pair<int, int>> &points);

    vector<double> distances_from_point(const vector<pair<int,int>> &matrix, int index);

    std::vector<double> get_distance_faces(const vector<pair<int, int>> &points, const vector<pair<int, int>> &target_points);

public:
    string identify(const vector<string> &ids, const vector<vector<pair<int, int>>> &target_points, const vector<pair<int, int>> &face);

};

#endif //FACERECOGNIZER_FACERECOGNIZER_H
