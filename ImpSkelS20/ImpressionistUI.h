//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H> // FLTK file chooser
#include <sys/stat.h>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Color_Chooser.H>

#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"

#include "ImpBrush.h"

class ImpressionistUI {
public:
  ImpressionistUI();

  // The FLTK widgets
  Fl_Window *m_mainWindow;
  Fl_Menu_Bar *m_menubar;

  PaintView *m_paintView;
  OriginalView *m_origView;

  // for brush dialog
  Fl_Window *m_brushDialog;
  Fl_Window* m_colorSelectionDialog;

  Fl_Choice *m_BrushTypeChoice;
  Fl_Choice *m_StrokeDirectionChoice;

  Fl_Slider *m_BrushSizeSlider;
  Fl_Slider *m_LineWidthSlider;
  Fl_Slider *m_LineAngleSlider;
  Fl_Slider *m_AlphaSlider;
  Fl_Slider *m_SpacingSlider;
  Fl_Slider *m_EdgeThresholdSlider;

  Fl_Button *m_ClearCanvasButton;
  Fl_Button *m_PaintButton;
  Fl_Button *m_DoItButton;

  Fl_Light_Button *m_EdgeClippingLightButton;
  Fl_Light_Button *m_AnotherGradientLightButton;
  Fl_Light_Button *m_SizeRandLightButton;

  Fl_Color_Chooser* Color_Selection;

  // Member functions
  void setDocument(ImpressionistDoc *doc);
  ImpressionistDoc *getDocument();
  PaintView *getPaintView();

  void show();
  void resize_windows(int w, int h);

  // Interface to get attribute

  int getSize(); // get the UI size
  int getLineWidth();
  int getLineAngle();
  float getAlpha();
  int getSpacing();
  int getEdgeThreshold();

  float getR();
  float getB();
  float getG();

  void setSize(int size);
  void setLineAngle(int angle);
  void brush_dialog_value_init();
  void color_Selection_init();
  bool isEdgeClipping;
  bool isAnotherGradient;
  bool isSizeRand;

  int m_tarLineAngle; // for right click

private:
  ImpressionistDoc
      *m_pDoc; // pointer to document to communicate with the document

  // All attributes here
  int m_nSize;
  int m_lineWidth;
  int m_lineAngle;
  float m_alpha;
  int m_spacing;
  int m_edgeThreshold;
  float m_color1;
  float m_color2;
  float m_color3;

  // Static class members
  static Fl_Menu_Item menuitems[];
  static Fl_Menu_Item brushTypeMenu[NUM_BRUSH_TYPE + 1];
  static Fl_Menu_Item strokeDirectionMenu[NUM_STROKE_DIRECTION + 1];

  static ImpressionistUI *whoami(Fl_Menu_ *o);

  // All callbacks here.  Callbacks are declared
  // static
  static void cb_load_image(Fl_Menu_ *o, void *v);
  static void cb_load_mural_image(Fl_Menu_ *o, void *v);
  static void cb_save_image(Fl_Menu_ *o, void *v);
  static void cb_brushes(Fl_Menu_ *o, void *v);
  static void cb_clear_canvas(Fl_Menu_ *o, void *v);
  static void cb_exit(Fl_Menu_ *o, void *v);
  static void cb_about(Fl_Menu_ *o, void *v);
  static void cb_brushChoice(Fl_Widget *o, void *v);
  static void cb_clear_canvas_button(Fl_Widget *o, void *v);
  static void cb_paint_button(Fl_Widget *o, void *v);
  static void cb_do_it_button(Fl_Widget *o, void *v);
  static void cb_colorSelection(Fl_Menu_ * o, void* v);
  static void cb_ManualColor(Fl_Widget* o, void* v);

  static void cb_strokeDirectionChoice(Fl_Widget *o, void *v);

  static void cb_sizeSlides(Fl_Widget *o, void *v);
  static void cb_lineWidth(Fl_Widget *o, void *v);
  static void cb_lineAngle(Fl_Widget *o, void *v);
  static void cb_alpha(Fl_Widget *o, void *v);
  static void cb_spacing(Fl_Widget *o, void *v);
  static void cb_edgeThreshold(Fl_Widget *o, void *v);

  static void cb_EdgeClippingLightButton(Fl_Widget *o, void *v);
  static void cb_AnotherGradientLightButton(Fl_Widget *o, void *v);
  static void cb_sizeRandLightButton(Fl_Widget *o, void *v);

};

#endif
