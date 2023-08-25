import math


def _distance_(point1, point2):
    return math.sqrt((point1[0] - point2[0]) ** 2 + (point1[1] - point2[1]) ** 2)


class SmileDetector:
    def __init__(self):
        self.corners_distances = []
        self.lips_distances = []
        self.counter = 0
        self.MAX_FRAMES = 20
        self.LEFT_CORNER = 61
        self.RIGHT_CORNER = 261
        self.TOP_LIP = 13
        self.BOTTOM_LIP = 15

    def no_face(self):
        self.corners_distances = []
        self.lips_distances = []
        self.counter = 0

    def face_detected(self, face):
        if len(self.corners_distances) >= self.MAX_FRAMES:
            self.corners_distances.pop()
            self.lips_distances.pop()
        self.corners_distances.append(_distance_(face[self.LEFT_CORNER], face[self.RIGHT_CORNER]))
        self.lips_distances.append(_distance_(face[self.TOP_LIP], face[self.BOTTOM_LIP]))
        self.counter += 1

    # def is_smile(self):
        # mat(self.corners_distances[0:self.MAX_FRAMES / 2])
        # math.mean(self.corners_distances[self.MAX_FRAMES / 2 + 1:self.MAX_FRAMES])
