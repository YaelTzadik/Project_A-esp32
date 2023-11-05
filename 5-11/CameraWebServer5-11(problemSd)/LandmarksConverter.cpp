#include "HardwareSerial.h"
#include "LandmarkConverter.h"
#include <Arduino.h>

LandmarkConverter::LandmarkConverter()
{
  this->points = new vector<pair<uint16_t, uint16_t>>();
  this->targets_points = new vector<vector<pair<uint16_t, uint16_t>>>();
  this->targets_names = new vector<string>();
}

bool LandmarkConverter::is_ready() {
  return done;
}

bool LandmarkConverter::process_str(const string &message) {
  if (message.compare("end") == 0) {
    this->convert_str_to_arr();
    this->done = true;
  } else if (message.compare("start") == 0) {
    this->done = false;
    this->points_str = "";
    this->points->clear();
  } else {
    this->done = false;
    this->points_str = this->points_str + message;
  }
  return true;
}

void LandmarkConverter::convert_str_to_arr() {
  const string &input = this->points_str;
  char delimiter = '.';
  size_t start = 0;
  size_t end = input.find(delimiter);
  while (end < input.size()) {
    string token = input.substr(start, end - start);
    size_t dotPos = token.find(',');

    string first = token.substr(0, dotPos);
    string second = token.substr(dotPos + 1);

    this->points->emplace_back(atoi(first.c_str()),atoi(second.c_str()));

    start = end + 1;
    end = input.find(delimiter, start);
  }
}

void LandmarkConverter::convert_str_targets_to_arr(const string &message) {
  const string &input = message;
  char delimiter = ';';
  vector<pair<int, int>> result;
  size_t start = 0;
  size_t end = input.find(delimiter);
  int counter = 0;
  while (end < message.size()) {
    Serial.print(counter);
    string token = input.substr(start, end - start);
    size_t dotPos = token.find(':');

    string name_str = token.substr(0, dotPos);
    this->targets_names->push_back(name_str);
    this->points_str = token.substr(dotPos + 1);
    this->points->clear();
    this->convert_str_to_arr();
    this->targets_points->push_back(*this->points);
    start = end + 1;
    end = input.find(delimiter, start);
    counter++;
  }
  this->points->clear();
  this->done = false;
}

vector<pair<uint16_t, uint16_t>>* LandmarkConverter::get_points() {
  return this->points;
}
vector<vector<pair<uint16_t, uint16_t>>>* LandmarkConverter::get_targets_points() {
  return this->targets_points;
}
vector<string>* LandmarkConverter::get_targets_names() {
  return this->targets_names;
}

LandmarkConverter::~LandmarkConverter()
{
    delete points;  // Deallocate the dynamically allocated vector
    delete targets_points;  // Deallocate the dynamically allocated vector of vectors
    // No need to delete targets_names since it's a vector of strings, which cleans up itself
}