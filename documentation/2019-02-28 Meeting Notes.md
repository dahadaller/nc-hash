# Notes 2/28/19

- C pre processing This is separate from Java.
  - image
  - cropping gray scaling
  - What image format. Only jpeg 
- There's three parts to the program.
- Java Front End, C pre processing image, Obliv-C
- Presentation: All parts of the project can be equally time consuming. 

### Presentation:

- If the goal is to present it to Facebook its a big deal. Kieran suggested the Zahn Center (whatever that is).
- Zahn Center is there to help us present the project. "Soft skills".
- Multiple versions of the software maybe required. 

### The App Itself (Work Flow):

- Take start up and installer for granted (Worry about later).
- Dragging the image to the window. (We already have it).
- Preserving the aspect ratio of the image. 
  - Currently the program will cut the size down.
  - We need it to be responsive. Design (Changes in window size).
  - **Look into JavaFX media queries**
    - Layout engine ? There's currently a lot of white space.
- Random Idea by professor: Some kind of visual representation for...
  - Hash creation
  - Secure transmission.
- Make an animation to show the user that the image has been hashed. Some kind of QR code/ bar code. (**Visual Metaphor**).
- Recursively pixelate the image and fade it white. (David drawing graph)
- Some kind of loading bar for the user.     
- Professor's idea: Custom file browser which shows the image icon but pixelated. Reasoning : Discretely select the image for hashing. Maybe we don't the user to see the image itself. We want the user to feel safe.
  - (David) Custom file browser to select image without showing it. Mouse-over, de-blurring, toggle on-off blurriness.   
- Input validation in (image throw error to java.) Error must appear to user. 

### Maven:

- Kieran tried to get a javaFX demo to work, but kept running into road blocks. 
- Complicated, annoying, setting up is hard.

### April 19th 7-8 Weeks:

#### Week One - Two:
  - Development environment. Maven, Auto Tools, Linux.
- Java Catch Up (David) Jon's tutorials.
- JNI Hello World (I hate this crap).
- No virtual machine (Thank God).

#### Week Three - Five:

- C programming, David, Bon.
- JavaFX : Jon
- JNI: Kieran

### GitHub:

- David will write some scripts for automation and documentation. 
- Reverting back to previous versions. 

### Unfinish Stuff:

- Input validation
- Preproccessing
- Aspect Ratio
- Running C from interface without error. 