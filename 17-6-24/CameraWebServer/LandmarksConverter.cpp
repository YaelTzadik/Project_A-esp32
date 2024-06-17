#include "stdint.h"
#include "sys/_types.h"
#include "HardwareSerial.h"
#include "LandmarkConverter.h"
#include <Arduino.h>

LandmarkConverter::LandmarkConverter()
{
  this->points = new vector<pair<int16_t, int16_t>>();
  this->targets_points = new vector<vector<pair<int16_t, int16_t>>>();
  this->targets_names = new vector<String>();
}

bool LandmarkConverter::is_ready() {
  return done;
}
void LandmarkConverter::no_face() {
  return this->points->clear();
}

bool LandmarkConverter::process_str(const String &message) {
  if (message=="end") {
    this->convert_str_to_arr();
    this->done = true;
  } else if (message == "start") {
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
  const String &input = this->points_str;
  size_t start = 0;
  size_t end = input.indexOf(".");

  while (end < input.length()) {
    String token = input.substring(start, end - start);
    size_t dotPos = token.indexOf(",");

    String first = token.substring(0, dotPos);
    String second = token.substring(dotPos + 1);
    this->points->emplace_back(atoi(first.c_str()),atoi(second.c_str()));

    start = end + 1;
    end = input.indexOf(".", start);
  }
}

void LandmarkConverter::convert_str_targets_to_arr(const String &input) {
  const String& delimiter = ";";
  vector<pair<int16_t, int16_t>> result;
  unsigned start = 0;
  unsigned end = input.indexOf(delimiter);
  while (end < input.length()) {
    unsigned pos = input.indexOf(":", start);
    String name_str = input.substring(start, pos);
    this->targets_names->push_back(name_str);
    pos += 1;
    while(pos < end-1 && pos< input.length())
    {
      unsigned dot = input.indexOf(".",pos);
      const String& token = input.substring(pos,dot);
      unsigned psic = token.indexOf(",");

      String first = token.substring(0, psic);
      String second = token.substring(psic + 1);
      this->points->emplace_back(atoi(first.c_str()),atoi(second.c_str()));

      pos = dot+1;
    }
    this->targets_points->push_back(*this->points);
    start = end + 1;
    end = input.indexOf(delimiter, start);
  }
  this->points->clear();
  this->done = false;
}

vector<pair<int16_t, int16_t>>* LandmarkConverter::get_points() {
  return this->points;
}
vector<vector<pair<int16_t, int16_t>>>* LandmarkConverter::get_targets_points() {
  return this->targets_points;
}
vector<String>* LandmarkConverter::get_targets_names() {
  return this->targets_names;
}

LandmarkConverter::~LandmarkConverter()
{
    delete points;  // Deallocate the dynamically allocated vector
    delete targets_points;  // Deallocate the dynamically allocated vector of vectors
    // No need to delete targets_names since it's a vector of Strings, which cleans up itself
}