#include "SmileDetector.h"

SmileDetector::SmileDetector() {
    counter = 0;
}

void SmileDetector::no_face() {
    corners_distances.clear();
    lips_distances.clear();
    counter = 0;
}

void SmileDetector::face_detected(std::vector<std::vector<int>> face) {
    if (corners_distances.size() >= MAX_FRAMES) {
        corners_distances.pop_back();
        lips_distances.pop_back();
    }

    corners_distances.insert(corners_distances.begin(), distance(face[LEFT_CORNER], face[RIGHT_CORNER]) /
                                                        distance(face[LEFT_NORM], face[RIGHT_NORM]));
    lips_distances.insert(lips_distances.begin(), distance(face[TOP_LIP], face[BOTTOM_LIP]) /
                                                  distance(face[LEFT_NORM], face[RIGHT_NORM]));
    counter++;
}

bool SmileDetector::is_smile() {
    if (counter < MAX_FRAMES) {
        return false;
    }

    double corner_dist_after = mean(corners_distances.begin(), corners_distances.begin() + (MAX_FRAMES / 2));
    double corner_dist_before = mean(corners_distances.begin() + (MAX_FRAMES / 2) + 1, corners_distances.end());

    double lips_dist_after = mean(lips_distances.begin(), lips_distances.begin() + (MAX_FRAMES / 2));
    double lips_dist_before = mean(lips_distances.begin() + (MAX_FRAMES / 2) + 1, lips_distances.end());

    if (1.5 * lips_dist_before > lips_dist_after) {
        std::cout << "not enough teethes" << std::endl;
        return false;
    }

    if (1.3 * corner_dist_before > corner_dist_after) {
        std::cout << "not wide enough b:" << corner_dist_before << " a:" << corner_dist_after << std::endl;
        return false;
    }

    std::cout << "good smile" << std::endl;
    return true;
}

double SmileDetector::distance(std::vector<int> point1, std::vector<int> point2) {
    return std::sqrt(std::pow(point1[0] - point2[0], 2) + std::pow(point1[1] - point2[1], 2));
}

double SmileDetector::mean(std::vector<int>::iterator begin, std::vector<int>::iterator end) {
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