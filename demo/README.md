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

| Percentage Cropping | Hash (before) | Hash (after) | Distance | Relative Change |
|:-------------------:|:-------------:|:------------:|:--------:|:---------------:|
| 5                   | 41309867      | 38608655     | 0.42     | 0.065           |
| 10                  | 40421293      | 34761464     | 0.54     | 0.140           |
| 15                  | 26635922      | 24627382     | 0.40     | 0.075           |
| 20                  | 35207526      | 31257052     | 0.53     | 0.112           |
| 25                  | 27412114      | 26057210     | 0.60     | 0.049           |

# Rotation

| Left Rotation | Hash (before) | Hash (after) | Distance | Relative Change |
|:-------------:|:-------------:|:------------:|:--------:|:---------------:|
| 01 degree     | 20973793      | 20015715     | 0.36     | 0.046           |
| 02 degree     | 20096345      | 19740706     | 0.64     | 0.018           |
| 03 degree     | 36881114      | 35020642     | 0.42     | 0.050           |
| 04 degree     | 67375082      | 58691517     | 0.56     | 0.129           |
| 05 degree     | 33704654      | 29683255     | 0.58     | 0.119           |
| 06 degree     | 28808088      | 27990235     | 0.40     | 0.028           |
| 07 degree     | 36719945      | 35473650     | 0.46     | 0.034           |
| 08 degree     | 30514396      | 29873771     | 0.48     | 0.021           |
| 09 degree     | 30163531      | 31010794     | 0.44     | 0.028           |
| 10 degree     | 34438147      | 34906570     | 0.50     | 0.014           |
| 20 degree     | 41281249      | 42932136     | 0.54     | 0.040           |
| 30 degree     | 29785108      | 30401221     | 0.44     | 0.021           |
| 40 degree     | 30536468      | 29818905     | 0.48     | 0.023           |

# Other manipulations

| Other manipulations   | Hash (before) | Hash (after) | Distance | Relative Change |
|:---------------------:|:-------------:|:------------:|:--------:|:---------------:|
| Left Rotate (90)      | 22267473      | 21941111     | 0.40     | 0.015           |
| Right Rotate (90)     | 32023788      | 32291200     | 0.36     | 0.008           |
| Horizontal Flip       | 28224592      | 27330120     | 0.40     | 0.032           |
| Vertical Flip         | 24303572      | 24704224     | 0.32     | 0.016           |
| Invert pixel values   | 16854459      | 17267339     | 0.36     | 0.024           |
| JPEG Compress         | 32846954      | 29239466     | 0.36     | 0.110           |
| 25% * pixel values    | 22860828      | 48778480     | 0.54     | 1.134           |
| 50% * pixel values    | 15877022      | 20929632     | 0.72     | 0.318           |
| 75% * pixel values    | 82890379      | 84215328     | 0.56     | 0.016           |
| Change 1 pixel values | 17807293      | 17806568     | 0.24     | 4.07e-05        |
| Change 10% of the img | 47481714      | 47844033     | 0.42     | 0.008           |
