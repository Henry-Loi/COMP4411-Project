# Project 1 : Impressionist
> **Assigned : Feb 3** \
> **Due : Mar 2**

## Basic Requirement

### List
1. Implement 5 different brush types: **single line, scattered lines, scattered points, (filled) circles, and scattered (filled) circles**. See the sample solution for an example of each brush's appearance. Note that **scattered brushes should sample from each location they color individually, not just use a single color for each splotch**.
   
2. Add sliders to control various brush attributes. You need to include sliders for the **line thickness and brush angle**, in addition to the existing **brush size** slider.
   
3. Add the ability to **control the brush direction**. The stroke direction should be controlled four different ways: using a **slider value**, using the **right mouse button to drag out a direction line**, using the **direction of the cursor movement**, and using **directions that are perpendicular to the gradient of the image**. You can use a radio box to allow the user to select which method to use.
   
4. Allow the user to **change the opacity (alpha value) of the brush stroke**. An alpha value slider should be added to the controls window. 

### Progress
#### Task 1 - Brush Type
- [X] single line 
- [X] (filled) circles
- [X] scattered lines brush
- [X] scattered points brush
- [X] scattered (filled) circles

#### Task 2 - Brush Attributes
- [X] line thickness
- [X] brush angle
- [X] brush size

#### Task 3 - Brush Direction Control
- [X] slider value
- [X] right mouse angle control
- [X] cursor movement
- [X] gradient of image (perpendicular)

#### Task 4 - Alpha (Opacity) Control
- [X] brush alpha value control

## Bells and Whistles
> **Total marks: 16.5/41** \
> 1 Bell = 1 mark, 1 whistle = 0.5 mark\
> I am summarizing the key points into bullet points. Please always check the [real requirement](https://course.cse.ust.hk/comp4411/Password_Only/projects/impressionist/index.html) before start working on it due to my poor english orz.

- [X] [0.5] More Brushes (1. Curved Brush)
- [X] [0.5] Clip brush strokes to the painted region
- [X] [0.5] Add a marker cursor to the original image
- [X] [0.5] swap contents of painting window and original image window
- [X] [1] independent RGB Brush
- [ ] [1] Design Brush hat selectively applies one or more filters (e.g. blurring and sharpening). 
- [X] [1] Undo feature for brush and filtering effects (undo every mouse up & down pair)
- [X] [1] Dissolve one image into another (set alpha to around 0.5-0.6 and paste on original view + image size checking)
- [X] [1.5] Add a slider that allows the user to fade in or fade out the original image beneath the user's brush strokes on the canvas. (fade the original as background of paint view (Hint: save the faded image separately with the paint buffer))

- [X] [1.5] Add a "mural" load in different images while preserving what has been drawn on the canvas
- [X] [1.5] Add "alpha-mapped" brush strokes
- [X] [1.5] Auto paint image (with randomness)
- [x] [1.5] Brush direction by gradient from user-specified image
- [ ] [1.5] Edge Clipping (cut the exceed edge when painting)

- [X] [2] User-specified kernel (odd size matrix only)
- [x] [2] Edge detection + user-specified edge clipping (using sobal)
- [ ] [2] Multiresolution automatic painting
- [ ] [2] Design a brush that can be used to stretch and pull the image as if it were rubber
- [ ] [2] Design curved brush following image gradient
- [ ] [<=4] Construct Mosaic
- [ ] [4] Impressionist video with temporal coherency
- [ ] [8] Implement Bayesian matting
