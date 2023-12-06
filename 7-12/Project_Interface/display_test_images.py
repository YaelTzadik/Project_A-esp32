import os
import tkinter as tk
from PIL import Image, ImageTk
import cv2

addr = "./TestCWSExample"



def process_files(directory_path):
    # Dictionary to keep track of the count of each number
    number_count = {}

    for filename in os.listdir(directory_path):
        file_path = os.path.join(directory_path, filename)

        # Check if it's a file
        if os.path.isfile(file_path):
            # Split the filename by "_"
            name_parts = filename.split("_")

            # Check if there are at least 4 parts and if the 4th part is neither "h" nor "n"
            if len(name_parts) >= 4 and name_parts[3] not in ["h", "n"]:
                # Extract the number from the filename
                number = name_parts[0]

                # Update the count for this number
                number_count[number] = number_count.get(number, 0) + 1

    # Iterate through the files again to delete those with counts not equal to 4
    for filename in os.listdir(directory_path):
        file_path = os.path.join(directory_path, filename)

        # Check if it's a file
        if os.path.isfile(file_path):
            # Split the filename by "_"
            name_parts = filename.split("_")

            # Check if there are at least 4 parts and if the 4th part is neither "h" nor "n"
            if len(name_parts) >= 4 and name_parts[3] not in ["h", "n"]:
                # Extract the number from the filename
                number = name_parts[0]

                # Check if the count for this number is not equal to 4
                if number_count.get(number, 0) != 4:
                    # Delete the file
                    os.remove(file_path)
                    print(f"Deleted: {filename}")
                else:
                    print(f"Keeping: {filename}")
            else:
                print(f"Skipped: {filename}")


def load_images_to_matrix(directory_path):
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

def load_n_images_to_matrix(directory_path):
    # Dictionary to store image names grouped by number
    images_by_number = {}

    for filename in os.listdir(directory_path):
        file_path = os.path.join(directory_path, filename)

        # Check if it's a file
        if os.path.isfile(file_path):
            # Split the filename by "_"
            name_parts = filename.split("_")

            # Check if there are at least 4 parts and if the 4th part is neither "h" nor "n"
            if len(name_parts) >= 2 and name_parts[3] in ["h", "n"]:
                # Extract the number from the filename
                number = name_parts[0]

                # Create a key for this number if it doesn't exist
                if number not in images_by_number:
                    images_by_number[number] = []

                # Add the filename to the list for this number
                images_by_number[number].append(filename)

    # Create a matrix with rows containing 4 images with the same number
    matrix = []
    for number, filenames in sorted(images_by_number.items(), key=lambda x: (int(x[0]), x[0][1] != 'n')):
        matrix.extend([[int(number)] + filenames[i:i + 2] for i in range(0, len(filenames), 2)])

    return matrix


class ImageViewer:
    def __init__(self, image_matrix):
        self.image_matrix = image_matrix
        self.current_row = 0
        self.current_image = 0

        self.root = tk.Tk()
        self.root.title("Image Viewer")

        self.prev_button = tk.Button(self.root, text="Prev", command=self.prev_image)
        self.prev_button.pack(side=tk.LEFT)

        self.next_button = tk.Button(self.root, text="Next", command=self.next_image)
        self.next_button.pack(side=tk.LEFT)

        self.image_label = tk.Label(self.root)
        self.image_label.pack()

        self.update_image()
        self.root.mainloop()


    def update_image(self):
        image_name = self.image_matrix[self.current_row][self.current_image + 1]
        image_path = os.path.join(addr, image_name)  # Set the path to ./public_faces
        original_image = Image.open(image_path)

        # Resize the image to fit within a 400x400 window
        resized_image = original_image.resize((500, 500), Image.ANTIALIAS)

        tk_image = ImageTk.PhotoImage(resized_image)
        self.image_label.config(image=tk_image)
        self.image_label.image = tk_image  # Keep a reference to the image

        self.root.title(f"Image Viewer - {image_name} subject-{self.current_row}")

    def next_image(self):
        leni = len(self.image_matrix[0])-1
        self.current_image = (self.current_image + 1) % leni # Ignoring the first column
        if self.current_image == 0:
            self.current_row = (self.current_row + 1) % len(self.image_matrix)
        self.update_image()

    def prev_image(self):
        leni = len(self.image_matrix[0]) - 1
        self.current_image = (self.current_image - 1) % leni  # Ignoring the first column
        if self.current_image == leni/2:
            self.current_row = (self.current_row - 1) % len(self.image_matrix)
        self.update_image()

if __name__ == '__main__':
    # process_files("./TestCWSExample")
    # image_matrix = load_images_to_matrix("./TestCWSExample")
    image_matrix = load_n_images_to_matrix(addr)
    # Create and run the image viewer
    ImageViewer(image_matrix)
