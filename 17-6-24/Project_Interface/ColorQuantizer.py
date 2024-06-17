import os
import cv2
import numpy as np
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
import math


def extract_eyes(image_path, output_dir):
    # Load pre-trained face and eye classifiers
    face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
    eye_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_eye.xml')

    # Read the image
    image = cv2.imread(image_path)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Detect faces in the image
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5)

    # Initialize an empty list to store eye images
    eye_images = []

    for (x, y, w, h) in faces:
        # Region of interest (ROI) for the face
        roi_gray = gray[y:y + h, x:x + w]
        roi_color = image[y:y + h, x:x + w]

        # Detect eyes in the face region
        eyes = eye_cascade.detectMultiScale(roi_gray)

        for (ex, ey, ew, eh) in eyes:
            # Extract the region of interest (ROI) around each eye
            eye_region = roi_color[ey:ey + eh, ex:ex + ew]

            # Append the eye region to the list
            eye_images.append(eye_region)

            # Save the eye image to the output directory
            image_name = os.path.basename(image_path)
            eye_filename = f"{output_dir}/eye_{image_name}_{len(eye_images)}.jpg"
            cv2.imwrite(eye_filename, eye_region)
    ''''main code
        # Create the output directory if it doesn't exist
        os.makedirs(output_directory, exist_ok=True)

        image_paths = np.array(image_matrix)[:, 3]

        # Initialize an empty list to store eye images for all faces
        all_eye_images = []

        for image_name in image_paths:
            # Extract eyes from each image
            eyes = extract_eyes(os.path.join(input_directory, image_name), output_directory)
            print(image_name)
            # Append the eye images to the list
            all_eye_images.extend(eyes)

        # Convert the list of eye images to a NumPy array
        all_eye_images_array = np.array(all_eye_images)

        # Print the shape of the resulting array
        print("Shape of all_eye_images_array:", all_eye_images_array.shape)

        # Display the first eye image (you can modify this part based on your requirements)
        if all_eye_images_array.shape[0] > 0:
            cv2.imshow('First Eye Image', all_eye_images_array[0])
            cv2.waitKey(0)
            cv2.destroyAllWindows()
        else:
            print("No eyes found in the provided images.")'''
    return eye_images


def load_face_images_to_matrix(directory_path):
    # Dictionary to store image names grouped by number
    images_by_number = {}

    for filename in os.listdir(directory_path):
        file_path = os.path.join(directory_path, filename)

        # Check if it's a file
        if os.path.isfile(file_path):
            # Split the filename by "_"
            name_parts = filename.split("_")

            # Check if there are at least 4 parts and if the 4th part is neither "h" nor "n"
            if len(name_parts) >= 4 and name_parts[3] in ["h", "n"]:
                # Extract the number from the filename
                number = name_parts[0]

                # Create a key for this number if it doesn't exist
                if number not in images_by_number:
                    images_by_number[number] = []

                # Add the filename to the list for this number
                images_by_number[number].append(filename)

    # Create a matrix with rows containing 4 images with the same number
    matrix = []
    for number, filenames in sorted(images_by_number.items(), key=lambda x: (int(x[0]), x[0][1] != 'h')):
        while len(filenames) % 4 != 0:
            filenames.append(None)  # Pad with None if the number of images is not a multiple of 4
        matrix.extend([[int(number)] + filenames[i:i + 4] for i in range(0, len(filenames), 4)])

    return matrix


def quantize_colors(image, k):
    # Reshape the image to be a list of pixels
    pixels = image.reshape((-1, 3))

    # Apply k-means clustering to find k dominant colors
    kmeans = KMeans(n_clusters=k)
    kmeans.fit(pixels)

    # Replace each pixel with its nearest centroid
    quantized_pixels = kmeans.predict(pixels)
    centroids = kmeans.cluster_centers_

    # Reshape the quantized image
    quantized_image = centroids[quantized_pixels].reshape(image.shape)

    return quantized_image.astype(np.uint8)


def load_images(directory_path):
    eyes = []
    for filename in os.listdir(directory_path):
        file_path = os.path.join(directory_path, filename)
        image = cv2.imread(file_path)
        eyes.append(image)
    return eyes


def cut_and_save_around_centers(eye_images, output_dir):
    size = 80
    for i, eye in enumerate(eye_images):
        center_x = int(eye.shape[1] / 2)
        center_y = int(eye.shape[0] / 2)
        eye_image = eye_images[i]

        # Calculate the bounding box around the center
        top_left_x = int(max(0, center_x - size / 2))
        top_left_y = int(max(0, center_y - size / 2))
        bottom_right_x = int(min(eye_image.shape[1], center_x + size / 2))
        bottom_right_y = int(min(eye_image.shape[0], center_y + size / 2))

        # Crop the region around the center
        eye_patch = eye_image[top_left_y:bottom_right_y, top_left_x:bottom_right_x]

        # Save the cropped region
        output_path = os.path.join(output_dir, f"eye_patch_{i}.png")
        cv2.imwrite(output_path, eye_patch)


def avg_image(eye_images):
    avrages = []
    for img in eye_images:
        # Convert the image to float32 for accurate calculations
        image_float = img.astype(np.float32)

        # Calculate the ABF for each channel
        abf_red = np.mean(image_float[:, :, 2])  # Red channel
        abf_green = np.mean(image_float[:, :, 1])  # Green channel
        abf_blue = np.mean(image_float[:, :, 0])  # Blue channel

        # Overall ABF is the average of ABF for each channel
        avrages.append([abf_red, abf_green, abf_blue])
    return avrages


def quantize_colors(color_tuples, num_clusters):
    # Convert the list of tuples to a NumPy array
    colors = np.array(color_tuples)

    # Reshape the array to have three columns (RGB values)
    colors_reshaped = colors.reshape(-1, 3)

    # Apply k-means clustering
    kmeans = KMeans(n_clusters=num_clusters)
    kmeans.fit(colors_reshaped)

    # Get the centroids of the clusters
    centroids = kmeans.cluster_centers_
    print("centroids: ", centroids)
    # Quantize the colors by replacing each original color with its nearest centroid
    quantized_colors = centroids[kmeans.labels_].astype(int)

    # Reshape the quantized colors array to match the original shape
    quantized_colors_reshaped = quantized_colors.reshape(colors.shape)

    return centroids, quantized_colors_reshaped


if __name__ == '__main__':
    # Specify the output directory
    input_directory = "./public_faces_eyes_cut"

    eye_images = load_images(input_directory)

    color_tuples = avg_image(eye_images)

    # Number of clusters (desired number of quantized colors)
    num_clusters = 3

    # Perform color quantization
    centroids, quantized_colors = quantize_colors(color_tuples, num_clusters)

    # Visualize the original and quantized colors on a color map
    fig, axes = plt.subplots(1, 2, figsize=(8, 4))

    # Original colors
    axes[0].scatter(*zip(*color_tuples), color=np.array(color_tuples) / 255)
    axes[0].set_title('Original Colors')

    # Quantized colors
    axes[1].scatter(*zip(*quantized_colors), color=np.array(quantized_colors) / 255)
    axes[1].set_title('Quantized Colors')

    plt.show()

    counter = [0, 0, 0]
    for [red, green, blue] in color_tuples:
        distance_1 = math.sqrt((red - color_tuples[0][0]) ** 2 + (red - color_tuples[0][1]) ** 2 + (red - color_tuples[
            0][2]) ** 2)
        distance_2 = math.sqrt((red - color_tuples[1][0]) ** 2 + (red - color_tuples[1][1]) ** 2 + (red - color_tuples[
            1][2]) ** 2)
        distance_3 = math.sqrt((red - color_tuples[2][0]) ** 2 + (red - color_tuples[3][1]) ** 2 + (red - color_tuples[
            3][2]) ** 2)
        if distance_1 <= distance_2 and distance_1 <= distance_3:
            counter[0] += 1
        elif distance_2 < distance_3 and distance_2 <= distance_1:
            counter[1] += 1
        else:
            counter[2] += 1

    print(counter)
