import math
import numpy as np


def _distance_(point1, point2):
    return math.sqrt((point1[0] - point2[0]) ** 2 + (point1[1] - point2[1]) ** 2)


class SmileDetector:
    def __init__(self):

        self.corners_distances = []
        self.lips_distances = []
        self.counter = 0
        self.MAX_FRAMES = 10
        self.LEFT_CORNER = 61
        self.RIGHT_CORNER = 291
        self.TOP_LIP = 13
        self.BOTTOM_LIP = 15
        self.LEFT_NORM = 44
        self.RIGHT_NORM = 274
        self.NOSE = 1
        self.CHIN = 152

    def no_face(self):
        self.corners_distances = []
        self.lips_distances = []
        self.counter = 0

    def face_detected(self, face):
        if len(self.corners_distances) >= self.MAX_FRAMES:
            self.corners_distances.pop()
            self.lips_distances.pop()

        self.corners_distances.insert(0, _distance_(face[self.LEFT_CORNER], face[self.RIGHT_CORNER]) /
                                      _distance_(face[self.LEFT_NORM], face[self.RIGHT_NORM]))
        self.lips_distances.insert(0, _distance_(face[self.TOP_LIP], face[self.BOTTOM_LIP]) /
                                   _distance_(face[self.LEFT_NORM], face[self.RIGHT_NORM]))
        self.counter += 1

    def is_smile(self):
        if self.counter < self.MAX_FRAMES:
            return False

        corner_dist_after = np.mean(self.corners_distances[0:int(self.MAX_FRAMES / 2)])
        corner_dist_before = np.mean(self.corners_distances[int(self.MAX_FRAMES / 2) + 1: self.MAX_FRAMES])

        lips_dist_after = np.mean(self.lips_distances[0:int(self.MAX_FRAMES / 2)])
        lips_dist_before = np.mean(self.lips_distances[int(self.MAX_FRAMES / 2) + 1: self.MAX_FRAMES])

        if 1.5 * lips_dist_before > lips_dist_after:
            # print("not enough teethes")
            return False

        if 1.3 * corner_dist_before > corner_dist_after:
            print("not wide enough b:" + str(corner_dist_before) + " a:" + str(corner_dist_after))
            return False

        print("good smile")
        return True
