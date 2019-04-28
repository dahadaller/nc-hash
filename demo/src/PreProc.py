import cv2
import imutils
import numpy as np
from random import randint
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
# # 5%
# cropped = img[6:506, 6:506]
# cropped = cv2.resize(cropped, (512, 512))
# blur = cv2.blur(cropped, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/cropped5.png', equl)

# # 10%
# cropped = img[13:499, 13:499]
# cropped = cv2.resize(cropped, (512, 512))
# blur = cv2.blur(cropped, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/cropped10.png', equl)

# # 15%
# cropped = img[20:492, 20:492]
# cropped = cv2.resize(cropped, (512, 512))
# blur = cv2.blur(cropped, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/cropped15.png', equl)

# # 20%
# cropped = img[27:485, 27:485]
# cropped = cv2.resize(cropped, (512, 512))
# blur = cv2.blur(cropped, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/cropped20.png', equl)

# # 25%
# cropped = img[34:478, 34:478]
# cropped = cv2.resize(cropped, (512, 512))
# blur = cv2.blur(cropped, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/cropped25.png', equl)


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

# rotated1 = imutils.rotate(src, 1)
# blur = cv2.blur(rotated1, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/rotate1.png', equl)

# rotated2 = imutils.rotate(src, 2)
# blur = cv2.blur(rotated2, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/rotate2.png', equl)

# rotated3 = imutils.rotate(src, 3)
# blur = cv2.blur(rotated3, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/rotate3.png', equl)

# rotated4 = imutils.rotate(src, 4)
# blur = cv2.blur(rotated4, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/rotate4.png', equl)

# rotated6 = imutils.rotate(src, 6)
# blur = cv2.blur(rotated6, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/rotate6.png', equl)

# rotated7 = imutils.rotate(src, 7)
# blur = cv2.blur(rotated7, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/rotate7.png', equl)

# rotated8 = imutils.rotate(src, 8)
# blur = cv2.blur(rotated8, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/rotate8.png', equl)

# rotated9 = imutils.rotate(src, 9)
# blur = cv2.blur(rotated9, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/rotate9.png', equl)

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
# src = cv2.imread('../images/Lenna.png', 0)
# # print(src[10][10])
# img = np.zeros((512,512), dtype=src.dtype)
# # img[10][10] = 0.25 * src[10][10]
# # print(img[10][10])
# for i in range(512):
#     for j in range(512):
#         img[i][j] = 1 * src[i][j]

# blur = cv2.blur(img, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/Lenna100.png', equl)

# Replace 1 randomly picked pixel with a random pixel value
# img = cv2.imread('../images/Lenna.png', 0)
# # print(src[10][10])
# row = randint(0, 511)
# col = randint(0, 511)
# val = randint(0, 255)
# img[row][col] = val
# blur = cv2.blur(img, (5, 5))
# equl = cv2.equalizeHist(blur)
# cv2.imwrite('../images/change_one_pixel.png', equl)

# Replace 10% * 512 * 512 randomly picked pixels with a random pixel value
img = cv2.imread('../images/Lenna.png', 0)
# print(src[10][10])
for _ in range(163):
    row = randint(0, 511)
    col = randint(0, 511)
    val = randint(0, 255)
    img[row][col] = val

blur = cv2.blur(img, (5, 5))
equl = cv2.equalizeHist(blur)
cv2.imwrite('../images/change_pixels.png', equl)