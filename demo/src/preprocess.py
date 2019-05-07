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

def cropping(img, long, short):
    a = int(long/2 - short/2)
    print("a =", a)
    b = int(long/2 + short/2)
    print("b =", b)
    if (img.shape[0] == long):
        return img[a:b, 0:short]
        # img = img[a:b, 0:short]
        # img = img[0:short, a:b]
    elif (img.shape[1] == long):
        return img[0:short, a:b]
        # img = img[0:short, a:b]
        # img = img[a:b, 0:short]

for i in range(1, len(sys.argv)):
    src = cv2.imread(sys.argv[i], 0)
    print(src.shape)
    longer_side = max(src.shape[0], src.shape[1])
    shorter_side = min(src.shape[0], src.shape[1])
    if (not (longer_side == shorter_side == 512)):
        print("Yes")
        img = cropping(src, longer_side, shorter_side)
        print(img.shape)
        img = cv2.resize(img, (512, 512))
        blur = cv2.blur(img, (5, 5))
        equl = cv2.equalizeHist(blur)
        image_name = naming(sys.argv[i])
        cv2.imwrite(image_name, equl)
    
    else:
        blur = cv2.blur(src, (5, 5))
        equl = cv2.equalizeHist(blur)
        image_name = naming(sys.argv[i])
        # print("image_name =", image_name)
        # image_name = sys.argv[i]
        # image_name = "../bmp/" + image_name
        cv2.imwrite(image_name, equl)
