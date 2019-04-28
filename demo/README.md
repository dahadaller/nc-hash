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
7. Rotate by specified degrees
8. Percentage change of each pixel values

# Crop image by specified degree
Cropping the edge of the image then resize it to 512x512

| Percentage Cropping | Hash (before) | Hash (after) | Distance |
|:-------------------:|:-------------:|:------------:|:--------:|
| 5                   | 41309867      | 38608655     | 0.42     |
| 10                  | 40421293      | 34761464     | 0.54     |
| 15                  | 26635922      | 24627382     | 0.40     |
| 20                  | 35207526      | 31257052     | 0.53     |
| 25                  | 27412114      | 26057210     | 0.60     |

# Rotation

| Left Rotation | Hash (before) | Hash (after) | Distance |
|:-------------:|:-------------:|:------------:|:--------:|
| 01 degree     | 20973793      | 20015715     | 0.36     |
| 02 degree     | 20096345      | 19740706     | 0.64     |
| 03 degree     | 36881114      | 35020642     | 0.42     |
| 04 degree     | 67375082      | 58691517     | 0.56     |
| 05 degree     | 33704654      | 29683255     | 0.58     |
| 06 degree     | 28808088      | 27990235     | 0.40     |
| 07 degree     | 36719945      | 35473650     | 0.46     |
| 08 degree     | 30514396      | 29873771     | 0.48     |
| 09 degree     | 30163531      | 31010794     | 0.44     |
| 10 degree     | 34438147      | 34906570     | 0.50     |
| 20 degree     | 41281249      | 42932136     | 0.54     |
| 30 degree     | 29785108      | 30401221     | 0.44     |
| 40 degree     | 30536468      | 29818905     | 0.48     |

# Other manipulations

| Other manipulations   | Hash (before) | Hash (after) | Distance |
|:---------------------:|:-------------:|:------------:|:--------:|
| Left Rotate (90)      | 22267473      | 21941111     | 0.40     |
| Right Rotate (90)     | 32023788      | 32291200     | 0.36     |
| Horizontal Flip       | 28224592      | 27330120     | 0.40     |
| Vertical Flip         | 24303572      | 24704224     | 0.32     |
| Invert pixel values   | 16854459      | 17267339     | 0.36     |
| JPEG Compress         | 32846954      | 29239466     | 0.36     |
| 25% * pixel values    | 22860828      | 48778480     | 0.54     |
| 50% * pixel values    | 15877022      | 20929632     | 0.72     |
| 75% * pixel values    | 82890379      | 84215328     | 0.56     |
| Change 1 pixel values | 17807293      | 17806568     | 0.24     |
| Change 10% of the img | 47481714      | 47844033     | 0.42     |
