#include "SmileDetector.h"

SmileDetector::SmileDetector() {
  this->corners_distances = new vector<uint16_t>();
  this->lips_distances = new vector<uint16_t>();
  counter = 0;
}

void SmileDetector::no_face() {
  corners_distances->clear();
  lips_distances->clear();
  counter = 0;
}

void SmileDetector::face_detected(vector<pair<uint16_t, uint16_t>>* face_ptr) {
  Serial.println("detect smile");
  vector<pair<uint16_t, uint16_t>>& face = *face_ptr;
  if (corners_distances->size() >= MAX_FRAMES) {
    corners_distances->pop_back();
    lips_distances->pop_back();
  }
  Serial.println("1");
  corners_distances->insert(corners_distances->begin(), distance(face[LEFT_CORNER], face[RIGHT_CORNER]) / distance(face[LEFT_NORM], face[RIGHT_NORM]));
  lips_distances->insert(lips_distances->begin(), distance(face[TOP_LIP], face[BOTTOM_LIP]) / distance(face[LEFT_NORM], face[RIGHT_NORM]));
  counter++;
}

bool SmileDetector::is_smile() {
  if (counter < MAX_FRAMES) {
    return false;
  }

  double corner_dist_after = mean(corners_distances->begin(), corners_distances->begin() + (MAX_FRAMES / 2));
  double corner_dist_before = mean(corners_distances->begin() + (MAX_FRAMES / 2) + 1, corners_distances->end());

  double lips_dist_after = mean(lips_distances->begin(), lips_distances->begin() + (MAX_FRAMES / 2));
  double lips_dist_before = mean(lips_distances->begin() + (MAX_FRAMES / 2) + 1, lips_distances->end());

  if (1.5 * lips_dist_before > lips_dist_after) {
    // Serial.println("not enough teethes");
    return false;
  }

  if (1.2 * corner_dist_before > corner_dist_after) {
    // Serial.print("not wide enough b:");
    // Serial.print(corner_dist_before);
    // Serial.print(" a:");
    // Serial.println(corner_dist_after);
    return false;
  }

  return true;
}

double SmileDetector::distance(pair<uint16_t, uint16_t>& point1, pair<uint16_t, uint16_t>& point2) {
  return std::sqrt(std::pow(point1.first - point2.first, 2) + std::pow(point1.second - point2.second, 2));
}

double SmileDetector::mean(std::vector<uint16_t>::iterator begin, std::vector<uint16_t>::iterator end) {
  double sum = 0.0;
  int count = 0;

  for (auto it = begin; it != end; ++it) {
    sum += *it;
    count++;
  }

  if (count > 0) {
    return sum / count;
  } else {
    return 0.0;
  }
}

SmileDetector::~SmileDetector() {
  delete this->corners_distances;
  delete this->lips_distances;
}