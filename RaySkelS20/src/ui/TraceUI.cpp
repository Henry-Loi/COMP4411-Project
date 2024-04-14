//
// TraceUI.h
//
// Handles FLTK integration and other user interface tasks
//
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <FL/fl_ask.h>

#include "../RayTracer.h"
#include "TraceUI.h"

static bool done;

//------------------------------------- Help Functions
//--------------------------------------------
TraceUI *TraceUI::whoami(Fl_Menu_ *o) // from menu item back to UI itself
{
  return ((TraceUI *)(o->parent()->user_data()));
}

//--------------------------------- Callback Functions
//--------------------------------------------
void TraceUI::cb_load_scene(Fl_Menu_ *o, void *v) {
  TraceUI *pUI = whoami(o);

  char *newfile = fl_file_chooser("Open Scene?", "*.ray", NULL);

  if (newfile != NULL) {
    char buf[256];

    if (pUI->raytracer->loadScene(newfile)) {
      sprintf(buf, "Ray <%s>", newfile);
      done = true; // terminate the previous rendering
    } else {
      sprintf(buf, "Ray <Not Loaded>");
    }

    pUI->m_mainWindow->label(buf);
  }
}

void TraceUI::cb_load_background(Fl_Menu_ *o, void *v) {
  TraceUI *pUI = whoami(o);

  char *newfile = fl_file_chooser("Open Background Image?", "*.bmp", NULL);

  if (newfile != NULL) {
    pUI->raytracer->loadBackground(newfile);
  }
}

void TraceUI::cb_save_image(Fl_Menu_ *o, void *v) {
  TraceUI *pUI = whoami(o);

  char *savefile = fl_file_chooser("Save Image?", "*.bmp", "save.bmp");
  if (savefile != NULL) {
    pUI->m_traceGlWindow->saveImage(savefile);
  }
}

void TraceUI::cb_exit(Fl_Menu_ *o, void *v) {
  TraceUI *pUI = whoami(o);

  // terminate the rendering
  done = true;

  pUI->m_traceGlWindow->hide();
  pUI->m_mainWindow->hide();
}

void TraceUI::cb_exit2(Fl_Widget *o, void *v) {
  TraceUI *pUI = (TraceUI *)(o->user_data());

  // terminate the rendering
  done = true;

  pUI->m_traceGlWindow->hide();
  pUI->m_mainWindow->hide();
}

void TraceUI::cb_about(Fl_Menu_ *o, void *v) {
  fl_message("RayTracer Project, FLTK version for CS 341 Spring 2002. Latest "
             "modifications by Jeff Maurer, jmaurer@cs.washington.edu");
}

void TraceUI::cb_sizeSlides(Fl_Widget *o, void *v) {
  TraceUI *pUI = (TraceUI *)(o->user_data());

  pUI->m_nSize = int(((Fl_Slider *)o)->value());
  int height = (int)(pUI->m_nSize / pUI->raytracer->aspectRatio() + 0.5);
  pUI->m_traceGlWindow->resizeWindow(pUI->m_nSize, height);
}

void TraceUI::cb_depthSlides(Fl_Widget *o, void *v) {
  ((TraceUI *)(o->user_data()))->m_nDepth = int(((Fl_Slider *)o)->value());
}

void TraceUI::cb_attenConstantSlides(Fl_Widget *o, void *v) {
  ((TraceUI *)(o->user_data()))->m_nConstant_att = ((Fl_Slider *)o)->value();
}

void TraceUI::cb_attenLinearSlides(Fl_Widget *o, void *v) {
  ((TraceUI *)(o->user_data()))->m_nLinear_att = ((Fl_Slider *)o)->value();
}

void TraceUI::cb_attenQuadSlides(Fl_Widget *o, void *v) {
  ((TraceUI *)(o->user_data()))->m_nQuad_att = ((Fl_Slider *)o)->value();
}

void TraceUI::cb_ambientLightSlides(Fl_Widget *o, void *v) {
  ((TraceUI *)(o->user_data()))->m_nAmbientLightIntensity =
      ((Fl_Slider *)o)->value();
}

void TraceUI::cb_intensityScaleSlides(Fl_Widget *o, void *v) {
  ((TraceUI *)(o->user_data()))->m_nIntensityScale = ((Fl_Slider *)o)->value();
}

void TraceUI::cb_distanceScaleSlides(Fl_Widget *o, void *v) {
  ((TraceUI *)(o->user_data()))->m_nDistanceScale = ((Fl_Slider *)o)->value();
}

void TraceUI::cb_adaptivethreshSlides(Fl_Widget *o, void *v) {
  ((TraceUI *)(o->user_data()))->m_nAdaptiveThresh = ((Fl_Slider *)o)->value();
}

void TraceUI::cb_subsamplepixelSlides(Fl_Widget *o, void *v) {
  ((TraceUI *)(o->user_data()))->m_nSubsamplePixelSize =
      ((Fl_Slider *)o)->value();
}

// Newly added to control distance atten
void TraceUI::cb_overideDistanceAttenConst(Fl_Widget *o, void *v) {
  ((TraceUI *)(o->user_data()))->m_nOverrideDistAtten =
      ((Fl_Check_Button *)o)->value();
}

void TraceUI::cb_subsamplejitterbutton(Fl_Widget *o, void *v) {
  ((TraceUI *)(o->user_data()))->m_nSubsampleJitter =
      ((Fl_Check_Button *)o)->value();
}

void TraceUI::cb_enablebackgroundbutton(Fl_Widget *o, void *v) {
  ((TraceUI *)(o->user_data()))->m_nEnableBackground =
      ((Fl_Check_Button *)o)->value();
}

void TraceUI::cb_render(Fl_Widget *o, void *v) {
  char buffer[256];

  TraceUI *pUI = ((TraceUI *)(o->user_data()));

  if (pUI->raytracer->sceneLoaded()) {
    int width = pUI->getSize();
    int height = (int)(width / pUI->raytracer->aspectRatio() + 0.5);
    pUI->m_traceGlWindow->resizeWindow(width, height);

    pUI->m_traceGlWindow->show();

    pUI->raytracer->traceSetup(width, height);

    // Save the window label
    const char *old_label = pUI->m_traceGlWindow->label();

    // start to render here
    done = false;
    clock_t prev, now;
    prev = clock();

    pUI->m_traceGlWindow->refresh();
    Fl::check();
    Fl::flush();

    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        if (done)
          break;

        // current time
        now = clock();

        // check event every 1/2 second
        if (((double)(now - prev) / CLOCKS_PER_SEC) > 0.5) {
          prev = now;

          if (Fl::ready()) {
            // refresh
            pUI->m_traceGlWindow->refresh();
            // check event
            Fl::check();

            if (Fl::damage()) {
              Fl::flush();
            }
          }
        }

        pUI->raytracer->tracePixel(x, y);
      }
      if (done)
        break;

      // flush when finish a row
      if (Fl::ready()) {
        // refresh
        pUI->m_traceGlWindow->refresh();

        if (Fl::damage()) {
          Fl::flush();
        }
      }
      // update the window label
      sprintf(buffer, "(%d%%) %s", (int)((double)y / (double)height * 100.0),
              old_label);
      pUI->m_traceGlWindow->label(buffer);
    }
    done = true;
    pUI->m_traceGlWindow->refresh();

    // Restore the window label
    pUI->m_traceGlWindow->label(old_label);
  }
}

void TraceUI::cb_stop(Fl_Widget *o, void *v) { done = true; }

void TraceUI::show() { m_mainWindow->show(); }

void TraceUI::setRayTracer(RayTracer *tracer) {
  raytracer = tracer;
  m_traceGlWindow->setRayTracer(tracer);
}

int TraceUI::getSize() { return m_nSize; }

int TraceUI::getDepth() { return m_nDepth; }

// menu definition
Fl_Menu_Item TraceUI::menuitems[] = {
    {"&File", 0, 0, 0, FL_SUBMENU},
    {"&Load Scene...", FL_ALT + 'l', (Fl_Callback *)TraceUI::cb_load_scene},
    {"&Save Image...", FL_ALT + 's', (Fl_Callback *)TraceUI::cb_save_image},
    {"&Load Background Image...", FL_ALT + 'l',
     (Fl_Callback *)TraceUI::cb_load_background},
    {"&Exit", FL_ALT + 'e', (Fl_Callback *)TraceUI::cb_exit},
    {0},

    {"&Help", 0, 0, 0, FL_SUBMENU},
    {"&About", FL_ALT + 'a', (Fl_Callback *)TraceUI::cb_about},
    {0},

    {0}};

TraceUI::TraceUI() {
  // init.
  m_nDepth = 0;
  m_nSize = 150;

  m_nAmbientLightIntensity = 0.2;
  m_nIntensityScale = 1;
  m_nDistanceScale = 1.87;

  m_nConstant_att = 0.25;
  m_nLinear_att = 0.25;
  m_nQuad_att = 0.5;

  m_nSubsamplePixelSize = 0;
  m_nSubsampleJitter = false;

  m_nEnableBackground = false;

  m_mainWindow = new Fl_Window(100, 40, 400, 400, "Ray <Not Loaded>");
  m_mainWindow->user_data(
      (void *)(this)); // record self to be used by static callback functions
  // install menu bar
  m_menubar = new Fl_Menu_Bar(0, 0, 400, 25);
  m_menubar->menu(menuitems);

  // install slider depth
  m_depthSlider = new Fl_Value_Slider(10, 30, 180, 20, "Depth");
  m_depthSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_depthSlider->type(FL_HOR_NICE_SLIDER);
  m_depthSlider->labelfont(FL_COURIER);
  m_depthSlider->labelsize(12);
  m_depthSlider->minimum(0);
  m_depthSlider->maximum(10);
  m_depthSlider->step(1);
  m_depthSlider->value(m_nDepth);
  m_depthSlider->align(FL_ALIGN_RIGHT);
  m_depthSlider->callback(cb_depthSlides);

  // install slider size
  m_sizeSlider = new Fl_Value_Slider(10, 55, 180, 20, "Size");
  m_sizeSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_sizeSlider->type(FL_HOR_NICE_SLIDER);
  m_sizeSlider->labelfont(FL_COURIER);
  m_sizeSlider->labelsize(12);
  m_sizeSlider->minimum(64);
  m_sizeSlider->maximum(512);
  m_sizeSlider->step(1);
  m_sizeSlider->value(m_nSize);
  m_sizeSlider->align(FL_ALIGN_RIGHT);
  m_sizeSlider->callback(cb_sizeSlides);

  m_renderButton = new Fl_Button(290, 27, 90, 25, "&Render");
  m_renderButton->user_data((void *)(this));
  m_renderButton->callback(cb_render);

  m_stopButton = new Fl_Button(290, 55, 90, 25, "&Stop");
  m_stopButton->user_data((void *)(this));
  m_stopButton->callback(cb_stop);

  m_attenConstantSlider =
      new Fl_Value_Slider(10, 80, 180, 20, "Attenuation Constant");
  m_attenConstantSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_attenConstantSlider->type(FL_HOR_NICE_SLIDER);
  m_attenConstantSlider->labelfont(FL_COURIER);
  m_attenConstantSlider->labelsize(12);
  m_attenConstantSlider->minimum(0);
  m_attenConstantSlider->maximum(1);
  m_attenConstantSlider->step(0.01);
  m_attenConstantSlider->value(0.25);
  m_attenConstantSlider->align(FL_ALIGN_RIGHT);
  m_attenConstantSlider->callback(cb_attenConstantSlides);

  m_attenLinearSlider =
      new Fl_Value_Slider(10, 105, 180, 20, "Attenuation Linear");
  m_attenLinearSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_attenLinearSlider->type(FL_HOR_NICE_SLIDER);
  m_attenLinearSlider->labelfont(FL_COURIER);
  m_attenLinearSlider->labelsize(12);
  m_attenLinearSlider->minimum(0);
  m_attenLinearSlider->maximum(1);
  m_attenLinearSlider->step(0.01);
  m_attenLinearSlider->value(0.25);
  m_attenLinearSlider->align(FL_ALIGN_RIGHT);
  m_attenLinearSlider->callback(cb_attenLinearSlides);

  m_attenQuadSlider =
      new Fl_Value_Slider(10, 130, 180, 20, "Attenuation Quadratic");
  m_attenQuadSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_attenQuadSlider->type(FL_HOR_NICE_SLIDER);
  m_attenQuadSlider->labelfont(FL_COURIER);
  m_attenQuadSlider->labelsize(12);
  m_attenQuadSlider->minimum(0);
  m_attenQuadSlider->maximum(1);
  m_attenQuadSlider->step(0.01);
  m_attenQuadSlider->value(0.5);
  m_attenQuadSlider->align(FL_ALIGN_RIGHT);
  m_attenQuadSlider->callback(cb_attenQuadSlides);

  m_ambientLightSlider = new Fl_Value_Slider(10, 155, 180, 20, "Ambient Light");
  m_ambientLightSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_ambientLightSlider->type(FL_HOR_NICE_SLIDER);
  m_ambientLightSlider->labelfont(FL_COURIER);
  m_ambientLightSlider->labelsize(12);
  m_ambientLightSlider->minimum(0);
  m_ambientLightSlider->maximum(1);
  m_ambientLightSlider->step(0.01);
  m_ambientLightSlider->value(0.2);
  m_ambientLightSlider->align(FL_ALIGN_RIGHT);
  m_ambientLightSlider->callback(cb_ambientLightSlides);

  m_intensityScaleSlider =
      new Fl_Value_Slider(10, 180, 180, 20, "Intensity Scale");
  m_intensityScaleSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_intensityScaleSlider->type(FL_HOR_NICE_SLIDER);
  m_intensityScaleSlider->labelfont(FL_COURIER);
  m_intensityScaleSlider->labelsize(12);
  m_intensityScaleSlider->minimum(1);
  m_intensityScaleSlider->maximum(10);
  m_intensityScaleSlider->step(1);
  m_intensityScaleSlider->value(1);
  m_intensityScaleSlider->align(FL_ALIGN_RIGHT);
  m_intensityScaleSlider->callback(cb_intensityScaleSlides);

  m_distanceScaleSlider =
      new Fl_Value_Slider(10, 205, 180, 20, "Distance Scale");
  m_distanceScaleSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_distanceScaleSlider->type(FL_HOR_NICE_SLIDER);
  m_distanceScaleSlider->labelfont(FL_COURIER);
  m_distanceScaleSlider->labelsize(12);
  m_distanceScaleSlider->minimum(-0.99);
  m_distanceScaleSlider->maximum(3);
  m_distanceScaleSlider->step(0.01);
  m_distanceScaleSlider->value(1.87);
  m_distanceScaleSlider->align(FL_ALIGN_RIGHT);
  m_distanceScaleSlider->callback(cb_distanceScaleSlides);

  m_adaptivethreshSlider =
      new Fl_Value_Slider(10, 230, 180, 20, "Adaptive Termination Threshold");
  m_adaptivethreshSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_adaptivethreshSlider->type(FL_HOR_NICE_SLIDER);
  m_adaptivethreshSlider->labelfont(FL_COURIER);
  m_adaptivethreshSlider->labelsize(12);
  m_adaptivethreshSlider->minimum(0);
  m_adaptivethreshSlider->maximum(5);
  m_adaptivethreshSlider->step(0.01);
  m_adaptivethreshSlider->value(0.01);
  m_adaptivethreshSlider->align(FL_ALIGN_RIGHT);
  m_adaptivethreshSlider->callback(cb_adaptivethreshSlides);

  m_distAttenOverrideButton = new Fl_Check_Button(
      10, 255, 20, 20, "Overide Distance Attenuation Constant");
  m_distAttenOverrideButton->user_data((void *)(this));
  m_distAttenOverrideButton->callback(cb_overideDistanceAttenConst);

  m_SubSameplePixelSlider =
      new Fl_Value_Slider(10, 280, 180, 20, "Subsample Pixel Size");
  m_SubSameplePixelSlider->user_data(
      (void *)(this)); // record self to be used by static callback functions
  m_SubSameplePixelSlider->type(FL_HOR_NICE_SLIDER);
  m_SubSameplePixelSlider->labelfont(FL_COURIER);
  m_SubSameplePixelSlider->labelsize(12);
  m_SubSameplePixelSlider->minimum(0);
  m_SubSameplePixelSlider->maximum(5);
  m_SubSameplePixelSlider->step(1);
  m_SubSameplePixelSlider->value(0);
  m_SubSameplePixelSlider->align(FL_ALIGN_RIGHT);
  m_SubSameplePixelSlider->callback(cb_subsamplepixelSlides);

  m_SubSampleJitterButton =
      new Fl_Check_Button(10, 305, 20, 20, "Enable Subsample Jitter");
  m_SubSampleJitterButton->user_data((void *)(this));
  m_SubSampleJitterButton->callback(cb_subsamplejitterbutton);

  m_enableBackgroundButton =
      new Fl_Check_Button(10, 330, 20, 20, "Enable External Background");
  m_enableBackgroundButton->user_data((void *)(this));
  m_enableBackgroundButton->callback(cb_enablebackgroundbutton);

  m_mainWindow->callback(cb_exit2);
  m_mainWindow->when(FL_HIDE);

  m_mainWindow->end();

  // image view
  m_traceGlWindow =
      new TraceGLWindow(100, 150, m_nSize, m_nSize, "Rendered Image");
  m_traceGlWindow->end();
  m_traceGlWindow->resizable(m_traceGlWindow);
}