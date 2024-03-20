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
- [X] 2 bells or equivalent


## Bells and Whistles
> **Total marks: 8.0/30** \
> 1 Bell = 1 mark, 1 whistle = 0.5 mark\
> I am summarizing the key points into bullet points. Please always check the [real requirement](https://course.cse.ust.hk/comp4411/Password_Only/projects/modeler/index.html) before start working on it due to my poor english orz.

- [X] [0.5] Change the default light source to illuminate your scene more dramatically. (Include light0, light1)
- [ ] [0.5] Allow for adjustable levels of detail for your character. You will need to add a UI control for this. (show and hide different level with slide bar)
- [ ] [0.5] Come up with another whistle and implement it.  A whistle is something that extends the use of one of the things you are already doing.  It is part of the basic model construction, but extended or cloned and modified in an interesting way.
- [X] [1] Use a texture map on all or part of your character. (The safest way to do this is to implement your own primitives inside your model file that do texture mapping.) (can toggle with slide bar)
- [ ] [1] Build a complex shape as a set of polygonal faces, using the "triangle" primitive to render them. 
- [ ] [1] Make an additional "animated" sequence your character can perform. (move the legs and hands continuously)

- [ ] [1] Add some widgets that control adjustable parameters to your model so that you can create individual-looking instances of your character. (show some cool stuff such as jetting and lasing)
- [X] [1] Add a "Frame All" feature that would change the position of the camera such that your model is entirely visible in the model window, regardless of the pose and location of your model. (no need to include the plane)
- [X] [1.5] The camera code has a constrained up vector -- modify the code to (1) handle camera twists (the framework is already there) and (2) to come up with an intuitive way for the user to twist the camera via mouse control. (roll of the camera?)
- [X] [2] Add a function in your model file for drawing a new type of primitive. (any of three can get two balls) (3: toras)

- [X] [2] Use some sort of procedural modeling (such as an L-system) to generate all or part of your character. (need slide bar, type, angle, iterator, display toggle)
- [ ] [2] Have your character react differently to UI controls depending on what mood they are in. (just movement is ok)

- [ ] [4] Implement your hierarchical model and "skin" it with metaballs. (add a adjustable metaball with the model)
- [ ] [8] "goal-oriented" motion (add and display a target point, make the hand always points to the target)
- [ ] [+4] add all angle constraints (Additional) 

