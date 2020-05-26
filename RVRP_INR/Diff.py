# import the necessary packages
from skimage.measure import compare_ssim
import argparse
import imutils
import cv2
import numpy as np

# construct the argument parse and parse the arguments
##ap = argparse.ArgumentParser()
##ap.add_argument("-f", "--first", required=True, help="first input image")
##ap.add_argument("-s", "--second", required=True, help="second")
##args = vars(ap.parse_args())

# load the two input images
##imageA = cv2.imread(args["first"])
##imageB = cv2.imread(args["second"])

imageA = cv2.imread("7570F505E147AQ060021.jpg")
imageB = cv2.imread("7570F505E147AQ060022.jpg")

# convert the images to grayscale
grayA = cv2.cvtColor(imageA, cv2.COLOR_BGR2GRAY)
grayB = cv2.cvtColor(imageB, cv2.COLOR_BGR2GRAY)

# compute the Structural Similarity Index (SSIM) between the two
# images, ensuring that the difference image is returned
(score, diff) = compare_ssim(grayA, grayB, full=True)
diff = (diff * 255).astype("uint8")
#print(diff)
#print("SSIM: {}".format(score))

# threshold the difference image, followed by finding contours to
# obtain the regions of the two input images that differ
thresh = cv2.threshold(diff, 0, 255, cv2.THRESH_BINARY_INV | cv2.THRESH_OTSU)[1]

kernel = np.ones((5,5),np.uint8)
#thresh = cv2.dilate(thresh,kernel,iterations = 1)
#opening = cv2.morphologyEx(img, cv2.MORPH_OPEN, kernel)
thresh = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)
#closing = cv2.morphologyEx(img, cv2.MORPH_CLOSE, kernel)
#thresh = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel)

#thresh = cv2.erode(thresh,kernel,iterations = 1)

cnts = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
cnts = imutils.grab_contours(cnts)
#print(cnts)

min_x = 0
min_y = 0
max_x = 0
max_y = 0
i = 1
# loop over the contours
for c in cnts:
    # compute the bounding box of the contour and then draw the
    # bounding box on both input images to represent where the two
    # images differ
    (x, y, w, h) = cv2.boundingRect(c)
    
    if(42<=y<=402):
        if (i==1):
            min_x = x
            min_y = y
            max_x = x + w
            max_y = y + h
            i=0
        if(x < min_x): min_x = x
        if(y < min_y): min_y = y
        if(x + w > max_x): max_x = x + w
        if(y + h > max_y): max_y = y + h
        #cv2.rectangle(imageA, (x, y), (x + w, y + h), (0, 0, 255), 2)
        cv2.rectangle(imageB, (x, y), (x + w, y + h), (0, 0, 255), 2)
cv2.rectangle(imageB, (min_x, min_y), (max_x, max_y), (0, 0, 255), 2)

text = "size: (x, y) = (" + str(max_x - min_x) + ", " + str(max_y - min_y) + ")"
cv2.putText(imageB, text, (min_x, min_y - 10), cv2.FONT_HERSHEY_COMPLEX, 0.8, (0, 0, 255), 2, cv2.LINE_AA)
#print("size: x = ",(max_x - min_x),"y = ",(max_y - min_y))
    

# show the output images
cv2.imshow("Original", imageA)
cv2.imshow("Modified", imageB)
cv2.imshow("Diff", diff)
cv2.imshow("Thresh", thresh)
cv2.waitKey(0)
