import csv
import threading
import SmileDetector
import requests
import time
import cv2
import numpy as np
import FaceLandMarks

size = (300, 300)

def set_resolution(url: str, index: int = 1, verbose: bool = False):
    try:
        if verbose:
            resolutions = "10: UXGA(1600x1200)\n9: SXGA(1280x1024)\n8: XGA(1024x768)\n7: SVGA(800x600)\n6: VGA(640x480)\n5: CIF(400x296)\n4: QVGA(320x240)\n3: HQVGA(240x176)\n0: QQVGA(160x120)"
            print("available resolutions\n{}".format(resolutions))

        if index in [10, 9, 8, 7, 6, 5, 4, 3, 0]:
            requests.get(url + "/control?var=framesize&val={}".format(index))
        else:
            print("Wrong index")
    except:
        print("SET_RESOLUTION: something went wrong")


def set_quality(url: str, value: int = 1, verbose: bool = False):
    try:
        if value >= 10 and value <= 63:
            requests.get(url + "/control?var=quality&val={}".format(value))
    except:
        print("SET_QUALITY: something went wrong")


def set_awb(url: str, awb: int = 1):
    try:
        awb = not awb
        requests.get(url + "/control?var=awb&val={}".format(1 if awb else 0))
    except:
        print("SET_QUALITY: something went wrong")
    return awb


def set_landmarks(url: str, points):
    try:
        url_new = url.strip("/capture")
        requests.post(url_new + ":81/landmarks?start")
        points_str = str(points).replace(' ', '').replace('],[', '.').replace('[', '').replace(']', '')
        chunk_size = 400
        for i in range(0, len(points_str), chunk_size):
            chunk = points_str[i:i + chunk_size]
            response = requests.post(url_new + ":81/landmarks?" + chunk)
            # You can handle the response here if needed
            print(f"Chunk {i // chunk_size + 1} sent. Response: {response.text}")

        requests.post(url_new + ":81/landmarks?end")
    except:
        print("SET_LANDMARKS: something went wrong")
    return 1


def stretch_and_align_points(points, size):
    new_max_x = size[0]
    new_min_x = 0
    new_max_y = size[1]
    new_min_y = 0

    x_values = [point[0] for point in points]
    y_values = [point[1] for point in points]

    min_x = min(x_values)
    max_x = max(x_values)
    min_y = min(y_values)
    max_y = max(y_values)

    stretched_points = []
    for point in points:
        new_x = int(10*(((point[0] - min_x) / (max_x - min_x)) * (new_max_x - new_min_x) + new_min_x))
        new_y = int(10*(((point[1] - min_y) / (max_y - min_y)) * (new_max_y - new_min_y) + new_min_y))
        stretched_points.append([new_x, new_y])

    return stretched_points


def distances_from_point(matrix, index=0):
    # Convert the input matrix to a NumPy array for easier manipulation
    points = np.array(matrix)

    # Get the coordinates of the point at the specified index
    base_point = points[index]

    # Calculate distances from the base point
    distances = np.linalg.norm(points - base_point, axis=1)

    return distances


def get_distance_faces(points, target_points):
    points_stretched = stretch_and_align_points(points, size)
    target_points = stretch_and_align_points(target_points, size)

    dists_live = distances_from_point(points_stretched)
    dists_target = distances_from_point(target_points)

    dists = []
    for i in range(len(dists_live)):
        dists.append(abs(dists_live[i] - dists_target[i]))
    return dists


def create_alignment_guide(points, target_points, image_size=size):
    # Create a blank image with the specified size and white background
    image = np.ones((image_size[1], image_size[0], 3), dtype=np.uint8) * 255

    indexes_to_extract = [0, 1, 6, 10, 13, 15, 17, 33, 58, 61, 127, 129, 152, 133, 145, 159, 221, 225, 263, 291,
                          288, 356, 358, 362, 386, 374, 441, 445]
    points = [points[idx] for idx in indexes_to_extract]
    target_points = [target_points[idx] for idx in indexes_to_extract]

    points_stretched = stretch_and_align_points(points, size)
    target_points = stretch_and_align_points(target_points, size)

    for point in target_points:
        cv2.circle(image, point, 1, (0, 0, 0), -1)
    # cv2.putText(image, str(i), (point[0], point[1]),
    #             cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 0, 0), 1, cv2.LINE_AA)

    for i, point in enumerate(points_stretched):
        cv2.circle(image, point, 1, (0, 0, 225), -1)
        if i == 10 or i == 152:
            cv2.circle(image, point, 1, (0, 225, 225), -1)
    # cv2.putText(image, str(i), (point[0], point[1]),
    #             cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 0, 0), 1, cv2.LINE_AA)

    return image


def identify(url, ids, target_points, faces):
    min_tot_dist = 10000  # lower number higher accuracy
    min_tot_id = -1
    for id in range(len(target_points)):
        dists = get_distance_faces(faces[0], target_points[id])
        img_guide = create_alignment_guide(faces[0], target_points[0])
        cv2.imshow("Guide", img_guide)
        dist_tot = sum(dists)
        if min_tot_dist > dist_tot:
            min_tot_dist = dist_tot
            min_tot_id = id
    if min_tot_id == -1:
        print(".", end="")
        return False
    else:
        print("\n" + ids[min_tot_id]+ "  " + str(min_tot_dist))
        return True


def distance(point1, point2):
    return np.sqrt((point1[0] - point2[0]) ** 2 + (point2[1] - point2[1]) ** 2)


def get_stream(url: str, ids, target_points):
    # create detector object
    detector = FaceLandMarks.FaceLandMarks()
    smile_detector = SmileDetector.SmileDetector()
    while True:
        cap = cv2.VideoCapture(url)
        if cap.isOpened():
            ret, frame = cap.read()
            if ret:
                # face landmarks
                img, faces = detector.findFaceLandmark(frame)
                # is_smiling = find_smile(faces)
                if len(faces) > 0:
                    cv2.imshow("Test", img)
                    # set_landmarks(url, faces[0])
                    identify(url, ids, target_points, faces)
                    smile_detector.face_detected(faces[0])
                    smile_detector.is_smile()
                else:
                    smile_detector.no_face()
                    cv2.imshow("Test", frame)
                cv2.waitKey(1)
    cv2.destroyAllWindows()
    cap.release()


def convert_strings_to_points(matrix):
    points_matrix = []
    for row in matrix:
        points_row = []
        for point_str in row:
            x, y = map(int, point_str.strip('[]').split(','))
            points_row.append((x, y))
        points_matrix.append(points_row)
    return points_matrix


def read_csv_to_array(file_path):
    points = []
    with open(file_path, 'r') as csvfile:
        csv_reader = csv.reader(csvfile)
        for row in csv_reader:
            points.append([point_str for point_str in row])

        ids = [row[0] for row in points]
        for row in points:
            del row[0]
        points = convert_strings_to_points(points)
    return ids, points


def ruun(ip_add: str):
    global counter
    global corners_dist
    global lips_dist

    # ESP32 URL
    # URL = "http://" + ip_add + ":81/capture" #fo_esp
    URL = "http://" + ip_add + "/capture"  # fo_esp
    print("URL " + URL)

    # set up only for esp32 active stream
    set_resolution(URL, index=10)
    time.sleep(1)

    ids, target_points = read_csv_to_array("database.csv")
    get_stream(URL, ids, target_points)

    cv2.destroyAllWindows()
