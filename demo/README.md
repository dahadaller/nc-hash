1. Use **PreProc** to pre-process the image
2. Feed the processed image to **scheme2WithBytestringImportExport.cpp** to compute the hash
3. Use **HamDist.py** to compute the Hamming distance between the orginal image and the Modified version of the image

# Image Manipulations
1. Cropping
2. Rotation
3. Horizontal Flipping
4. Vertical Flipping
5. Invert pixel values
6. JPEG Compression (Mac's default rate)

| Manipulations       | Hash (before) | Hash (after) | Distance |
|:-------------------:|:-------------:|:------------:|:--------:|
| Cropping            | 21194984      | 16213941     | 0.56     |
| Left Rotate         | 22267473      | 21941111     | 0.40     |
| Right Rotate        | 32023788      | 32291200     | 0.36     |
| Horizontal Flip     | 28224592      | 27330120     | 0.40     |
| Vertical Flip       | 24303572      | 24704224     | 0.32     |
| Invert pixel values | 16854459      | 17267339     | 0.36     |
| JPEG Compress       | 32846954      | 29239466     | 0.36     |

