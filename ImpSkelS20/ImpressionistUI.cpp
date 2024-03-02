//
// impressionistUI.h
//
// The user interface part for the program.
//

#include <FL/fl_ask.h>

#include <iterator>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <string>

#include "PaintView.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include <iostream>

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

void ImpressionistUI::cb_load_mural_image(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();

  char *newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
  if (newfile != NULL) {
    pDoc->loadMuralImage(newfile);
  }
}

void ImpressionistUI::cb_load_another_image(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();

  char *newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
  if (newfile != NULL) {
    pDoc->loadAnotherImage(newfile);
  }
}


void ImpressionistUI::cb_load_edge_image(Fl_Menu_* o, void* v) {
    ImpressionistDoc* pDoc = whoami(o)->getDocument();

    char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
    if (newfile != NULL) {
        pDoc->loadEdgeImage(newfile);
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

void ImpressionistUI::cb_dissolve_image(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();

  char *newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
  if (newfile != NULL) {
    pDoc->dissolveImage(newfile);
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
void ImpressionistUI::cb_colorSelection(Fl_Menu_ *o, void *v) {
  whoami(o)->m_colorSelectionDialog->show();
}

void ImpressionistUI::cb_fade(Fl_Menu_ *o, void *) {
  whoami(o)->m_fadeDialog->show();
}

void ImpressionistUI::cb_painterly(Fl_Menu_ *o, void *v) {
  whoami(o)->m_painterlyDialog->show();
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
// Swaps the paintview canvas.
// Called by the UI when the swap canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_swap_canvas(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();

  pDoc->swapCanvas();
}

//------------------------------------------------------------
// Swaps the paintview canvas.
// Called by the UI when the swap canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_undo(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();

  pDoc->Undo();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_ *o, void *v) {
  whoami(o)->m_mainWindow->hide();
  whoami(o)->m_brushDialog->hide();
  whoami(o)->m_painterlyDialog->hide();
}

//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_ *o, void *v) {
  fl_message("Impressionist FLTK version for CS341, Spring 2002");
}

/* *************************
 * Custom Kernel
 ************************** */
bool ImpressionistUI::parseKernel() {
  matrix_kernel.clear();
  std::string input(m_KernelStr);
  if (input.empty()) {
    fl_alert("Invalid kernel input! Input is empty!");
    return false;
  }
  std::stringstream ss(input);
  std::stringstream ssSize(input);

  int size = 0;
  float dummy = 0.0f;
  float sum = 0.0f;
  while (ssSize.rdbuf()->in_avail()) {
    // check if the input is all number
    if (!isdigit((ssSize.peek())) && ssSize.peek() != ' ' &&
        ssSize.peek() != '-') {
      char test = ssSize.peek();
      fl_alert("Invalid kernel input! Input must be all numbers!");
      return false;
    }
    ssSize >> dummy;
    sum += dummy;
    size++;
  }

  // check if the input is a square odd matrix
  if (sqrt(size) != (int)sqrt(size) || size % 2 == 0) {
    fl_alert("Invalid kernel size! It should be square matrix in odd size! "
             "Detected number(s): %d",
             sqrt(size));
    return false;
  }

  size = sqrt(size);

  for (int i = 0; i < size; i++) {
    std::vector<float> row;
    for (int j = 0; j < size; j++) {
      float tmp;
      if (!ss.rdbuf()->in_avail())
        return false;
      ss >> tmp;
      if (m_IsNormalizedKernel && sum != 0.0f) {
        tmp /= sum;
      }
      row.push_back(tmp);
    }
    matrix_kernel.push_back(row);
  }
  return true;
}

void ImpressionistUI::cb_KernelInput(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = (ImpressionistUI *)(o->user_data());

  const char *str = ((Fl_Input *)o)->value();
  strcpy(pUI->m_KernelStr, str);
}

void ImpressionistUI::cb_KernelApplyButton(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = (ImpressionistUI *)(o->user_data());
  if (pUI->parseKernel()) {
    pUI->m_paintView->applyKernel();
  }
}

void ImpressionistUI::cb_KernelNormalizeButton(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = (ImpressionistUI *)(o->user_data());
  pUI->m_IsNormalizedKernel = !pUI->m_IsNormalizedKernel;
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

  // add brush init & its checking
  if (!ImpBrush::c_pBrushes[type]->BrushInit(nullptr)) {
    fl_alert("Brush failed to initialize!");
    return;
  }

  pDoc->setBrushType(type);

  // add activate and deactivate handler
  switch (type) {
  case BRUSH_POINTS:
  case BRUSH_SCATTERED_POINTS:
  case BRUSH_CIRCLES:
  case BRUSH_SCATTERED_CIRCLES: {
    pUI->m_LineWidthSlider->deactivate();
    pUI->m_LineAngleSlider->deactivate();
    pUI->m_EdgeClippingLightButton->deactivate();
    pUI->m_AnotherGradientLightButton->deactivate();

    // reset the stroke direction
    pDoc->setStokeDirection(SLIDER_RIGHT_MOUSE);
    pUI->m_StrokeDirectionChoice->deactivate();
    break;
  }
  case BRUSH_ALPHA_MAPPED: {
    // no alpha slider
    pUI->m_AlphaSlider->deactivate();

    pUI->m_LineWidthSlider->deactivate();
    pUI->m_LineAngleSlider->deactivate();
    pUI->m_EdgeClippingLightButton->deactivate();
    pUI->m_AnotherGradientLightButton->deactivate();

    // reset the stroke direction
    pDoc->setStokeDirection(SLIDER_RIGHT_MOUSE);
    pUI->m_StrokeDirectionChoice->deactivate();

    pUI->m_SizeRandLightButton->deactivate();
    pUI->m_SizeRandLightButton->value(false);
    pUI->isSizeRand = false;
    break;
  }
  case BRUSH_CUSTOM_KERNEL: {
    pUI->m_LineWidthSlider->deactivate();
    pUI->m_LineAngleSlider->deactivate();
    pUI->m_EdgeClippingLightButton->deactivate();
    pUI->m_AnotherGradientLightButton->deactivate();

    // reset the stroke direction
    pDoc->setStokeDirection(SLIDER_RIGHT_MOUSE);
    pUI->m_StrokeDirectionChoice->deactivate();

    // active kernel only option
    pUI->m_KernelInput->activate();
    pUI->m_KernelApplyButton->activate();
    pUI->m_KernelNormalizeButton->activate();
    break;
  }
  case BRUSH_CURVED: {
    pUI->m_LineWidthSlider->deactivate();
    pUI->m_BrushSizeSlider->deactivate();

    pUI->m_SizeRandLightButton->deactivate();
    pUI->m_SizeRandLightButton->value(false);
    pUI->isSizeRand = false;
    break;
  }
  default: {
    pUI->m_KernelInput->deactivate();
    pUI->m_KernelApplyButton->deactivate();
    pUI->m_KernelNormalizeButton->deactivate();

    pUI->m_SizeRandLightButton->activate();
    pUI->m_AlphaSlider->activate();
    pUI->m_LineWidthSlider->activate();
    pUI->m_LineAngleSlider->activate();
    pUI->m_EdgeClippingLightButton->activate();
    pUI->m_AnotherGradientLightButton->activate();

    pUI->m_BrushSizeSlider->activate();

    pUI->m_StrokeDirectionChoice->activate();
    break;
  }
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
  PaintView *pView = ((ImpressionistUI *)(o->user_data()))->getPaintView();
  pView->autoPaint();
}

void ImpressionistUI::cb_do_it_button(Fl_Widget *o, void *v) {
  ImpressionistDoc *pDoc = ((ImpressionistUI *)(o->user_data()))->getDocument();
  pDoc->m_pCurrentBrush->EdgeAutoPaint();
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

void ImpressionistUI::cb_ManualColor(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_color1 =
      float(((Fl_Color_Chooser *)o)->r());
  ((ImpressionistUI *)(o->user_data()))->m_color2 =
      float(((Fl_Color_Chooser *)o)->g());
  ((ImpressionistUI *)(o->user_data()))->m_color3 =
      float(((Fl_Color_Chooser *)o)->b());
}

//---Display callbacks
void ImpressionistUI::cb_display_original_image(Fl_Menu_ *o, void *v) {
  ImpressionistDoc *pDoc = whoami(o)->getDocument();
  pDoc->m_ucBitmap = pDoc->m_ucOriginal;
  pDoc->m_pUI->m_origView->refresh();
}

void ImpressionistUI::cb_display_another_image(Fl_Menu_* o, void* v) {
    ImpressionistDoc* pDoc = whoami(o)->getDocument();
    if(pDoc->m_ucAnotherImage!=nullptr)
        pDoc->m_ucBitmap = pDoc->m_ucAnotherImage;
    pDoc->m_pUI->m_origView->refresh();
}
void ImpressionistUI::cb_display_edge_image(Fl_Menu_* o, void* v) {
    ImpressionistDoc* pDoc = whoami(o)->getDocument();
    if (pDoc->m_ucEdgeImage != nullptr)
        pDoc->m_ucBitmap = pDoc->m_ucEdgeImage;
    pDoc->m_pUI->m_origView->refresh();
}

//---------------------------------- per instance functions
//--------------------------------------

//------------------------------------------------
ImpressionistDoc *ImpressionistUI::getDocument() { return m_pDoc; }
// Return the ImpressionistDoc used
//------------------------------------------------

PaintView *ImpressionistUI::getPaintView() { return m_paintView; };

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
  std::cout << "PaintView_x:" << m_paintView->x() << " " << m_paintView->y() << std::endl;


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

bool ImpressionistUI::get_IsNormalizedKernel() { return m_IsNormalizedKernel; }

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
    {"&Add Mural Image", FL_ALT + 'm',
     (Fl_Callback *)ImpressionistUI::cb_load_mural_image},
    {"&Save Image...", FL_ALT + 's',
     (Fl_Callback *)ImpressionistUI::cb_save_image},
    {"&Dissolve Image...", FL_ALT + 'd',
     (Fl_Callback *)ImpressionistUI::cb_dissolve_image, 0, FL_MENU_DIVIDER},

    {"&Brushes...", FL_ALT + 'b', (Fl_Callback *)ImpressionistUI::cb_brushes},
    {"&Colors...", FL_ALT + 'k',
     (Fl_Callback *)ImpressionistUI::cb_colorSelection, 0, FL_MENU_DIVIDER},

    {"&Clear Canvas", FL_ALT + 'c',
     (Fl_Callback *)ImpressionistUI::cb_clear_canvas},
    {"&Swap Canvas", FL_ALT + 'w',
     (Fl_Callback *)ImpressionistUI::cb_swap_canvas, 0, FL_MENU_DIVIDER},

    {"&Fade image...", FL_ALT + 'f', (Fl_Callback *)ImpressionistUI::cb_fade, 0,
     FL_MENU_DIVIDER},

    {"&Load Another Image...", FL_ALT + 'a',
     (Fl_Callback *)ImpressionistUI::cb_load_another_image},
    {"&Load Edge Image...", FL_ALT + 'e',
     (Fl_Callback*)ImpressionistUI::cb_load_edge_image},

    {"&Undo", FL_ALT + 'u', (Fl_Callback *)ImpressionistUI::cb_undo, 0,
     FL_MENU_DIVIDER},

    {"&Painterly", FL_ALT + 'p', (Fl_Callback *)ImpressionistUI::cb_painterly,
     0, FL_MENU_DIVIDER},

    {"&Quit", FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit},
    {0},
    {"&Display", 0, 0, 0, FL_SUBMENU},
    {"&Original Image...", FL_ALT + 'o',
     (Fl_Callback*)ImpressionistUI::cb_display_original_image},
    {"&Edge Image...", FL_ALT + 'e',
     (Fl_Callback*)ImpressionistUI::cb_display_edge_image},
    {"&Another Image...", FL_ALT + 'a',
     (Fl_Callback*)ImpressionistUI::cb_display_another_image},
    {0},
    {"&Help", 0, 0, 0, FL_SUBMENU},
    {"&About", FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about},
    {0},

    {0}};

void ImpressionistUI::cb_fadeAlpha(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = ((ImpressionistUI *)(o->user_data()));
  ImpressionistDoc *pDoc = pUI->getDocument();

  ((ImpressionistUI *)(o->user_data()))->fadeAlpha =
      int(((Fl_Slider *)o)->value());
  pDoc->fadeAlpha();
}

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
    {"Alpha Mapped", FL_ALT + 'a',
     (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_ALPHA_MAPPED},
    {"Kernel", FL_ALT + 'k', (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_CUSTOM_KERNEL},
    {"Curved", FL_ALT + 'u', (Fl_Callback *)ImpressionistUI::cb_brushChoice,
     (void *)BRUSH_CURVED},
    {"Blur", FL_ALT + 'b', (Fl_Callback*)ImpressionistUI::cb_brushChoice,
     (void*)BRUSH_BLUR},
    {"Sharpening", FL_ALT + 's', (Fl_Callback*)ImpressionistUI::cb_brushChoice,
     (void*)BRUSH_SHARPENING},
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

  m_IsNormalizedKernel = true;
}

void ImpressionistUI::color_Selection_init() {
  m_color1 = 1.000;
  m_color2 = 1.000;
  m_color3 = 1.000;
}

float ImpressionistUI::get_m_R() { return m_color1; }
float ImpressionistUI::get_m_B() { return m_color2; }
float ImpressionistUI::get_m_G() { return m_color3; }

Fl_Menu_Item ImpressionistUI::painterlyStyleMenu[NUM_OF_PAINTERLY_STYLE + 1] = {
    {"Impressionist", FL_ALT + 'i',
     (Fl_Callback *)ImpressionistUI::cb_painterlyStyleChoice,
     (void *)PAINTERLY_IMPRESSIONIST},
    {"Expressionist", FL_ALT + 'e',
     (Fl_Callback *)ImpressionistUI::cb_painterlyStyleChoice,
     (void *)PAINTERLY_EXPRESSIONIST},
    {"Color Wash", FL_ALT + 'c',
     (Fl_Callback *)ImpressionistUI::cb_painterlyStyleChoice,
     (void *)PAINTERLY_COLOR_WASH},
    {"Pointillist", FL_ALT + 'p',
     (Fl_Callback *)ImpressionistUI::cb_painterlyStyleChoice,
     (void *)PAINTERLY_POINTILLIST},
    {"Custom", FL_ALT + 'u',
     (Fl_Callback *)ImpressionistUI::cb_painterlyStyleChoice,
     (void *)PAINTERLY_CUSTOMIZED},
    {0}};

Fl_Menu_Item ImpressionistUI::painterlyStrokeMenu[NUM_OF_PAINTERLY_STROKE + 1] =
    {{"Curve Brush", FL_ALT + 'c',
      (Fl_Callback *)ImpressionistUI::cb_painterlyStrokeChoice,
      (void *)PAINTERLY_CURVE_BRUSH},
     {"BSpline Brush", FL_ALT + 'b',
      (Fl_Callback *)ImpressionistUI::cb_painterlyStrokeChoice,
      (void *)PAINTERLY_BSPLINE_BRUSH},
     {"Circle Brush", FL_ALT + 'p',
      (Fl_Callback *)ImpressionistUI::cb_painterlyStrokeChoice,
      (void *)PAINTERLY_CIRCLE_BRUSH},
     {"Clip Line Brush", FL_ALT + 'l',
      (Fl_Callback *)ImpressionistUI::cb_painterlyStrokeChoice,
      (void *)PAINTERLY_CLIP_LINE_BRUSH},
     {"Line Brush", FL_ALT + 'n',
      (Fl_Callback *)ImpressionistUI::cb_painterlyStrokeChoice,
      (void *)PAINTERLY_LINE_BRUSH},
     {0}};

void ImpressionistUI::painterly_dialog_value_init() {
  // default init
}

void ImpressionistUI::cb_painterlyStyleChoice(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = ((ImpressionistUI *)(o->user_data()));
  ImpressionistDoc *pDoc = pUI->getDocument();

  PainterlyStyle type = static_cast<PainterlyStyle>((int)v);

  pUI->m_pPainterlyBrush->m_painterlyStyle = type;

  if (type == PAINTERLY_CUSTOMIZED) {
    pUI->m_paintView->get_painterly_param()->Threshold =
        pUI->m_painterlyThreshold;
    pUI->m_paintView->get_painterly_param()->Curvature =
        pUI->m_painterlyCurvature;
    pUI->m_paintView->get_painterly_param()->Blur = pUI->m_painterlyBlur;
    pUI->m_paintView->get_painterly_param()->GridSize =
        pUI->m_painterlyGridSize;
    pUI->m_paintView->get_painterly_param()->MinStrokeLength =
        pUI->m_painterlyMinStrokeLength;
    pUI->m_paintView->get_painterly_param()->MaxStrokeLength =
        pUI->m_painterlyMaxStrokeLength;
    pUI->m_paintView->get_painterly_param()->Alpha = pUI->m_painterlyAlpha;
    pUI->m_paintView->get_painterly_param()->Layer = pUI->m_painterlyLayers;
    pUI->m_paintView->get_painterly_param()->R0Level = pUI->m_painterlyR0Level;
    pUI->m_paintView->get_painterly_param()->Jr = pUI->m_painterlyJr;
    pUI->m_paintView->get_painterly_param()->Jg = pUI->m_painterlyJg;
    pUI->m_paintView->get_painterly_param()->Jb = pUI->m_painterlyJb;
    pUI->m_paintView->get_painterly_param()->Jh = pUI->m_painterlyJh;
    pUI->m_paintView->get_painterly_param()->Js = pUI->m_painterlyJs;
    pUI->m_paintView->get_painterly_param()->Jv = pUI->m_painterlyJv;

    pUI->m_painterlyStrokeChoice->activate();
    pUI->m_painterlyThresholdSlider->activate();
    pUI->m_painterlyCurvatureSlider->activate();
    pUI->m_painterlyBlurSlider->activate();
    pUI->m_painterlyGridSizeSlider->activate();
    pUI->m_painterlyMinStrokeLengthSlider->activate();
    pUI->m_painterlyMaxStrokeLengthSlider->activate();
    pUI->m_painterlyAlphaSlider->activate();
    pUI->m_painterlyLayersSlider->activate();
    pUI->m_painterlyR0LevelSlider->activate();
    pUI->m_painterlyJrSlider->activate();
    pUI->m_painterlyJgSlider->activate();
    pUI->m_painterlyJbSlider->activate();
    pUI->m_painterlyJhSlider->activate();
    pUI->m_painterlyJsSlider->activate();
    pUI->m_painterlyJvSlider->activate();

    pUI->m_painterlyThresholdSlider->value(pUI->m_painterlyThreshold);
    pUI->m_painterlyCurvatureSlider->value(pUI->m_painterlyCurvature);
    pUI->m_painterlyBlurSlider->value(pUI->m_painterlyBlur);
    pUI->m_painterlyGridSizeSlider->value(pUI->m_painterlyGridSize);
    pUI->m_painterlyMinStrokeLengthSlider->value(
        pUI->m_painterlyMinStrokeLength);
    pUI->m_painterlyMaxStrokeLengthSlider->value(
        pUI->m_painterlyMaxStrokeLength);
    pUI->m_painterlyAlphaSlider->value(pUI->m_painterlyAlpha);
    pUI->m_painterlyLayersSlider->value(pUI->m_painterlyLayers);
    pUI->m_painterlyR0LevelSlider->value(pUI->m_painterlyR0Level);
    pUI->m_painterlyJrSlider->value(pUI->m_painterlyJr);
    pUI->m_painterlyJgSlider->value(pUI->m_painterlyJg);
    pUI->m_painterlyJbSlider->value(pUI->m_painterlyJb);
    pUI->m_painterlyJhSlider->value(pUI->m_painterlyJh);
    pUI->m_painterlyJsSlider->value(pUI->m_painterlyJs);
    pUI->m_painterlyJvSlider->value(pUI->m_painterlyJv);
  } else {
    pUI->m_painterlyStrokeChoice->value(PAINTERLY_CURVE_BRUSH);
    pUI->m_painterlyThresholdSlider->value(
        pUI->m_paintView->get_painterly_param()->Threshold);
    pUI->m_painterlyCurvatureSlider->value(
        pUI->m_paintView->get_painterly_param()->Curvature);
    pUI->m_painterlyBlurSlider->value(
        pUI->m_paintView->get_painterly_param()->Blur);
    pUI->m_painterlyGridSizeSlider->value(
        pUI->m_paintView->get_painterly_param()->GridSize);
    pUI->m_painterlyMinStrokeLengthSlider->value(
        pUI->m_paintView->get_painterly_param()->MinStrokeLength);
    pUI->m_painterlyMaxStrokeLengthSlider->value(
        pUI->m_paintView->get_painterly_param()->MaxStrokeLength);
    pUI->m_painterlyAlphaSlider->value(
        pUI->m_paintView->get_painterly_param()->Alpha);
    pUI->m_painterlyLayersSlider->value(
        pUI->m_paintView->get_painterly_param()->Layer);
    pUI->m_painterlyR0LevelSlider->value(
        pUI->m_paintView->get_painterly_param()->R0Level);
    pUI->m_painterlyJrSlider->value(
        pUI->m_paintView->get_painterly_param()->Jr);
    pUI->m_painterlyJgSlider->value(
        pUI->m_paintView->get_painterly_param()->Jg);
    pUI->m_painterlyJbSlider->value(
        pUI->m_paintView->get_painterly_param()->Jb);
    pUI->m_painterlyJhSlider->value(
        pUI->m_paintView->get_painterly_param()->Jh);
    pUI->m_painterlyJsSlider->value(
        pUI->m_paintView->get_painterly_param()->Js);
    pUI->m_painterlyJvSlider->value(
        pUI->m_paintView->get_painterly_param()->Jv);

    pUI->m_painterlyStrokeChoice->deactivate();
    pUI->m_painterlyThresholdSlider->deactivate();
    pUI->m_painterlyCurvatureSlider->deactivate();
    pUI->m_painterlyBlurSlider->deactivate();
    pUI->m_painterlyGridSizeSlider->deactivate();
    pUI->m_painterlyMinStrokeLengthSlider->deactivate();
    pUI->m_painterlyMaxStrokeLengthSlider->deactivate();
    pUI->m_painterlyAlphaSlider->deactivate();
    pUI->m_painterlyLayersSlider->deactivate();
    pUI->m_painterlyR0LevelSlider->deactivate();
    pUI->m_painterlyJrSlider->deactivate();
    pUI->m_painterlyJgSlider->deactivate();
    pUI->m_painterlyJbSlider->deactivate();
    pUI->m_painterlyJhSlider->deactivate();
    pUI->m_painterlyJsSlider->deactivate();
    pUI->m_painterlyJvSlider->deactivate();
  }
}

void ImpressionistUI::cb_painterlyStrokeChoice(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = ((ImpressionistUI *)(o->user_data()));
  ImpressionistDoc *pDoc = pUI->getDocument();

  PainterlyStroke style = static_cast<PainterlyStroke>((int)v);

  pUI->m_paintView->setPainterlyStroke(style);
}

void ImpressionistUI::cb_painterlyPaintButton(Fl_Widget *o, void *v) {
  ImpressionistUI *pUI = ((ImpressionistUI *)(o->user_data()));

  pUI->m_paintView->apply_painterly();
}

void ImpressionistUI::cb_painterlyThreshold(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyThreshold =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyCurvature(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyCurvature =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyBlur(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyBlur =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyGridSize(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyGridSize =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyMinStrokeLength(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyMinStrokeLength =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyMaxStrokeLength(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyMaxStrokeLength =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyAlpha(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyAlpha =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyLayers(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyLayers =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyR0Level(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyR0Level =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyJr(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyJr =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyJg(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyJg =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyJb(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyJb =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyJh(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyJh =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyJs(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyJs =
      int(((Fl_Slider *)o)->value());
}

void ImpressionistUI::cb_painterlyJv(Fl_Widget *o, void *v) {
  ((ImpressionistUI *)(o->user_data()))->m_painterlyJv =
      int(((Fl_Slider *)o)->value());
}

int ImpressionistUI::getPainterlyStyle() {
  return m_pPainterlyBrush->m_painterlyStyle;
}
int ImpressionistUI::getPainterlyStroke() {
  return m_pPainterlyBrush->m_painterlyStroke;
}

int ImpressionistUI::getPainterlyThreshold() { return m_painterlyThreshold; }
float ImpressionistUI::getPainterlyCurvature() { return m_painterlyCurvature; }
float ImpressionistUI::getPainterlyBlur() { return m_painterlyBlur; }
float ImpressionistUI::getPainterlyGridSize() { return m_painterlyGridSize; }
int ImpressionistUI::getPainterlyMinStrokeLength() {
  return m_painterlyMinStrokeLength;
}
int ImpressionistUI::getPainterlyMaxStrokeLength() {
  return m_painterlyMaxStrokeLength;
}
float ImpressionistUI::getPainterlyAlpha() { return m_painterlyAlpha; }
int ImpressionistUI::getPainterlyLayers() { return m_painterlyLayers; }
int ImpressionistUI::getPainterlyR0Level() { return m_painterlyR0Level; }

float ImpressionistUI::getPainterlyJr() { return m_painterlyJr; }
float ImpressionistUI::getPainterlyJg() { return m_painterlyJg; }
float ImpressionistUI::getPainterlyJb() { return m_painterlyJb; }

float ImpressionistUI::getPainterlyJh() { return m_painterlyJh; }
float ImpressionistUI::getPainterlyJs() { return m_painterlyJs; }
float ImpressionistUI::getPainterlyJv() { return m_painterlyJv; }

//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI(ImpressionistDoc *pDoc) {
  m_pDoc = pDoc;
  m_KernelStr = new char[1000];
  memset(m_KernelStr, 0, 1000);

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
  m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view",
                              m_pDoc); // 0jon
  m_paintView->box(FL_DOWN_FRAME);

  // install original view window
  m_origView =
      new OriginalView(0, 25, 300, 275, "This is the orig view"); // 300jon
  m_origView->box(FL_DOWN_FRAME);
  m_origView->deactivate();

  group->end();
  Fl_Group::current()->resizable(group);
  m_mainWindow->end();

  m_pPainterlyBrush = new PainterlyBrush(m_pDoc, "Painterly Brush");

  brush_dialog_value_init();

  // brush dialog definition
  m_brushDialog = new Fl_Window(400, 380, "Brush Dialog");
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
  m_StrokeDirectionChoice->callback(cb_strokeDirectionChoice);

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

  m_KernelInput = new Fl_Input(50, 310, 300, 35, "Kernel");
  m_KernelInput->value(m_KernelStr);
  m_KernelInput->user_data((void *)(this));
  m_KernelInput->callback(cb_KernelInput);

  m_KernelApplyButton = new Fl_Button(60, 350, 150, 20, "Apply");
  m_KernelApplyButton->user_data((void *)(this));
  m_KernelApplyButton->callback(cb_KernelApplyButton);

  m_KernelNormalizeButton = new Fl_Check_Button(240, 348, 150, 25, "Normalize");
  m_KernelNormalizeButton->user_data((void *)(this));
  m_KernelNormalizeButton->value(m_IsNormalizedKernel);
  m_KernelNormalizeButton->callback(cb_KernelNormalizeButton);

  // deactivation init
  m_LineWidthSlider->deactivate();
  m_LineAngleSlider->deactivate();
  m_EdgeClippingLightButton->deactivate();
  m_AnotherGradientLightButton->deactivate();
  m_StrokeDirectionChoice->deactivate();

  // deactivate kernel only option
  m_KernelInput->deactivate();
  m_KernelApplyButton->deactivate();
  m_KernelNormalizeButton->deactivate();

  m_brushDialog->end();

  // Manual Color Slection
  // color_Selection_init();
  m_colorSelectionDialog = new Fl_Window(220, 220, "color Selection Dialog");
  Color_Selection = new Fl_Color_Chooser(10, 20, 200, 190, "Color Blending");
  Color_Selection->user_data((void *)(this));
  /*Color_Selection->type(FL_RGB);*/
  Color_Selection->rgb(1.000, 1.000, 1.000);
  Color_Selection->callback(cb_ManualColor);

  m_colorSelectionDialog->end();
  // Manual Color Selection end

  // dissolve dialog definition
  m_fadeDialog = new Fl_Window(320, 100, "Dissolve...");

  m_fadeOpacitySlider = new Fl_Value_Slider(10, 10, 300, 20, "Opacity");
  m_fadeOpacitySlider->user_data(static_cast<void *>(this));
  m_fadeOpacitySlider->type(FL_HOR_NICE_SLIDER);
  m_fadeOpacitySlider->minimum(0);
  m_fadeOpacitySlider->maximum(100);
  m_fadeOpacitySlider->step(1);
  m_fadeOpacitySlider->value(fadeAlpha);
  m_fadeOpacitySlider->align(FL_ALIGN_BOTTOM);
  m_fadeOpacitySlider->callback(cb_fadeAlpha);

  m_fadeDialog->end();

  // painterly dialog definition
  m_painterlyDialog = new Fl_Window(400, 280, "Painterly Dialog");
  painterly_dialog_value_init();

  // Add a brush type choice to the dialog
  m_painterlyStyleTypeChoice = new Fl_Choice(50, 10, 120, 20, "&Style");
  m_painterlyStyleTypeChoice->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyStyleTypeChoice->menu(painterlyStyleMenu);
  m_painterlyStyleTypeChoice->callback(cb_painterlyStyleChoice);

  m_painterlyStrokeChoice = new Fl_Choice(220, 10, 110, 20, "&Stroke");
  m_painterlyStrokeChoice->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyStrokeChoice->menu(painterlyStrokeMenu);
  m_painterlyStrokeChoice->callback(cb_painterlyStrokeChoice);

  m_painterlyPaintButton = new Fl_Button(340, 10, 50, 20, "&Run");
  m_painterlyPaintButton->user_data((void *)(this));
  m_painterlyPaintButton->callback(cb_painterlyPaintButton);

  m_painterlyThresholdSlider =
      new Fl_Value_Slider(15, 40, 200, 20, "Threshold");
  m_painterlyThresholdSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyThresholdSlider->type(FL_HOR_NICE_SLIDER);
  m_painterlyThresholdSlider->labelfont(FL_COURIER);
  m_painterlyThresholdSlider->labelsize(12);
  m_painterlyThresholdSlider->minimum(0);
  m_painterlyThresholdSlider->maximum(250);
  m_painterlyThresholdSlider->step(1);
  m_painterlyThresholdSlider->value(m_painterlyThreshold);
  m_painterlyThresholdSlider->align(FL_ALIGN_RIGHT);
  m_painterlyThresholdSlider->callback(cb_painterlyThreshold);

  m_painterlyCurvatureSlider =
      new Fl_Value_Slider(15, 65, 200, 20, "Curvature");
  m_painterlyCurvatureSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyCurvatureSlider->type(FL_HOR_NICE_SLIDER);
  m_painterlyCurvatureSlider->labelfont(FL_COURIER);
  m_painterlyCurvatureSlider->labelsize(12);
  m_painterlyCurvatureSlider->minimum(0);
  m_painterlyCurvatureSlider->maximum(1);
  m_painterlyCurvatureSlider->step(0.01);
  m_painterlyCurvatureSlider->value(m_painterlyCurvature);
  m_painterlyCurvatureSlider->align(FL_ALIGN_RIGHT);
  m_painterlyCurvatureSlider->callback(cb_painterlyCurvature);

  m_painterlyBlurSlider = new Fl_Value_Slider(15, 90, 200, 20, "Blur");
  m_painterlyBlurSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyBlurSlider->type(FL_HOR_NICE_SLIDER);
  m_painterlyBlurSlider->labelfont(FL_COURIER);
  m_painterlyBlurSlider->labelsize(12);
  m_painterlyBlurSlider->minimum(0);
  m_painterlyBlurSlider->maximum(1);
  m_painterlyBlurSlider->step(0.01);
  m_painterlyBlurSlider->value(m_painterlyBlur);
  m_painterlyBlurSlider->align(FL_ALIGN_RIGHT);
  m_painterlyBlurSlider->callback(cb_painterlyBlur);

  m_painterlyGridSizeSlider =
      new Fl_Value_Slider(15, 115, 200, 20, "Grid Size");
  m_painterlyGridSizeSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyGridSizeSlider->type(FL_HOR_NICE_SLIDER);
  m_painterlyGridSizeSlider->labelfont(FL_COURIER);
  m_painterlyGridSizeSlider->labelsize(12);
  m_painterlyGridSizeSlider->minimum(0);
  m_painterlyGridSizeSlider->maximum(1);
  m_painterlyGridSizeSlider->step(0.01);
  m_painterlyGridSizeSlider->value(m_painterlyGridSize);
  m_painterlyGridSizeSlider->align(FL_ALIGN_RIGHT);
  m_painterlyGridSizeSlider->callback(cb_painterlyGridSize);

  m_painterlyMinStrokeLengthSlider =
      new Fl_Value_Slider(15, 140, 200, 20, "Min Stroke");
  m_painterlyMinStrokeLengthSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyMinStrokeLengthSlider->type(FL_HOR_NICE_SLIDER);
  m_painterlyMinStrokeLengthSlider->labelfont(FL_COURIER);
  m_painterlyMinStrokeLengthSlider->labelsize(12);
  m_painterlyMinStrokeLengthSlider->minimum(0);
  m_painterlyMinStrokeLengthSlider->maximum(30);
  m_painterlyMinStrokeLengthSlider->step(1);
  m_painterlyMinStrokeLengthSlider->value(m_painterlyMinStrokeLength);
  m_painterlyMinStrokeLengthSlider->align(FL_ALIGN_RIGHT);
  m_painterlyMinStrokeLengthSlider->callback(cb_painterlyMinStrokeLength);

  m_painterlyMaxStrokeLengthSlider =
      new Fl_Value_Slider(15, 165, 200, 20, "Max Stroke");
  m_painterlyMaxStrokeLengthSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyMaxStrokeLengthSlider->type(FL_HOR_NICE_SLIDER);
  m_painterlyMaxStrokeLengthSlider->labelfont(FL_COURIER);
  m_painterlyMaxStrokeLengthSlider->labelsize(12);
  m_painterlyMaxStrokeLengthSlider->minimum(0);
  m_painterlyMaxStrokeLengthSlider->maximum(30);
  m_painterlyMaxStrokeLengthSlider->step(1);
  m_painterlyMaxStrokeLengthSlider->value(m_painterlyMaxStrokeLength);
  m_painterlyMaxStrokeLengthSlider->align(FL_ALIGN_RIGHT);
  m_painterlyMaxStrokeLengthSlider->callback(cb_painterlyMaxStrokeLength);

  m_painterlyAlphaSlider = new Fl_Value_Slider(15, 190, 200, 20, "Alpha");
  m_painterlyAlphaSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyAlphaSlider->type(FL_HOR_NICE_SLIDER);
  m_painterlyAlphaSlider->labelfont(FL_COURIER);
  m_painterlyAlphaSlider->labelsize(12);
  m_painterlyAlphaSlider->minimum(0);
  m_painterlyAlphaSlider->maximum(1);
  m_painterlyAlphaSlider->step(0.01);
  m_painterlyAlphaSlider->value(m_painterlyAlpha);
  m_painterlyAlphaSlider->align(FL_ALIGN_RIGHT);
  m_painterlyAlphaSlider->callback(cb_painterlyAlpha);

  m_painterlyLayersSlider = new Fl_Value_Slider(15, 215, 200, 20, "Layers");
  m_painterlyLayersSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyLayersSlider->type(FL_HOR_NICE_SLIDER);
  m_painterlyLayersSlider->labelfont(FL_COURIER);
  m_painterlyLayersSlider->labelsize(12);
  m_painterlyLayersSlider->minimum(1);
  m_painterlyLayersSlider->maximum(5);
  m_painterlyLayersSlider->step(1);
  m_painterlyLayersSlider->value(m_painterlyLayers);
  m_painterlyLayersSlider->align(FL_ALIGN_RIGHT);
  m_painterlyLayersSlider->callback(cb_painterlyLayers);

  m_painterlyR0LevelSlider = new Fl_Value_Slider(15, 240, 200, 20, "R0 Level");
  m_painterlyR0LevelSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyR0LevelSlider->type(FL_HOR_NICE_SLIDER);
  m_painterlyR0LevelSlider->labelfont(FL_COURIER);
  m_painterlyR0LevelSlider->labelsize(12);
  m_painterlyR0LevelSlider->minimum(0);
  m_painterlyR0LevelSlider->maximum(5);
  m_painterlyR0LevelSlider->step(1);
  m_painterlyR0LevelSlider->value(m_painterlyR0Level);
  m_painterlyR0LevelSlider->align(FL_ALIGN_RIGHT);
  m_painterlyR0LevelSlider->callback(cb_painterlyR0Level);

  m_painterlyJrSlider = new Fl_Value_Slider(300, 40, 25, 90, "Jr");
  m_painterlyJrSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyJrSlider->type(FL_VERT_NICE_SLIDER);
  m_painterlyJrSlider->labelfont(FL_COURIER);
  m_painterlyJrSlider->labelsize(12);
  m_painterlyJrSlider->minimum(0);
  m_painterlyJrSlider->maximum(1);
  m_painterlyJrSlider->step(0.01);
  m_painterlyJrSlider->value(m_painterlyJr);
  m_painterlyJrSlider->align(FL_ALIGN_BOTTOM);
  m_painterlyJrSlider->callback(cb_painterlyJr);

  m_painterlyJgSlider = new Fl_Value_Slider(330, 40, 25, 90, "Jg");
  m_painterlyJgSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyJgSlider->type(FL_VERT_NICE_SLIDER);
  m_painterlyJgSlider->labelfont(FL_COURIER);
  m_painterlyJgSlider->labelsize(12);
  m_painterlyJgSlider->minimum(0);
  m_painterlyJgSlider->maximum(1);
  m_painterlyJgSlider->step(0.01);
  m_painterlyJgSlider->value(m_painterlyJg);
  m_painterlyJgSlider->align(FL_ALIGN_BOTTOM);
  m_painterlyJgSlider->callback(cb_painterlyJg);

  m_painterlyJbSlider = new Fl_Value_Slider(360, 40, 25, 90, "Jb");
  m_painterlyJbSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyJbSlider->type(FL_VERT_NICE_SLIDER);
  m_painterlyJbSlider->labelfont(FL_COURIER);
  m_painterlyJbSlider->labelsize(12);
  m_painterlyJbSlider->minimum(0);
  m_painterlyJbSlider->maximum(1);
  m_painterlyJbSlider->step(0.01);
  m_painterlyJbSlider->value(m_painterlyJb);
  m_painterlyJbSlider->align(FL_ALIGN_BOTTOM);
  m_painterlyJbSlider->callback(cb_painterlyJb);

  m_painterlyJhSlider = new Fl_Value_Slider(300, 155, 25, 90, "Jh");
  m_painterlyJhSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyJhSlider->type(FL_VERT_NICE_SLIDER);
  m_painterlyJhSlider->labelfont(FL_COURIER);
  m_painterlyJhSlider->labelsize(12);
  m_painterlyJhSlider->minimum(0);
  m_painterlyJhSlider->maximum(1);
  m_painterlyJhSlider->step(0.01);
  m_painterlyJhSlider->value(m_painterlyJh);
  m_painterlyJhSlider->align(FL_ALIGN_BOTTOM);
  m_painterlyJhSlider->callback(cb_painterlyJh);

  m_painterlyJsSlider = new Fl_Value_Slider(330, 155, 25, 90, "Js");
  m_painterlyJsSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyJsSlider->type(FL_VERT_NICE_SLIDER);
  m_painterlyJsSlider->labelfont(FL_COURIER);
  m_painterlyJsSlider->labelsize(12);
  m_painterlyJsSlider->minimum(0);
  m_painterlyJsSlider->maximum(1);
  m_painterlyJsSlider->step(0.01);
  m_painterlyJsSlider->value(m_painterlyJs);
  m_painterlyJsSlider->align(FL_ALIGN_BOTTOM);
  m_painterlyJsSlider->callback(cb_painterlyJs);

  m_painterlyJvSlider = new Fl_Value_Slider(360, 155, 25, 90, "Jv");
  m_painterlyJvSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_painterlyJvSlider->type(FL_VERT_NICE_SLIDER);
  m_painterlyJvSlider->labelfont(FL_COURIER);
  m_painterlyJvSlider->labelsize(12);
  m_painterlyJvSlider->minimum(0);
  m_painterlyJvSlider->maximum(1);
  m_painterlyJvSlider->step(0.01);
  m_painterlyJvSlider->value(m_painterlyJv);
  m_painterlyJvSlider->align(FL_ALIGN_BOTTOM);
  m_painterlyJvSlider->callback(cb_painterlyJv);

  // deactivate some painterly dialog
  m_painterlyStrokeChoice->deactivate();
  m_painterlyThresholdSlider->deactivate();
  m_painterlyCurvatureSlider->deactivate();
  m_painterlyBlurSlider->deactivate();
  m_painterlyGridSizeSlider->deactivate();
  m_painterlyMinStrokeLengthSlider->deactivate();
  m_painterlyMaxStrokeLengthSlider->deactivate();
  m_painterlyAlphaSlider->deactivate();
  m_painterlyLayersSlider->deactivate();
  m_painterlyR0LevelSlider->deactivate();
  m_painterlyJrSlider->deactivate();
  m_painterlyJgSlider->deactivate();
  m_painterlyJbSlider->deactivate();
  m_painterlyJhSlider->deactivate();
  m_painterlyJsSlider->deactivate();
  m_painterlyJvSlider->deactivate();

  m_painterlyDialog->end();
}
