//
// Created by ASUS on 02/09/2023.
//
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>

#ifndef FACERECOGNIZER_H_LANDMARKCONVERTER_H
#define FACERECOGNIZER_H_LANDMARKCONVERTER_H
using namespace std;

class LandmarkConverter {
  public:
  String points_str;
  vector<pair<int16_t, int16_t>>* points;
  vector<vector<pair<int16_t, int16_t>>>* targets_points;
  vector<String>* targets_names;
  bool done = false;


  LandmarkConverter();
  bool is_ready();
  void no_face();
  bool process_str(const String& message);
  vector<pair<int16_t, int16_t>>* get_points();
  vector<vector<pair<int16_t, int16_t>>>* get_targets_points();
  vector<String>* get_targets_names();
  void convert_str_to_arr();
  void convert_str_targets_to_arr(const String& message);
  ~LandmarkConverter();
};


#endif  //FACERECOGNIZER_H_LANDMARKCONVERTER_H