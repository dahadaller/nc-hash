import cv2

camera = cv2.VideoCapture(0)
cv2.namedWindow("Camera")
img_counter = 0

while True:
    ret, frame = cam.read()
    cv2.imshow("test", frame)
    if not ret:
        break
    k = cv2.waitKey(1)

    if k%256 == 27:
        # ESC pressed
        print("Escape hit, closing...")
        break
    elif k%256 == 32:
        # SPACE pressed
        img_name = "/Users/bon/Documents/CCNY/public-phash/demo/images/test_image_{}.png".format(img_counter)
        cv2.imwrite(img_name, frame)
        print("{} written!".format(img_name))
        img_counter += 1

camera.release()

# Closing window for Mac
cv2.destroyAllWindows()
for _ in range(4):
    cv2.waitKey(1)
