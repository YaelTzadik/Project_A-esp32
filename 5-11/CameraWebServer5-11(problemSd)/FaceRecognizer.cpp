#include "HardwareSerial.h"
//
// Created by ASUS on 28/08/2023.
//

#include "FaceRecognizer.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <Arduino.h>


vector<double> FaceRecognizer::distances_from_point(const vector<pair<uint16_t, uint16_t>>* matrix, int index = 0) {
  vector<double> distances;

  pair<uint16_t, uint16_t> base_point = (*matrix)[index];

  for (const auto& point : *matrix) {
    double distance = sqrt(pow(point.first - base_point.first, 2) + pow(point.second - base_point.second, 2));
    distances.push_back(distance);
  }

  return distances;
}

vector<double> FaceRecognizer::get_distance_faces(const vector<pair<uint16_t, uint16_t>>* points, const vector<pair<uint16_t, uint16_t>>* target_points) {
  vector<pair<uint16_t, uint16_t>> points_stretched = stretch_and_align_points(points);
  vector<pair<uint16_t, uint16_t>> target_points_stretched = stretch_and_align_points(target_points);

  vector<double> dists_live = distances_from_point(&points_stretched);
  vector<double> dists_target = distances_from_point(&target_points_stretched);

  vector<double> dists;
  for (size_t i = 0; i < dists_live.size(); ++i) {
    dists.push_back(abs(dists_live[i] - dists_target[i]));
  }
  return dists;
}

string FaceRecognizer::identify(const vector<string>* ids, const vector<vector<pair<uint16_t, uint16_t>>>* target_points, const vector<pair<uint16_t, uint16_t>>* face) {
  double min_tot_dist = 1400;
  int min_tot_id = -1;

  Serial.print("identify ");
  // Serial.println(target_points[0].size());
  // Iterate through each vector of pairs using a for loop
  for (size_t i = 0; i < target_points->size(); ++i) {
    const std::vector<std::pair<uint16_t, uint16_t>>& target = (*target_points)[i];

    vector<double> dists = get_distance_faces(face, &target);

    double dist_tot = accumulate(dists.begin(), dists.end(), 0.0);
    if (min_tot_dist > dist_tot) {
      min_tot_dist = dist_tot;
      min_tot_id = i;
    }
  }
  if (min_tot_id == -1) {
    return "-";
    //        cout << ".";
  } else {
    ;
    return (*ids)[min_tot_id];
    //        cout << "\n" << ids[min_tot_id] << endl;
  }
}

vector<pair<uint16_t, uint16_t>>
FaceRecognizer::stretch_and_align_points(const vector<pair<uint16_t, uint16_t>>* points) {
  int new_max_x = size.first;
  int new_min_x = 0;
  int new_max_y = size.second;
  int new_min_y = 0;

  vector<int> x_values, y_values;
  for (const auto& point : *points) {
    x_values.push_back(point.first);
    y_values.push_back(point.second);
  }

  int min_x = *min_element(x_values.begin(), x_values.end());
  int max_x = *max_element(x_values.begin(), x_values.end());
  int min_y = *min_element(y_values.begin(), y_values.end());
  int max_y = *max_element(y_values.begin(), y_values.end());

  vector<pair<uint16_t, uint16_t>> stretched_points;
  for (const auto& point : *points) {
    int new_x = static_cast<int>(((point.first - min_x) / static_cast<double>(max_x - min_x)) * (new_max_x - new_min_x) + new_min_x);
    int new_y = static_cast<int>(((point.second - min_y) / static_cast<double>(max_y - min_y)) * (new_max_y - new_min_y) + new_min_y);
    stretched_points.emplace_back(new_x, new_y);
  }

  return stretched_points;
}