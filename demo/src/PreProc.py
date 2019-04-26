import cv2
import numpy as np
from matplotlib import pyplot as plt

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
img = cv2.imread('../images/Lenna.jpg', 0)
# print(src.dtype)
# img = np.zeros((512,512), dtype=src.dtype)
# for i in range(512):
#     for j in range(512):
#         img[i][j] = 255 - src[i][j]

blur = cv2.blur(img, (5, 5))
equl = cv2.equalizeHist(blur)


# cv2.imshow("img", src)
# cv2.waitKey(0)

cv2.imwrite('../images/JPCLenna.jpg', equl)
