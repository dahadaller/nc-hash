import sys
import cv2
import numpy as np

def naming(path):
    # Pre: the path to the file (an image)
    # Post: the file name of the preprocessed file (an image)
    file_name = ""
    path_len = len(path)
    for k in reversed(range(path_len)):
        if path[k] == '/':
            k += 1
            # path = path[k:]
            file_name = path[:k] + "processed_" + path[k:]
            print("path =", path)
            print("file_name =", file_name)
            break

    # for i in reversed(range(len(path))):
    #     if path[i] == '.':
    #         name = path[:i]
    #         format = ".png"
    #         file_name = "processed_" + name + format
    #         break
            
    return file_name     

for i in range(1, len(sys.argv)):
    src = cv2.imread(sys.argv[i], 0)
    blur = cv2.blur(src, (5, 5))
    img = cv2.resize(blur, (512, 512))
    equl = cv2.equalizeHist(blur)

    # image_name = naming(sys.argv[i])
    image_name = sys.argv[i]
    # image_name = "../bmp/" + image_name
    cv2.imwrite(image_name, equl)


