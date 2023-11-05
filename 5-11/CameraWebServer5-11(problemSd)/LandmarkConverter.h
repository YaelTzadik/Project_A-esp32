#include "stdint.h"
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
  public:
  string points_str;
  vector<pair<uint16_t,uint16_t>>* points;
  vector<vector<pair<uint16_t,uint16_t>>>* targets_points;
  vector<string>* targets_names;
  bool done = false;


  LandmarkConverter();
  bool is_ready();
  bool process_str(const string& message);
  vector<pair<uint16_t, uint16_t>>* get_points();
  vector<vector<pair<uint16_t, uint16_t>>>* get_targets_points();
  vector<string>* get_targets_names();
  void convert_str_to_arr();
  void convert_str_targets_to_arr(const string& message);
  ~LandmarkConverter();
};


#endif  //FACERECOGNIZER_H_LANDMARKCONVERTER_H