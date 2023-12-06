import csv
import threading
import SmileDetector
import requests
import time
import cv2
import numpy as np
import FaceLandMarks

size = (300, 300)


def pic_points(full_cap_Points, target_points):
    # indexes_to_extract = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    #                       26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    #                       49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
    #                       72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94,
    #                       95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114,
    #                       115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133,
    #                       134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
    #                       153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171,
    #                       172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190,
    #                       191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
    #                       210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228,
    #                       229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247,
    #                       248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266,
    #                       267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285,
    #                       286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304,
    #                       305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323,
    #                       324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342,
    #                       343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361,
    #                       362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380,
    #                       381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399,
    #                       400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418,
    #                       419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437,
    #                       438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456,
    #                       457, 458, 459, 460]
    indexes_to_extract = [463, 263, 133, 33, 1, 2, 98, 327, 0, 61, 17, 91, 152, 447, 366, 401, 227, 137, 177]


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
        new_x = int(10 * (((point[0] - min_x) / (max_x - min_x)) * (new_max_x - new_min_x) + new_min_x))
        new_y = int(10 * (((point[1] - min_y) / (max_y - min_y)) * (new_max_y - new_min_y) + new_min_y))
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
        print("\n" + ids[min_tot_id] + "  " + str(min_tot_dist))
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
