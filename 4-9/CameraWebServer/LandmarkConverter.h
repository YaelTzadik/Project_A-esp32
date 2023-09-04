//
// Created by ASUS on 02/09/2023.
//
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string.h>

#ifndef FACERECOGNIZER_H_LANDMARKCONVERTER_H
#define FACERECOGNIZER_H_LANDMARKCONVERTER_H
using namespace std;

class LandmarkConverter {
  string points_str;
  vector<pair<int,int>> points = vector<pair<int, int>>();
  vector<vector<pair<int,int>>> targets_points = vector<vector<pair<int, int>>>();
  vector<string> targets_names;
  bool done = false;

public:
  bool is_ready();
  bool process_str(const string& message);
  vector<pair<int, int>>& get_points();
  vector<vector<pair<int, int>>>& get_targets_points();
  vector<string>& get_targets_names();
  void convert_str_to_arr();
  void convert_str_targets_to_arr(const string& message);
};


#endif  //FACERECOGNIZER_H_LANDMARKCONVERTER_H