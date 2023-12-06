import mediapipe as mp
import cv2


class FaceLandMarks():
    def __init__(self, static_mode=False, max_face=1, min_detection_con=0.5, min_track_con=0.5):
        self.staticMode = static_mode
        self.maxFace = max_face
        self.minDetectionCon = min_detection_con
        self.minTrackCon = min_track_con

        self.mpDraw = mp.solutions.drawing_utils
        self.mpFaceMesh = mp.solutions.face_mesh
        self.faceMesh = self.mpFaceMesh.FaceMesh(static_mode, max_face, False, self.minDetectionCon, self.minTrackCon)
        self.drawSpec = self.mpDraw.DrawingSpec(thickness=1, circle_radius=1)
        self.NUM_FACE = 1

    def findFaceLandmark(self, img, draw=True, acuurate=False):
        self.imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        self.results = self.faceMesh.process(self.imgRGB)

        faces = []

        if self.results.multi_face_landmarks:
            for faceLms in self.results.multi_face_landmarks:
                if draw:
                    self.mpDraw.draw_landmarks(img, faceLms, self.mpFaceMesh.FACEMESH_CONTOURS, self.drawSpec,
                                               self.drawSpec)

                face = []
                for id, lm in enumerate(faceLms.landmark):
                    ih, iw, ic = img.shape
                    x, y = float(lm.x * iw), float(lm.y * ih)
                    if acuurate:
                        factor = 1000
                        face.append([float(lm.x * iw * factor), float(lm.y * ih * factor)])
                    else:
                        face.append([x, y])
                faces.append(face)
        return img, faces












