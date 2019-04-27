import cv2
import imutils
import numpy as np
from matplotlib import pyplot as plt

def rotate_bound(image, angle):
    # grab the dimensions of the image and then determine the
    # center
    (h, w) = image.shape[:2]
    (cX, cY) = (w // 2, h // 2)
 
    # grab the rotation matrix (applying the negative of the
    # angle to rotate clockwise), then grab the sine and cosine
    # (i.e., the rotation components of the matrix)
    M = cv2.getRotationMatrix2D((cX, cY), -angle, 1.0)
    cos = np.abs(M[0, 0])
    sin = np.abs(M[0, 1])
 
    # compute the new bounding dimensions of the image
    nW = int((h * sin) + (w * cos))
    nH = int((h * cos) + (w * sin))
 
    # adjust the rotation matrix to take into account translation
    M[0, 2] += (nW / 2) - cX
    M[1, 2] += (nH / 2) - cY
 
    # perform the actual rotation and return the image
    return cv2.warpAffine(image, M, (nW, nH))


# Cropping
# img = cv2.imread('../images/Lenna.png', 0)
# cropped = img[127:384, 127:384]
# cropped = cv2.resize(cropped, (512, 512))
# blur = cv2.blur(cropped, (5, 5))
# equl = cv2.equalizeHist(blur)

# Rotate
# img = cv2.imread('../images/RightRotateLenna.png', 0)
# blur = cv2.blur(img, (5, 5))
# equl = cv2.equalizeHist(blur)

# Flipping
# src = cv2.imread('../images/Lenna.png', 0)
# # print(src.dtype)
# img = np.zeros((512,512), dtype=src.dtype)
# for i in range(512):
#     for j in range(512):
#         img[i][j] = src[511-i][j]

# blur = cv2.blur(img, (5, 5))
# equl = cv2.equalizeHist(blur)


# # cv2.imshow("img", img)
# # cv2.waitKey(0)

# cv2.imwrite('../images/VFLenna.png', equl)


# Invert pixel values
# src = cv2.imread('../images/Lenna.png', 0)
# # print(src.dtype)
# img = np.zeros((512,512), dtype=src.dtype)
# for i in range(512):
#     for j in range(512):
#         img[i][j] = 255 - src[i][j]

# blur = cv2.blur(img, (5, 5))
# equl = cv2.equalizeHist(blur)


# # cv2.imshow("img", img)
# # cv2.waitKey(0)

# cv2.imwrite('../images/IVLenna.png', equl)

# JPEG Compression
# img = cv2.imread('../images/Lenna.jpg', 0)

# blur = cv2.blur(img, (5, 5))
# equl = cv2.equalizeHist(blur)


# # cv2.imshow("img", src)
# # cv2.waitKey(0)

# cv2.imwrite('../images/JPCLenna.jpg', equl)

# Rotate by degree
# src = cv2.imread('../images/Lenna.png', 0)
# # img = rotate_bound(src, 10)
# # cv2.imshow("img", img)
# # cv2.waitKey(0)

# rotated05 = imutils.rotate(src, 5)
# blur = cv2.blur(rotated05, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/rotate05.png', equl)

# rotated10 = imutils.rotate(src, 10)
# blur = cv2.blur(rotated10, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/rotate10.png', equl)

# rotated20 = imutils.rotate(src, 20)
# blur = cv2.blur(rotated20, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/rotate20.png', equl)

# rotated30 = imutils.rotate(src, 30)
# blur = cv2.blur(rotated30, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/rotate30.png', equl)

# rotated40 = imutils.rotate(src, 40)
# blur = cv2.blur(rotated40, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/rotate40.png', equl)
# # cv2.imshow("Rotated (Problematic)", rotated)
# # cv2.waitKey(0)

# % change of Pixel Values
src = cv2.imread('../images/Lenna.png', 0)
# print(src[10][10])
img = np.zeros((512,512), dtype=src.dtype)
# img[10][10] = 0.25 * src[10][10]
# print(img[10][10])
for i in range(512):
    for j in range(512):
        img[i][j] = 1 * src[i][j]

blur = cv2.blur(img, (5, 5))
equl = cv2.equalizeHist(blur)
cv2.imwrite('../images/Lenna100.png', equl)