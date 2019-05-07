import sys
import cv2
import numpy as np

def naming(path):
    # Pre: the path to the file (an image)
    # Post: the file name of the preprocessed file (an image)
    file_name = ""
    path_len = len(path)
    for i in reversed(range(len(path))):
        if path[i] == '.':
            name = path[:i]
            file_name = name + "_processed.png"
            break
            
    return file_name     

for i in range(1, len(sys.argv)):
    src = cv2.imread(sys.argv[i], 0)
    src = cv2.resize(src, (512, 512))
    blur = cv2.blur(src, (5, 5))
    equl = cv2.equalizeHist(blur)

    image_name = naming(sys.argv[i])
    # print("image_name =", image_name)
    # image_name = sys.argv[i]
    # image_name = "../bmp/" + image_name
    cv2.imwrite(image_name, equl)


