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


def set_face_detection(url: str, value: bool = True):
    try:
        requests.get(url + "/control?var=face_detect&val={} ".format(value))
    except:
        print("SET_FACE_DETECTION: something went wrong")


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

        # Convert to array of arrays of ints
        points = [[int(x) for x in sub_array] for sub_array in points]

        # Convert to sendble String
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


def distances_from_point(matrix, index=1):
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


def display_landmarks_numbered(points, points_2=[], image_size=size):
    # Create a blank image with the specified size and white background
    image = np.ones((image_size[1] * 2, image_size[0] * 2, 3), dtype=np.uint8) * 255
    points_stretched_original = stretch_and_align_points(points, size)

    indexes_to_extract = [463, 263, 133, 33, 1, 2, 98, 327, 0, 61, 17, 291, 152, 447, 366, 401, 227, 137, 177, 10]

    points = [points[idx] for idx in indexes_to_extract]

    points_stretched = stretch_and_align_points(points, size)

    for point in points_2:
        px, py = point
        pointi = [0, 0]
        pointi[0] = int(px / 10) + 50
        pointi[1] = int(py / 10) + 50
        cv2.circle(image, pointi, 1, (128, 128, 128), -1)

    for i, point in enumerate(points_stretched_original):
        point[0] = int(point[0] / 10) + 50
        point[1] = int(point[1] / 10) + 50
        cv2.circle(image, point, 1, (225, 0, 225), -1)

    for i, point in enumerate(points_stretched):
        point[0] = int(point[0] / 10) + 50
        point[1] = int(point[1] / 10) + 50
        cv2.circle(image, point, 1, (225, 100, 100), -1)
        cv2.putText(image, str(indexes_to_extract[i]), (point[0], point[1]), cv2.FONT_HERSHEY_SIMPLEX, 0.4, (0, 0, 0),
                    1, cv2.LINE_AA)

    return image


def identify(url, ids, target_points, face):
    min_tot_dist = 1000000  # lower number higher accuracy
    min_tot_id = -1
    for id in range(len(target_points)):
        dists = get_distance_faces(face, target_points[id])
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


def is_gray_pixel(frame):
    # Get the color values of pixel (1, 1)
    pixel_color = frame[1, 1]

    # Check if the pixel is gray
    is_gray = all(value == pixel_color[0] for value in pixel_color)

    return is_gray


def pick_points(points  #
                # indexes_to_extract=[463, 263, 133, 33, 1, 2, 98, 327, 0, 61, 17, 291, 152, 447, 366, 401, 227, 137, 177,
                #                     10]
                ):
    # points = [points[idx] for idx in indexes_to_extract]
    return points


def get_stream(url: str, ids, target_points):
    # create detector object
    detector = FaceLandMarks.FaceLandMarks()
    smile_detector = SmileDetector.SmileDetector()
    send_delayer = 0

    cropped = 0
    cropped_count = 0
    full = 0
    full_count = 0

    while True:
        cap = cv2.VideoCapture(url)
        if cap.isOpened():
            ret, frame = cap.read()

            _, jpeg_frame = cv2.imencode('.jpg', frame)
            leni = len(jpeg_frame.tobytes())
            if (is_gray_pixel(frame)):
                cropped = (cropped * cropped_count + leni) / (cropped_count + 1)
                cropped_count += 1
            else:
                full = (full * full_count + leni) / (full_count + 1)
                full_count += 1

            # print("len:" + str(leni) + " c:" + str(cropped) + " f:" + str(full))  # print standard variation

            if ret:
                # face landmarks
                img, faces = detector.findFaceLandmark(frame)
                # is_smiling = find_smile(faces)
                if len(faces) > 0:
                    cv2.imshow("Mesh", img)
                    cv2.waitKey(1)
                    send_delayer = (send_delayer + 1) % 3

                    # todo: disabled for testing other things
                    # if send_delayer == 0:
                    #     set_landmarks(url, faces[0])

                    # create_alignment_guide()
                    landmarks_numbered_img = display_landmarks_numbered(faces[0], target_points[0])
                    cv2.imshow("Numbered", landmarks_numbered_img)
                    cv2.waitKey(1)
                    # identify(url, ids, target_points, faces[0]) # original identify

                    # ---- identify with minimal landmarks
                    identify(url, ids, target_points, pick_points(faces[0]))  # original identify

                    # smile_detector.face_detected(faces[0])# todo: disabled for testing other things
                    # smile_detector.is_smile()# todo: disabled for testing other things
                # else:# todo: disabled for testing other things
                # smile_detector.no_face() # todo: disabled for testing other things
                # cv2.imshow("Mesh", frame) # todo: disabled for testing other things
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
    URL = "http://" + ip_add
    URL_CAP = "http://" + ip_add + "/capture"

    # set up only for esp32 active stream
    set_resolution(URL, index=5)
    print("set resolution 5/CIF")
    time.sleep(1)

    # set up only for esp32 active stream
    set_face_detection(URL, value=True)
    print("set face detection on")
    time.sleep(1)

    ids, target_points = read_csv_to_array("database_FACES.csv")

    # --- minimal points -----
    target_points_minimal = []
    for target in target_points:
        target_points_minimal.append(pick_points(target))
    get_stream(URL_CAP, ids, target_points_minimal)

    # get_stream(URL_CAP, ids, target_points) #todo: disabled for testing minimal points

    cv2.destroyAllWindows()
