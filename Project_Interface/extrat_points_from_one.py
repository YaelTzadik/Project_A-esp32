import csv

import FaceLandMarks
import cv2
import os


def stretch_and_align_points(points):
    new_max_x = 500
    new_min_x = 0
    new_max_y = 500
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


def create_database_from_vid():
    ret, frame = cv2.VideoCapture(0).read()
    image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    cv2.imshow("test", image)
    cv2.waitKey(0)
    detector = FaceLandMarks.FaceLandMarks()
    image2, faces = detector.findFaceLandmark(image)
    cv2.imshow("test", image2)
    cv2.waitKey(0)
    if len(faces) > 0:
        face = faces[0]
        face_align = stretch_and_align_points(face)
        csv_file_path = 'eden.csv'
        with open(csv_file_path, newline='', mode='a') as csv_file:
            csv_writer = csv.writer(csv_file)
            csv_writer.writerow(face_align)  # Write data


detector = FaceLandMarks.FaceLandMarks()


def create_database_from(addr: str):
    frame = cv2.imread(addr)
    image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    detector = FaceLandMarks.FaceLandMarks()
    image2, faces = detector.findFaceLandmark(image)
    if len(faces) > 0:
        face = faces[0]
        face_align = [addr] + stretch_and_align_points(face)
        csv_file_path = 'database.csv'
        with open(csv_file_path, newline='', mode='a') as csv_file:
            csv_writer = csv.writer(csv_file)
            csv_writer.writerow(face_align)  # Write data


if __name__ == '__main__':
    create_database_from_vid()
    # Specify the directory containing the images
    image_directory = 'images'

    # Get a list of all files in the directory
    file_list = os.listdir(image_directory)

    # Filter the list to include only image files (you can add more image extensions if needed)
    image_files = [file for file in file_list if file.lower().endswith(('.png', '.jpg', '.jpeg', '.gif', '.bmp'))]

    # Create an array to store the image paths
    image_paths = [os.path.join(image_directory, image_file) for image_file in image_files]

    for addr in image_paths:
        create_database_from(addr)
