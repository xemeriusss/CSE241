import numpy as np
import cv2

# Load the image
image = cv2.imread("istanbul.jpg")
image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

# Get the dimensions of the image
height, width, channels = image.shape
center = (width // 2, height // 2)

# Translation paremeters
scale_factor = 1.4
rotation_angle = 60
shear_factor = 1.4
zoom_factor = 1.4

# Scale
scale_matrix = np.array([[scale_factor, 0, 0],
                          [0, scale_factor, 0],
                          [0, 0, 1]])

# Rotation
rotation_matrix = np.array([[np.cos(np.radians(rotation_angle)), -np.sin(np.radians(rotation_angle)), 0],
                             [np.sin(np.radians(rotation_angle)), np.cos(np.radians(rotation_angle)), 0],
                             [0, 0, 1]])

# Horizontal shear
shear_matrix = np.array([[1, shear_factor, 0],
                         [0, 1, 0],
                         [0, 0, 1]])

# Zoom
zoom_matrix = np.array([[zoom_factor, 0, 0],
                        [0, zoom_factor, 0],
                        [0, 0, 1]])


def matrix_multiply(A, B):

    rows_A = len(A)
    cols_A = len(A[0])
    rows_B = len(B)
    cols_B = len(B[0])
    
    if cols_A != rows_B:
        raise ValueError("not compatible matrices")

    # Create the result matrix filled with zeros
    result = [[0 for _ in range(cols_B)] for _ in range(rows_A)]

    # Matrix multiplication with three nested loops
    for i in range(rows_A):
        for j in range(cols_B):
            for k in range(cols_A):  
                result[i][j] += A[i][k] * B[k][j]

    return result

# Combine all transformations
transformation_matrix = matrix_multiply(scale_matrix, rotation_matrix)
transformation_matrix = matrix_multiply(transformation_matrix, shear_matrix)

# Calculate inverse of a matrix
def inverse_matrix(matrix):

    if len(matrix) == 3 and len(matrix[0]) == 3:
        a, b, c = matrix[0]
        d, e, f = matrix[1]
        g, h, i = matrix[2]

        # Determinant for 3x3 matrix
        determinant = a * e * i + b * f * g + c * d * h - c * e * g - b * d * i - a * f * h

        inverse_matrix = [[(e * i - f * h) / determinant, (c * h - b * i) / determinant, (b * f - c * e) / determinant],
                          [(f * g - d * i) / determinant, (a * i - c * g) / determinant, (c * d - a * f) / determinant],
                          [(d * h - e * g) / determinant, (g * b - a * h) / determinant, (a * e - b * d) / determinant]]
        return inverse_matrix

    else:
        raise ValueError("Matrix is not 3x3")


# Forward mapping with transformation matrix
def forward_mapping(image, transform_matrix):
    height, width, _ = image.shape
    transformed_image = np.zeros_like(image)

    # Calculate the center of the image
    center_x, center_y = width / 2, height / 2

    for y in range(height):
        for x in range(width):
            # Translate coordinates to center
            original_position = np.array([x - center_x, y - center_y, 1])
            # Find the new position 
            new_position = transform_matrix @ original_position
            # Find the new coordinates
            new_x, new_y = int(new_position[0] + center_x), int(new_position[1] + center_y)

            # If the new coordinates are within the bounds of the transformed image,
            # assign the pixel value of the original image to the new coordinates
            if 0 <= new_x < width and 0 <= new_y < height:
                transformed_image[new_y, new_x] = image[y, x]

    return transformed_image


# Backward mapping without interpolation with transformation matrix
def backward_mapping_no_interpolation(image, transform_matrix):
    height, width, _ = image.shape
    transformed_image = np.zeros_like(image)

    # Calculate the center of the image
    center_x, center_y = width / 2, height / 2

    # Compute the inverse transformation matrix
    inverse_transform_matrix = inverse_matrix(transform_matrix)

    for y in range(height):
        for x in range(width):
            # Translate coordinates to center
            original_position = np.array([x - center_x, y - center_y, 1])
            # Find the new position with the inverse transformation
            new_position = inverse_transform_matrix @ original_position
            # Find the new coordinates
            new_x, new_y = int(new_position[0] + center_x), int(new_position[1] + center_y)

            # If the new coordinates are within the bounds of the original image,
            # assign the pixel value of the transformed image to the new coordinates
            if 0 <= new_x < width and 0 <= new_y < height:
                transformed_image[y, x] = image[new_y, new_x]

    return transformed_image

# Bilinear interpolation helper function
def bilinear_interpolation(image, x, y):

    # If the coordinates are out of bounds, return black 
    if x < 0 or y < 0 or x >= image.shape[1] - 1 or y >= image.shape[0] - 1:
        return [0, 0, 0]

    x0 = int(x)
    x1 = min(x0 + 1, image.shape[1] - 1) # Prevent out of bounds error
    y0 = int(y)
    y1 = min(y0 + 1, image.shape[0] - 1)

    a = x - x0 
    b = y - y0

    # Find the pixel values of the four corners
    top_left = image[y0, x0]
    top_right = image[y0, x1]
    bottom_left = image[y1, x0]
    bottom_right = image[y1, x1]

    # Apply bilinear interpolation formula
    pixel_value = (1 - a) * (1 - b) * top_left + \
                  a * (1 - b) * top_right + \
                  (1 - a) * b * bottom_left + \
                  a * b * bottom_right

    return pixel_value.astype(np.uint8)

# Backward mapping with interpolation with transformation matrix
def backward_mapping_with_interpolation(image, transform_matrix):
    height, width, _ = image.shape
    transformed_image = np.zeros_like(image)

    # Calculate the center of the image
    center_x, center_y = width / 2, height / 2

    # Compute the inverse transformation matrix
    inverse_transform_matrix = inverse_matrix(transform_matrix)

    for y in range(height):
        for x in range(width):
            # Translate coordinates to center
            original_position = np.array([x - center_x, y - center_y, 1])
            # Find the new position with the inverse transformation
            new_position = inverse_transform_matrix @ original_position
            # Find the new coordinates
            new_x, new_y = new_position[0] + center_x, new_position[1] + center_y

            # Apply bilinear interpolation
            pixel_value = bilinear_interpolation(image, new_x, new_y)

            # Assign the pixel value to the transformed image
            transformed_image[y, x] = pixel_value

    return transformed_image


# Save functions with opencv
def save_forward_mapping_images(image, scale_matrix, rotation_matrix, shear_matrix, zoom_matrix):
    forward_scaled_image = forward_mapping(image, scale_matrix)
    cv2.imwrite("forward_scaled_image.jpg", cv2.cvtColor(forward_scaled_image, cv2.COLOR_RGB2BGR))

    forward_rotated_image = forward_mapping(image, rotation_matrix)
    cv2.imwrite("forward_rotated_image.jpg", cv2.cvtColor(forward_rotated_image, cv2.COLOR_RGB2BGR))

    forward_sheared_image = forward_mapping(image, shear_matrix)
    cv2.imwrite("forward_sheared_image.jpg", cv2.cvtColor(forward_sheared_image, cv2.COLOR_RGB2BGR))

    forward_zoomed_image = forward_mapping(image, zoom_matrix)
    cv2.imwrite("forward_zoomed_image.jpg", cv2.cvtColor(forward_zoomed_image, cv2.COLOR_RGB2BGR))


def save_backward_mapping_images(image, scale_matrix, rotation_matrix, shear_matrix, zoom_matrix):
    backward_scaled_image = backward_mapping_no_interpolation(image, scale_matrix)
    cv2.imwrite("backward_scaled_image.jpg", cv2.cvtColor(backward_scaled_image, cv2.COLOR_RGB2BGR))

    backward_rotated_image = backward_mapping_no_interpolation(image, rotation_matrix)
    cv2.imwrite("backward_rotated_image.jpg", cv2.cvtColor(backward_rotated_image, cv2.COLOR_RGB2BGR))

    backward_sheared_image = backward_mapping_no_interpolation(image, shear_matrix)
    cv2.imwrite("backward_sheared_image.jpg", cv2.cvtColor(backward_sheared_image, cv2.COLOR_RGB2BGR))

    backward_zoomed_image = backward_mapping_no_interpolation(image, zoom_matrix)
    cv2.imwrite("backward_zoomed_image.jpg", cv2.cvtColor(backward_zoomed_image, cv2.COLOR_RGB2BGR))


def save_backward_with_interpolation_images(image, scale_matrix, rotation_matrix, shear_matrix, zoom_matrix):
    backward_interpolation_scaled_image = backward_mapping_with_interpolation(image, scale_matrix)
    cv2.imwrite("backward_interpolation_scaled_image.jpg", cv2.cvtColor(backward_interpolation_scaled_image, cv2.COLOR_RGB2BGR))

    backward_interpolation_rotated_image = backward_mapping_with_interpolation(image, rotation_matrix)
    cv2.imwrite("backward_interpolation_rotated_image.jpg", cv2.cvtColor(backward_interpolation_rotated_image, cv2.COLOR_RGB2BGR))

    backward_interpolation_sheared_image = backward_mapping_with_interpolation(image, shear_matrix)
    cv2.imwrite("backward_interpolation_sheared_image.jpg", cv2.cvtColor(backward_interpolation_sheared_image, cv2.COLOR_RGB2BGR))

    backward_interpolation_zoomed_image = backward_mapping_with_interpolation(image, zoom_matrix)
    cv2.imwrite("backward_interpolation_zoomed_image.jpg", cv2.cvtColor(backward_interpolation_zoomed_image, cv2.COLOR_RGB2BGR))


def save_forward_with_transformation_matrix(image, transformation_matrix):
    forward_transformed_image = forward_mapping(image, transformation_matrix)
    cv2.imwrite("forward_transformed_image.jpg", cv2.cvtColor(forward_transformed_image, cv2.COLOR_RGB2BGR))

def save_backward_with_transformation_matrix(image, transformation_matrix):
    backward_transformed_image = backward_mapping_no_interpolation(image, transformation_matrix)
    cv2.imwrite("backward_transformed_image.jpg", cv2.cvtColor(backward_transformed_image, cv2.COLOR_RGB2BGR))

def save_backward_with_interpolation_transformation_matrix(image, transformation_matrix):
    backward_interpolation_transformed_image = backward_mapping_with_interpolation(image, transformation_matrix)
    cv2.imwrite("backward_interpolation_transformed_image.jpg", cv2.cvtColor(backward_interpolation_transformed_image, cv2.COLOR_RGB2BGR))
    

# Main function
if __name__ == "__main__":
    save_forward_mapping_images(image, scale_matrix, rotation_matrix, shear_matrix, zoom_matrix)
    save_backward_mapping_images(image, scale_matrix, rotation_matrix, shear_matrix, zoom_matrix)
    save_backward_with_interpolation_images(image, scale_matrix, rotation_matrix, shear_matrix, zoom_matrix)

    save_forward_with_transformation_matrix(image, transformation_matrix)
    save_backward_with_transformation_matrix(image, transformation_matrix)
    save_backward_with_interpolation_transformation_matrix(image, transformation_matrix)

    # # Print scale matrix
    # print("Scale matrix:")
    # for row in scale_matrix:
    #     print(row)

    # # Print rotation matrix
    # print("Rotation matrix:")
    # for row in rotation_matrix:
    #     print(row)

    # # Print shear matrix
    # print("Shear matrix:")
    # for row in shear_matrix:
    #     print(row)

    # # Print zoom matrix
    # print("Zoom matrix:")
    # for row in zoom_matrix:
    #     print(row)

    # # Print the transformation matrix
    # print("Transformation matrix:")

    # for row in transformation_matrix:
    #     print(row)


    


