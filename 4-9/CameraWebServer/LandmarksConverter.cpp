#include "HardwareSerial.h"
#include "LandmarkConverter.h"

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
    this->points.clear();
  } else {
    this->done = false;
    this->points_str = this->points_str + message;
  }
  return true;
}

void LandmarkConverter::convert_str_to_arr() {
  const string &input = this->points_str;
  char delimiter = ',';
  std::vector<std::pair<string, string>> result;
  size_t start = 0;
  size_t end = input.find(delimiter);

  while (end != string::npos) {
    string token = input.substr(start, end - start);
    size_t dotPos = token.find('.');

    if (dotPos != string::npos) {
      string first = token.substr(0, dotPos);
      string second = token.substr(dotPos + 1);
      result.emplace_back(first, second);
    }

    start = end + 1;
    end = input.find(delimiter, start);
  }

  // Process the last token (or the only token if there are no delimiters)
  string lastToken = input.substr(start);
  size_t dotPos = lastToken.find('.');

  if (dotPos != string::npos) {
    string first = lastToken.substr(0, dotPos);
    string second = lastToken.substr(dotPos + 1);
    result.emplace_back(first, second);
  }
}

void LandmarkConverter::convert_str_targets_to_arr(const string &message) {
  
  const string &input = this->points_str;
  char delimiter = ';';
  vector<pair<int, int>> result;
  size_t start = 0;
  size_t end = input.find(delimiter);

  while (end != string::npos) {
    string token = input.substr(start, end - start);
    size_t dotPos = token.find(':');

    if (dotPos != string::npos) {
      string name_str = token.substr(0, dotPos);
      this->targets_names.push_back(name_str);
      this->points_str = token.substr(dotPos + 1);
      this->convert_str_to_arr();
      this->targets_points.push_back(this->points);
    }
    start = end + 1;
    end = input.find(delimiter, start);
  }
  this->points.clear();
  this->done = false;
}

vector<pair<int, int>> &LandmarkConverter::get_points() {
  return this->points;
}
vector<vector<pair<int, int>>> &LandmarkConverter::get_targets_points() {
  return this->targets_points;
}
vector<string> &LandmarkConverter::get_targets_names() {
  return this->targets_names;
}

