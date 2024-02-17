//
// impressionistUI.h
//
// The user interface part for the program.
//

#include <FL/fl_ask.h>

#include <math.h>

#include "impressionistDoc.h"
#include "impressionistUI.h"

/*
//------------------------------ Widget Examples
------------------------------------------------- Here is some example code for
all of the widgets that you may need to add to the project.  You can copy and
paste these into your code and then change them to make them look how you want.
Descriptions for all of the widgets here can be found in links on the fltk help
session page.

//---------Window/Dialog and Menubar-----------------------------------

        //----To install a window--------------------------
        Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
                myWindow->user_data((void*)(this));	// record self to be
used by static callback functions

                // install menu bar
                myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
                Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
                        { "&File",		0, 0, 0, FL_SUBMENU },
                                { "&Load...",	FL_ALT + 'l', (Fl_Callback
*)ImpressionistUI::cb_load }, { "&Save...",	FL_ALT + 's', (Fl_Callback
*)ImpressionistUI::cb_save }.
                                { "&Quit",			FL_ALT + 'q',
(Fl_Callback *)ImpressionistUI::cb_exit }, { 0 }, { "&Edit",		0, 0, 0,
FL_SUBMENU }, { "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy,
(void *)COPY }, { "&Cut",	FL_ALT + 'x', (Fl_Callback
*)ImpressionistUI::cb_cut, (void *)CUT }, { "&Paste",	FL_ALT + 'v',
(Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE }, { 0 }, { "&Help",
0, 0, 0, FL_SUBMENU }, { "&About",	FL_ALT + 'a', (Fl_Callback
*)ImpressionistUI::cb_about }, { 0 }, { 0 }
                };
                myMenubar->menu(myMenuItems);
    m_mainWindow->end();

        //----The window callback--------------------------
        // One of the callbacks
        void ImpressionistUI::cb_load(Fl_Menu_* o, void* v)
        {
                ImpressionistDoc *pDoc=whoami(o)->getDocument();

                char* newfile = fl_file_chooser("Open File?", "*.bmp",
pDoc->getImageName() ); if (newfile != NULL) { pDoc->loadImage(newfile);
                }
        }


//------------Slider---------------------------------------

        //----To install a slider--------------------------
        Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My
Value"); mySlider->user_data((void*)(this));	// record self to be used by
static callback functions mySlider->type(FL_HOR_NICE_SLIDER);
    mySlider->labelfont(FL_COURIER);
    mySlider->labelsize(12);
        mySlider->minimum(1);
        mySlider->maximum(40);
        mySlider->step(1);
        mySlider->value(m_nMyValue);
        mySlider->align(FL_ALIGN_RIGHT);
        mySlider->callback(cb_MyValueSlides);

        //----The slider callback--------------------------
        void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
        {
                ((ImpressionistUI*)(o->user_data()))->m_nMyValue=int(
((Fl_Slider *)o)->value() ) ;
        }


//------------Choice---------------------------------------

        //----To install a choice--------------------------
        Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
        myChoice->user_data((void*)(this));	 // record self to be used by
static callback functions Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
          {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void
*)ONE},
          {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void
*)TWO},
          {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice,
(void *)THREE}, {0}
        };
        myChoice->menu(myChoiceMenu);
        myChoice->callback(cb_myChoice);

        //-----The choice callback-------------------------
        void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
        {
                ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
                ImpressionistDoc* pDoc=pUI->getDocument();

                int type=(int)v;

                pDoc->setMyType(type);
        }


//------------Button---------------------------------------

        //---To install a button---------------------------
        Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
        myButton->user_data((void*)(this));   // record self to be used by
static callback functions myButton->callback(cb_myButton);

        //---The button callback---------------------------
        void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
        {
                ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
                ImpressionistDoc* pDoc = pUI->getDocument();
                pDoc->startPainting();
        }


//---------Light Button------------------------------------

        //---To install a light button---------------------
        Fl_Light_Button* myLightButton = new
Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
        myLightButton->user_data((void*)(this));   // record self to be used by
static callback functions myLightButton->callback(cb_myLightButton);

        //---The light button callback---------------------
        void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
        {
                ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

                if (pUI->myBool==TRUE) pUI->myBool=FALSE;
                else pUI->myBool=TRUE;
        }

//----------Int Input--------------------------------------

    //---To install an int input-----------------------
        Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
        myInput->user_data((void*)(this));   // record self to be used by static
callback functions myInput->callback(cb_myInput);

        //---The int input callback------------------------
        void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
        {
                ((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int(
((Fl_Int_Input *)o)->value() );
        }

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions
//--------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI *ImpressionistUI::whoami(Fl_Menu_ *o) {
  return ((ImpressionistUI *)(o->parent()->user_data()));
}

//--------------------------------- Callback Functions
//--------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();

  char *newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
  if (newfile != NULL) {
    pDoc->loadImage(newfile);
  }
}

//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();

  char *newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp");
  if (newfile != NULL) {
    pDoc->saveImage(newfile);
  }
}

//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_ *o, void *v) {
  whoami(o)->m_brushDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();

  pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_ *o, void *v) {
  whoami(o)->m_mainWindow->hide();
  whoami(o)->m_brushDialog->hide();
}

//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_ *o, void *v) {
  fl_message("Impressionist FLTK version for CS341, Spring 2002");
}

//------- UI should keep track of the current for all the controls for answering
// the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush
// choice.
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = ((ImpressionistUI *)(o->user_data()));
  ImpressionistDoc *pDoc = pUI->getDocument();

  int type = (int)v;

  pDoc->setBrushType(type);

  // add activate and deactivate handler
  if (type == BRUSH_POINTS || type == BRUSH_SCATTERED_POINTS ||
      type == BRUSH_CIRCLES || type == BRUSH_SCATTERED_CIRCLES) {
    pUI->m_LineWidthSlider->deactivate();
    pUI->m_LineAngleSlider->deactivate();
    pUI->m_EdgeClippingLightButton->deactivate();
    pUI->m_AnotherGradientLightButton->deactivate();

    // reset the stroke direction
    pDoc->setStokeDirection(SLIDER_RIGHT_MOUSE);
    pUI->m_StrokeDirectionChoice->deactivate();

  } else {
    pUI->m_LineWidthSlider->activate();
    pUI->m_LineAngleSlider->activate();
    pUI->m_EdgeClippingLightButton->activate();
    pUI->m_AnotherGradientLightButton->activate();

    pUI->m_StrokeDirectionChoice->activate();
  }
}

void ImpressionistUI::cb_strokeDirectionChoice(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = ((ImpressionistUI *)(o->user_data()));
  ImpressionistDoc *pDoc = pUI->getDocument();

  int type = (int)v;

  pDoc->setStokeDirection(type);
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget *o, void *v) {
  ImpressionistDoc *pDoc = ((ImpressionistUI *)(o->user_data()))->getDocument();

  pDoc->clearCanvas();
}

void ImpressionistUI::cb_paint_button(Fl_Widget *o, void *v) {
  ImpressionistDoc *pDoc = ((ImpressionistUI *)(o->user_data()))->getDocument();

  pDoc->clearCanvas();
}

void ImpressionistUI::cb_do_it_button(Fl_Widget *o, void *v) {
  ImpressionistDoc *pDoc = ((ImpressionistUI *)(o->user_data()))->getDocument();

  pDoc->clearCanvas();
}

//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_nSize =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_lineWidth(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_lineWidth =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_lineAngle(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_lineAngle =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_alpha(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_alpha = (((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_spacing(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_spacing =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_edgeThreshold(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_edgeThreshold =
      int(((Fl_Slider *)o)->value());
}

//---The light button callback---------------------
void ImpressionistUI::cb_EdgeClippingLightButton(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = ((ImpressionistUI *)(o->user_data()));

  if (pUI->isEdgeClipping == TRUE)
    pUI->isEdgeClipping = FALSE;
  else
    pUI->isEdgeClipping = TRUE;
}

void ImpressionistUI::cb_AnotherGradientLightButton(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = ((ImpressionistUI *)(o->user_data()));

  if (pUI->isAnotherGradient == TRUE)
    pUI->isAnotherGradient = FALSE;
  else
    pUI->isAnotherGradient = TRUE;
}

void ImpressionistUI::cb_sizeRandLightButton(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = ((ImpressionistUI *)(o->user_data()));

  if (pUI->isSizeRand == TRUE)
    pUI->isSizeRand = FALSE;
  else
    pUI->isSizeRand = TRUE;
}

//---------------------------------- per instance functions
//--------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc *ImpressionistUI::getDocument() { return m_pDoc; }

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
  m_mainWindow->show();
  m_paintView->show();
  m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
  m_paintView->size(w, h);
  m_origView->size(w, h);
}

//------------------------------------------------
// Set the ImpressionistDoc used by the UI to
// communicate with the brushes
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc *doc) {
  m_pDoc = doc;

  m_origView->m_pDoc = doc;
  m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize() { return m_nSize; }

int ImpressionistUI::getLineWidth() { return m_lineWidth; }
int ImpressionistUI::getLineAngle() { return m_lineAngle; }
float ImpressionistUI::getAlpha() { return m_alpha; }
int ImpressionistUI::getSpacing() { return m_spacing; }
int ImpressionistUI::getEdgeThreshold() { return m_edgeThreshold; }

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize(int size) {
  m_nSize = size;

  if (size <= 40)
    m_BrushSizeSlider->value(m_nSize);
}
//-------------------------------------------------
// Set the brush angle in degree
//-------------------------------------------------
void ImpressionistUI::setLineAngle(int angle) {
  m_lineAngle = angle;
  if (angle <= 359)
    m_LineAngleSlider->value(m_lineAngle);
}

// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
    {"&File", 0, 0, 0, FL_SUBMENU},
    {"&Load Image...", FL_ALT + 'l',
     (Fl_Callback *)ImpressionistUI::cb_load_image},
    {"&Save Image...", FL_ALT + 's',
     (Fl_Callback *)ImpressionistUI::cb_save_image},
    {"&Brushes...", FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes},
    {"&Clear Canvas", FL_ALT + 'c',
     (Fl_Callback *)ImpressionistUI::cb_clear_canvas, 0, FL_MENU_DIVIDER},

    {"&Quit", FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit},
    {0},

    {"&Help", 0, 0, 0, FL_SUBMENU},
    {"&About", FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about},
    {0},

    {0}};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE + 1] = {
    {"Points", FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_POINTS},
    {"Lines", FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_LINES},
    {"Circles", FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_CIRCLES},
    {"Scattered Points", FL_ALT + 'q',
     (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_SCATTERED_POINTS},
    {"Scattered Lines", FL_ALT + 'm',
     (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_SCATTERED_LINES},
    {"Scattered Circles", FL_ALT + 'd',
     (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_SCATTERED_CIRCLES},
    {0}};

Fl_Menu_Item ImpressionistUI::strokeDirectionMenu[NUM_STROKE_DIRECTION + 1] = {
    {"Slider/Right Mouse", FL_ALT + 's',
     (Fl_Callback *)ImpressionistUI::cb_strokeDirectionChoice,
     (void *)SLIDER_RIGHT_MOUSE},
    {"Gradient", FL_ALT + 'g',
     (Fl_Callback *)ImpressionistUI::cb_strokeDirectionChoice,
     (void *)GRADIENT},
    {"Brush Direction", FL_ALT + 'b',
     (Fl_Callback *)ImpressionistUI::cb_strokeDirectionChoice,
     (void *)BRUSH_DIRECTION},
    {0}};

/* Helper function*/
void ImpressionistUI::brush_dialog_value_init() {
  // init values
  m_nSize = 10;
  m_lineWidth = 1;
  m_lineAngle = 0;
  m_alpha = 1.0;
  m_spacing = 4;
  m_edgeThreshold = 200;

  m_tarLineAngle = 0;

  isEdgeClipping = true;
  isAnotherGradient = false;
  isSizeRand = true;
}

//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
  // Create the main window
  m_mainWindow = new Fl_Window(600, 300, "Impressionist");
  m_mainWindow->user_data(
      (void *)(this)); // record self to be used by static callback functions
  // install menu bar
  m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
  m_menubar->menu(menuitems);

  // Create a group that will hold two sub windows inside the main
  // window
  Fl_Group *group = new Fl_Group(0, 25, 600, 275);

  // install paint view window
  m_paintView =
      new PaintView(300, 25, 300, 275, "This is the paint view"); // 0jon
  m_paintView->box(FL_DOWN_FRAME);

  // install original view window
  m_origView =
      new OriginalView(0, 25, 300, 275, "This is the orig view"); // 300jon
  m_origView->box(FL_DOWN_FRAME);
  m_origView->deactivate();

  group->end();
  Fl_Group::current()->resizable(group);
  m_mainWindow->end();

  brush_dialog_value_init();

  // brush dialog definition
  m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");
  // Add a brush type choice to the dialog
  m_BrushTypeChoice = new Fl_Choice(50, 10, 150, 25, "&Brush");
  m_BrushTypeChoice->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_BrushTypeChoice->menu(brushTypeMenu);
  m_BrushTypeChoice->callback(cb_brushChoice);

  m_ClearCanvasButton = new Fl_Button(240, 10, 150, 25, "&Clear Canvas");
  m_ClearCanvasButton->user_data((void *)(this));
  m_ClearCanvasButton->callback(cb_clear_canvas_button);

  m_StrokeDirectionChoice =
      new Fl_Choice(114, 45, 150, 25, "&Stroke Direction");
  m_StrokeDirectionChoice->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_StrokeDirectionChoice->menu(strokeDirectionMenu);
  m_StrokeDirectionChoice->callback(cb_brushChoice);

  // Add brush size slider to the dialog
  m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
  m_BrushSizeSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
  m_BrushSizeSlider->labelfont(FL_COURIER);
  m_BrushSizeSlider->labelsize(12);
  m_BrushSizeSlider->minimum(1);
  m_BrushSizeSlider->maximum(40);
  m_BrushSizeSlider->step(1);
  m_BrushSizeSlider->value(m_nSize);
  m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
  m_BrushSizeSlider->callback(cb_sizeSlides);

  // Add Line Width slide bar
  m_LineWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Width");
  m_LineWidthSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_LineWidthSlider->type(FL_HOR_NICE_SLIDER);
  m_LineWidthSlider->labelfont(FL_COURIER);
  m_LineWidthSlider->labelsize(12);
  m_LineWidthSlider->minimum(1);
  m_LineWidthSlider->maximum(40);
  m_LineWidthSlider->step(1);
  m_LineWidthSlider->value(m_lineWidth);
  m_LineWidthSlider->align(FL_ALIGN_RIGHT);
  m_LineWidthSlider->callback(cb_lineWidth);

  // Add Line Angle slide bar
  m_LineAngleSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle");
  m_LineAngleSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_LineAngleSlider->type(FL_HOR_NICE_SLIDER);
  m_LineAngleSlider->labelfont(FL_COURIER);
  m_LineAngleSlider->labelsize(12);
  m_LineAngleSlider->minimum(0);
  m_LineAngleSlider->maximum(359);
  m_LineAngleSlider->step(1);
  m_LineAngleSlider->value(m_lineAngle);
  m_LineAngleSlider->align(FL_ALIGN_RIGHT);
  m_LineAngleSlider->callback(cb_lineAngle);

  // Add Alpha slide bar
  m_AlphaSlider = new Fl_Value_Slider(10, 170, 300, 20, "Alpha");
  m_AlphaSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_AlphaSlider->type(FL_HOR_NICE_SLIDER);
  m_AlphaSlider->labelfont(FL_COURIER);
  m_AlphaSlider->labelsize(12);
  m_AlphaSlider->minimum(0.0);
  m_AlphaSlider->maximum(1.0);
  m_AlphaSlider->step(0.01);
  m_AlphaSlider->value(m_alpha);
  m_AlphaSlider->align(FL_ALIGN_RIGHT);
  m_AlphaSlider->callback(cb_alpha);
  // m_BrushSizeSlider->deactivate();

  m_EdgeClippingLightButton =
      new Fl_Light_Button(10, 200, 130, 25, "&Edge Clipping");
  m_EdgeClippingLightButton->user_data(
      (void *)(this)); // record self to be used by
  m_EdgeClippingLightButton->callback(cb_EdgeClippingLightButton);
  m_EdgeClippingLightButton->value(true);

  m_AnotherGradientLightButton =
      new Fl_Light_Button(240, 200, 150, 25, "&Another Gradient");
  m_AnotherGradientLightButton->user_data(
      (void *)(this)); // record self to be used by
  m_AnotherGradientLightButton->callback(cb_AnotherGradientLightButton);
  m_AnotherGradientLightButton->value(false);

  // Add Spacing slide bar
  m_SpacingSlider = new Fl_Value_Slider(10, 240, 150, 20, "Spacing");
  m_SpacingSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_SpacingSlider->type(FL_HOR_NICE_SLIDER);
  m_SpacingSlider->labelfont(FL_COURIER);
  m_SpacingSlider->labelsize(12);
  m_SpacingSlider->minimum(1);
  m_SpacingSlider->maximum(16);
  m_SpacingSlider->step(1);
  m_SpacingSlider->value(m_spacing);
  m_SpacingSlider->align(FL_ALIGN_RIGHT);
  m_SpacingSlider->callback(cb_spacing);

  m_SizeRandLightButton = new Fl_Light_Button(230, 240, 90, 20, "&Size Rand.");
  m_SizeRandLightButton->user_data((void *)(this)); // record self to be used by
  m_SizeRandLightButton->callback(cb_sizeRandLightButton);
  m_SizeRandLightButton->value(true);

  m_PaintButton = new Fl_Button(340, 240, 50, 20, "&Paint");
  m_PaintButton->user_data((void *)(this));
  m_PaintButton->callback(cb_paint_button);

  // Add Edge Threshold slide bar
  m_EdgeThresholdSlider =
      new Fl_Value_Slider(10, 280, 210, 20, "Edge Threshold");
  m_EdgeThresholdSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_EdgeThresholdSlider->type(FL_HOR_NICE_SLIDER);
  m_EdgeThresholdSlider->labelfont(FL_COURIER);
  m_EdgeThresholdSlider->labelsize(12);
  m_EdgeThresholdSlider->minimum(0);
  m_EdgeThresholdSlider->maximum(500);
  m_EdgeThresholdSlider->step(1);
  m_EdgeThresholdSlider->value(m_edgeThreshold);
  m_EdgeThresholdSlider->align(FL_ALIGN_RIGHT);
  m_EdgeThresholdSlider->callback(cb_edgeThreshold);

  m_DoItButton = new Fl_Button(340, 280, 50, 20, "&Do it");
  m_DoItButton->user_data((void *)(this));
  m_DoItButton->callback(cb_do_it_button);

  // deactivation init
  m_LineWidthSlider->deactivate();
  m_LineAngleSlider->deactivate();
  m_EdgeClippingLightButton->deactivate();
  m_AnotherGradientLightButton->deactivate();
  m_StrokeDirectionChoice->deactivate();

  m_brushDialog->end();
}
