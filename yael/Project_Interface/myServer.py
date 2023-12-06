import csv
import threading
import SmileDetector
import requests
import time
import cv2
import numpy as np
import FaceLandMarks


def pic_points(full_cap_Points, target_points):
    indexes_to_extract = [463, 263, 133, 33, 1, 2, 98, 327, 0, 61, 17, 91, 152, 447, 366, 401, 227, 137, 177]
    new_points = [full_cap_Points[idx] for idx in indexes_to_extract]
    target_points = [target_points[idx] for idx in indexes_to_extract]

    partial_cap_points = stretch_and_align_points(new_points, (1, 122))
    partial_target_points = stretch_and_align_points(target_points, (1, 122))
    return partial_cap_points, partial_target_points



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
        requests.get(url_new + ":81/landmarks?start")
        points_str = str(points).replace(' ', '').replace('],[', '.').replace('[', '').replace(']', '')
        chunk_size = 500
        for i in range(0, len(points_str), chunk_size):
            chunk = points_str[i:i + chunk_size]
            # response = requests.post(url_new + ":81/landmarks?" + chunk)
            # You can handle the response here if needed
            # print(f"Chunk {i // chunk_size + 1} sent. Response: {response.text}")

        requests.get(url_new + ":81/landmarks?end")
    except:
        print("SET_LANDMARKS: something went wrong")
    return 1


def stretch_and_align_points(points, size=(300, 300)):
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
        new_x = int(((point[0] - min_x) / (max_x - min_x)) * (new_max_x - new_min_x) + new_min_x)
        new_y = int(((point[1] - min_y) / (max_y - min_y)) * (new_max_y - new_min_y) + new_min_y)
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
    points_stretched = stretch_and_align_points(points, (300, 300))
    target_points = stretch_and_align_points(target_points, (300, 300))

    dists_live = distances_from_point(points_stretched)
    dists_target = distances_from_point(target_points)

    dists = []
    for i in range(len(dists_live)):
        dists.append(abs(dists_live[i] - dists_target[i]))
    return dists


def create_alignment_guide(points, target_points, image_size=(300, 300)):
    # Create a blank image with the specified size and white background
    image = np.ones((image_size[1], image_size[0], 3), dtype=np.uint8) * 255

    indexes_to_extract = [0, 1, 2, 267, 269, 270, 409, 91, 375, 321, 405, 314, 17, 84, 181, 91, 146, 61, 185, 40,
                          39, 37, 81, 358, 294, 455, 327, 460, 305, 290, 328, 326, 97, 99, 60, 75, 240, 98, 64, 129,
                          102, 48, 362, 398, 384, 385, 386, 387, 388, 466, 263, 249, 390, 373, 374, 380, 381, 382, 359, 130,
                          33, 246, 161, 160, 159, 158, 157, 173, 133, 155, 154, 153, 145, 144, 163, 7, 10, 338, 297, 332,
                          284, 251, 389, 356, 447, 454, 366, 323, 401, 361, 435, 288, 367, 397, 365, 379, 378, 400, 377, 152,
                          148, 176, 149, 150, 136, 172, 5, 215, 132, 177, 93, 137, 234, 227, 127, 34, 162, 21, 54, 103, 67, 109]
        # [0, 1, 6, 10, 13, 15, 17, 33, 58, 61, 127, 129, 152, 133, 145, 159, 221, 225, 263, 291,
        #                   288, 356, 358, 362, 386, 374, 441, 445]
    points = [points[idx] for idx in indexes_to_extract]
    target_points = [target_points[idx] for idx in indexes_to_extract]

    points_stretched = stretch_and_align_points(points, (300, 300))
    target_points = stretch_and_align_points(target_points, (300, 300))

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

def create_alignment_mini(points, target_points, image_size=(300, 300)):
    # Create a blank image with the specified size and white background
    image = np.ones((image_size[1], image_size[0], 3), dtype=np.uint8) * 255

    indexes_to_extract = [463, 263, 133, 33, 1, 2, 98, 327, 0, 61, 17, 91, 152, 447, 366, 401, 227, 137, 177]
    points = [points[idx] for idx in indexes_to_extract]
    target_points = [target_points[idx] for idx in indexes_to_extract]

    points_stretched = stretch_and_align_points(points, (300, 300))
    target_points = stretch_and_align_points(target_points, (300, 300))

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


def mini_image_create(points_mini):
    # Sample array of coordinates
    coordinates = [(50, 50), (150, 100), (200, 200), (300, 150)]

    # Create a white image
    image_size = (400, 400, 3)
    image = 255 * np.ones(image_size, dtype=np.uint8)  # 255 corresponds to white color

    # Draw black dots on the image
    for (x, y) in points_mini:
        cv2.circle(image, (x, y), 5, (0, 0, 0), -1)  # (0, 0, 0) is black color

    # Annotate each point with its index
    for i, (x, y) in enumerate(points_mini):
        cv2.putText(image, str(i + 1), (x - 5, y + 5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2, cv2.LINE_AA)

    # Save the image
    cv2.imwrite('numbered_dots_image.png', image)

    # Show the image using cv2.imshow
    cv2.imshow('Numbered Dots', image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()



def identify(url, ids, target_points, faces):
    min_tot_dist = 1400
    min_tot_id = -1
    for id in range(len(target_points)):
        #
        smaller_faces_p, smaller_target_p = pic_points(faces[0], target_points[id])
        #
        # mini_image_create(smaller_faces_p)

        img_mini = create_alignment_mini(faces[0], target_points[id])
        cv2.imshow("MINIMAL#",img_mini)
        dists = get_distance_faces(smaller_faces_p, smaller_target_p)
        # img_guide = create_alignment_guide(smaller_faces_p, smaller_target_p)
        # cv2.imshow("Guide", img_guide)
        dist_tot = sum(dists)
        if min_tot_dist > dist_tot:
            min_tot_dist = dist_tot
            min_tot_id = id
    if min_tot_id == -1:
        print(".", end="")
        return False
    else:
        print("\n" + ids[min_tot_id])
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
