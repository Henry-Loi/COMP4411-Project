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
#include <vector>

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
  Fl_Button *m_KernelApplyButton;

  Fl_Check_Button *m_KernelNormalizeButton;

  Fl_Light_Button *m_EdgeClippingLightButton;
  Fl_Light_Button *m_AnotherGradientLightButton;
  Fl_Light_Button *m_SizeRandLightButton;

  Fl_Input *m_KernelInput;
  // brush dialog ends here

  // for painterly dialog
  Fl_Window *m_painterlyDialog;

  Fl_Choice *m_painterlyStyleTypeChoice;
  Fl_Choice *m_painterlyStrokeChoice;

  Fl_Button *m_painterlyPaintButton;

  Fl_Slider *m_painterlyThresholdSlider;
  Fl_Slider *m_painterlyCurvatureSlider;
  Fl_Slider *m_painterlyBlurSlider;
  Fl_Slider *m_painterlyGridSizeSlider;
  Fl_Slider *m_painterlyMinStrokeLengthSlider;
  Fl_Slider *m_painterlyMaxStrokeLengthSlider;
  Fl_Slider *m_painterlyAlphaSlider;
  Fl_Slider *m_painterlyLayersSlider;
  Fl_Slider *m_painterlyR0LevelSlider;

  Fl_Slider *m_painterlyJrSlider;
  Fl_Slider *m_painterlyJgSlider;
  Fl_Slider *m_painterlyJbSlider;
  Fl_Slider *m_painterlyJhSlider;
  Fl_Slider *m_painterlyJsSlider;
  Fl_Slider *m_painterlyJvSlider;

  // painterly dialog ends here

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

  void setSize(int size);
  void setLineAngle(int angle);

  bool isEdgeClipping;
  bool isAnotherGradient;
  bool isSizeRand;

  // custom kernel
  char *m_KernelStr;
  std::vector<std::vector<float>> matrix_kernel =
      std::vector<std::vector<float>>();
  bool get_IsNormalizedKernel();

  int m_tarLineAngle; // for right click

  // interface to get painterly attributes
  int getPainterlyStyle();
  int getPainterlyStroke();

  int getPainterlyThreshold();
  float getPainterlyCurvature();
  float getPainterlyBlur();
  float getPainterlyGridSize();
  int getPainterlyMinStrokeLength();
  int getPainterlyMaxStrokeLength();
  float getPainterlyAlpha();
  int getPainterlyLayers();
  int getPainterlyR0Level();

  float getPainterlyJr();
  float getPainterlyJg();
  float getPainterlyJb();
  float getPainterlyJh();
  float getPainterlyJs();
  float getPainterlyJv();

private:
  ImpressionistDoc
      *m_pDoc; // pointer to document to communicate with the document

  static ImpressionistUI *whoami(Fl_Menu_ *o);

  // All basic attributes here
  int m_nSize;
  int m_lineWidth;
  int m_lineAngle;
  float m_alpha;
  int m_spacing;
  int m_edgeThreshold;

  void brush_dialog_value_init();

  // Static class members
  static Fl_Menu_Item menuitems[];
  static Fl_Menu_Item brushTypeMenu[NUM_BRUSH_TYPE + 1];
  static Fl_Menu_Item strokeDirectionMenu[NUM_STROKE_DIRECTION + 1];

  // All basic callbacks here.  Callbacks are declared
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

  // custom kernel
  bool m_IsNormalizedKernel;
  static void cb_KernelInput(Fl_Widget *o, void *v);
  static void cb_KernelApplyButton(Fl_Widget *o, void *v);
  static void cb_KernelNormalizeButton(Fl_Widget *o, void *v);
  bool ImpressionistUI::parseKernel(); // helper function

  // all painterly attributes
  static Fl_Menu_Item painterlyStyleMenu[NUM_OF_PAINTERLY_STYLE + 1];
  static Fl_Menu_Item painterlyStrokeMenu[NUM_OF_PAINTERLY_STROKE + 1];

  int m_painterlyStyle;
  int m_painterlyStroke;

  int m_painterlyThreshold;
  float m_painterlyCurvature;
  float m_painterlyBlur;
  float m_painterlyGridSize;
  int m_painterlyMinStrokeLength;
  int m_painterlyMaxStrokeLength;
  float m_painterlyAlpha;
  int m_painterlyLayers;
  int m_painterlyR0Level;

  float m_painterlyJr;
  float m_painterlyJg;
  float m_painterlyJb;
  float m_painterlyJh;
  float m_painterlyJs;
  float m_painterlyJv;

  void painterly_dialog_value_init();

  // all painterly callbacks
  static void cb_painterly(Fl_Menu_ *o, void *v);
  static void cb_painterlyStyleChoice(Fl_Widget *o, void *v);
  static void cb_painterlyStrokeChoice(Fl_Widget *o, void *v);
  static void cb_painterlyPaintButton(Fl_Widget *o, void *v);

  static void cb_painterlyThreshold(Fl_Widget *o, void *v);
  static void cb_painterlyCurvature(Fl_Widget *o, void *v);
  static void cb_painterlyBlur(Fl_Widget *o, void *v);
  static void cb_painterlyGridSize(Fl_Widget *o, void *v);
  static void cb_painterlyMinStrokeLength(Fl_Widget *o, void *v);
  static void cb_painterlyMaxStrokeLength(Fl_Widget *o, void *v);
  static void cb_painterlyAlpha(Fl_Widget *o, void *v);
  static void cb_painterlyLayers(Fl_Widget *o, void *v);
  static void cb_painterlyR0Level(Fl_Widget *o, void *v);

  static void cb_painterlyJr(Fl_Widget *o, void *v);
  static void cb_painterlyJg(Fl_Widget *o, void *v);
  static void cb_painterlyJb(Fl_Widget *o, void *v);
  static void cb_painterlyJh(Fl_Widget *o, void *v);
  static void cb_painterlyJs(Fl_Widget *o, void *v);
  static void cb_painterlyJv(Fl_Widget *o, void *v);
};

#endif
