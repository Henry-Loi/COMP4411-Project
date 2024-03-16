# Project 2 : Modeler
> **Assigned : Mar 2** \
> **Due : Mar 23**

## Basic Requirement

### List
1. Have at least ten primitives and at least four levels of hierarchy.
   
2. Add at least one of these as a control knob (slider, actually) for some joint/component of your model - have your character do some simple action as you scrub a slider back and forth.
   
3. At least one of your controls must be tied to more than one joint/component.
   
4. Implement your own version of gluLookAt() using any method (i.e. the standard transformations (glTranslate, glRotate, glScale), matrix construction). 

5. Complete two bells (or equivalent) from the list below.

### Progress
#### Task 1 - Model
- [X] at least ten primitives
- [ ] at least four levels of hierarchy

#### Task 2 - Control Knob
- [ ] Add at least one of these as a control knob
- [ ] Tied to more than one joint/component

#### Task 3 - LookAt
- [X] Implement own LookAt

#### Task 4 - At least 2 Bells
- [ ] 2 bells or equivalent


## Bells and Whistles
> **Total marks: 21.5/41** \
> 1 Bell = 1 mark, 1 whistle = 0.5 mark\
> I am summarizing the key points into bullet points. Please always check the [real requirement](https://course.cse.ust.hk/comp4411/Password_Only/projects/impressionist/index.html) before start working on it due to my poor english orz.

- [X] [0.5] More Brushes (1. Pixelize Brush 2. Triangle Brush)
- [X] [0.5] Clip brush strokes to the painted region
- [X] [0.5] Add a marker cursor to the original image
- [X] [0.5] swap contents of painting window and original image window
- [X] [1] independent RGB Brush
- [X] [1] Design Brush hat selectively applies one or more filters (e.g. blurring and sharpening). 
- [ ] [1] Undo feature for brush and filtering effects (undo every mouse up & down pair)
- [X] [1] Dissolve one image into another (set alpha to around 0.5-0.6 and paste on original view + image size checking)
- [X] [1.5] Add a slider that allows the user to fade in or fade out the original image beneath the user's brush strokes on the canvas. (fade the original as background of paint view (Hint: save the faded image separately with the paint buffer))

- [X] [1.5] Add a "mural" load in different images while preserving what has been drawn on the canvas
- [X] [1.5] Add "alpha-mapped" brush strokes
- [X] [1.5] Auto paint image (with randomness)
- [x] [1.5] Brush direction by gradient from user-specified image
- [x] [1.5] Edge Clipping (cut the exceed edge when painting)

- [X] [2] User-specified kernel (odd size matrix only)
- [x] [2] Edge detection + user-specified edge clipping (using sobal)
- [X] [2] Multiresolution automatic painting (1B)
- [ ] [2] Design a brush that can be used to stretch and pull the image as if it were rubber
- [X] [2] Design curved brush following image gradient
- [ ] [<=4] Construct Mosaic
- [ ] [4] Impressionist video with temporal coherency
- [ ] [8] Implement Bayesian matting
