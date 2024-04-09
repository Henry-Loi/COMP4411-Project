//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Window.H>

#include <FL/fl_file_chooser.H> // FLTK file chooser

#include "TraceGLWindow.h"

class TraceUI {
public:
  TraceUI();

  // The FLTK widgets
  Fl_Window *m_mainWindow;
  Fl_Menu_Bar *m_menubar;

  Fl_Slider *m_sizeSlider;
  Fl_Slider *m_depthSlider;

  // multi-light sources
  Fl_Slider *m_attenConstantSlider;
  Fl_Slider *m_attenLinearSlider;
  Fl_Slider *m_attenQuadSlider;

  Fl_Slider *m_ambientLightSlider;
  Fl_Slider *m_intensityScaleSlider;
  Fl_Slider *m_distanceScaleSlider;

  // adaptive termination threshold
  Fl_Slider *m_adaptivethreshSlider;
  Fl_Check_Button *m_distAttenOverrideButton;

  Fl_Button *m_renderButton;
  Fl_Button *m_stopButton;

  TraceGLWindow *m_traceGlWindow;

  // member functions
  void show();

  void setRayTracer(RayTracer *tracer);

  int getSize();
  int getDepth();

  double m_nConstant_att;
  double m_nLinear_att;
  double m_nQuad_att;

  double m_nAmbientLightIntensity;
  double m_nIntensityScale;
  double m_nDistanceScale;

  // adaptive termination threshold
  double m_nAdaptiveThresh;
  bool m_nOverrideDistAtten;

private:
  RayTracer *raytracer;

  int m_nSize;
  int m_nDepth;

  // static class members
  static Fl_Menu_Item menuitems[];

  static TraceUI *whoami(Fl_Menu_ *o);

  static void cb_load_scene(Fl_Menu_ *o, void *v);
  static void cb_save_image(Fl_Menu_ *o, void *v);
  static void cb_exit(Fl_Menu_ *o, void *v);
  static void cb_about(Fl_Menu_ *o, void *v);

  static void cb_exit2(Fl_Widget *o, void *v);

  static void cb_sizeSlides(Fl_Widget *o, void *v);
  static void cb_depthSlides(Fl_Widget *o, void *v);

  static void cb_render(Fl_Widget *o, void *v);
  static void cb_stop(Fl_Widget *o, void *v);

  static void TraceUI::cb_attenConstantSlides(Fl_Widget *o, void *v);
  static void TraceUI::cb_attenLinearSlides(Fl_Widget *o, void *v);
  static void TraceUI::cb_attenQuadSlides(Fl_Widget *o, void *v);

  static void cb_ambientLightSlides(Fl_Widget *o, void *v);
  static void cb_intensityScaleSlides(Fl_Widget *o, void *v);
  static void cb_distanceScaleSlides(Fl_Widget *o, void *v);

  // adaptive termination threshold
  static void cb_adaptivethreshSlides(Fl_Widget *o, void *v);
  // overide distanceatten
  static void cb_overideDistanceAttenConst(Fl_Widget *o, void *v);
};

#endif
