#ifdef WIN32
#pragma warning(disable : 4786)
#endif

#include <cmath>
#include <cstring>
#include <fstream>
#include <strstream>

#include <vector>

#include "../SceneObjects//Hyperbolic.h"
#include "../SceneObjects/Box.h"
#include "../SceneObjects/Cone.h"
#include "../SceneObjects/Cylinder.h"
#include "../SceneObjects/Sphere.h"
#include "../SceneObjects/Square.h"
#include "../SceneObjects/trimesh.h"
#include "../scene/light.h"
#include "../scene/particle_system.h"
#include "../scene/scene.h"
#include "../ui/TraceUI.h"
#include "bitmap.h"
#include "parse.h"
#include "read.h"


typedef map<string, Material *> mmap;

static void processObject(Obj *obj, Scene *scene, mmap &materials);
static Obj *getColorField(Obj *obj);
static Obj *getField(Obj *obj, const string &name);
static bool hasField(Obj *obj, const string &name);
static vec3f tupleToVec(Obj *obj);
static void processGeometry(string name, Obj *child, Scene *scene,
                            const mmap &materials, TransformNode *transform);
static void processTrimesh(string name, Obj *child, Scene *scene,
                           const mmap &materials, TransformNode *transform);
static void processCamera(Obj *child, Scene *scene);
static Material *getMaterial(Obj *child, const mmap &bindings);
static Material *processMaterial(Obj *child, mmap *bindings = NULL);
static void verifyTuple(const mytuple &tup, size_t size);
static vec3f readColor(Obj *child);
extern TraceUI *traceUI;

Scene *readScene(const string &filename) {
  ifstream ifs(filename.c_str());
  if (!ifs) {
    cerr << "Error: couldn't read scene file " << filename << endl;
    return NULL;
  }

  try {
    return readScene(ifs);
  } catch (ParseError &pe) {
    cout << "Parse error: " << pe << endl;
    return NULL;
  }
}

Scene *readScene(istream &is) {
  Scene *ret = new Scene;

  // Extract the file header
  static const int MAXNAME = 80;
  char buf[MAXNAME];
  int ct = 0;

  while (ct < MAXNAME - 1) {
    char c;
    is.get(c);
    if (c == ' ' || c == '\t' || c == '\n') {
      break;
    }
    buf[ct++] = c;
  }

  buf[ct] = '\0';

  if (strcmp(buf, "SBT-raytracer")) {
    throw ParseError(string("Input is not an SBT input file."));
  }

  float version;
  is >> version;

  if (version != 1.0) {
    ostrstream oss;
    oss << "Input is version " << version << ", need version 1.0" << ends;

    throw ParseError(string(oss.str()));
  }

  // vector<Obj*> result;
  mmap materials;

  while (true) {
    Obj *cur = readFile(is);
    if (!cur) {
      break;
    }

    processObject(cur, ret, materials);
    delete cur;
  }

  return ret;
}

#include "../fileio/bitmap.h"
Scene *loadHeightField(char *iname) {

  // try to open the image to read
  unsigned char *data;
  int width, height;

  if ((data = readBMP(iname, width, height)) == NULL) {
    return NULL;
  }

  // create a new scene
  Scene *scene = new Scene();
  Material *mat = new Material();

  Trimesh *tmesh = new Trimesh(scene, mat, &scene->transformRoot);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int pos = y * width + x;
      unsigned char pixel[3];
      memcpy(pixel, data + pos * 3, 3);
      double height = double(pixel[0] + pixel[1] + pixel[2]) / 3 / 128;
      vec3f point(x, y, height);
      tmesh->addVertex(point);
      if (x > 0 && y > 0) { // link the points
        tmesh->addFace(pos, pos - 1, pos - 1 - width);
        tmesh->addFace(pos, pos - 1 - width, pos - width);
      }
    }
  }

  char *error;
  if (error = tmesh->doubleCheck())
    throw ParseError(error);

  scene->add(tmesh);

  // add a pointlight
  PointLight *point_light =
      new PointLight(scene, vec3f(width, height, 10), vec3f(1.0, 1.0, 1.0),
                     vec3f(0.0, 0.0, 0.0));
  scene->add(point_light);

  // set the camerea
  // TODO: calculate the correct viewing distance;
  vec3f map_center((double)width / 2 - 0.5, (double)height / 2 - 0.5, 0.5);
  double camera_distance = (double)width + 3.0;
  vec3f camera_pos(0, -camera_distance, 2 * camera_distance);
  camera_pos += map_center;
  scene->getCamera()->setEye(camera_pos);
  scene->getCamera()->setLook((map_center - camera_pos).normalize(),
                              vec3f(0, 0, 1).normalize());

  return scene;
}

// Find a color field inside some object.  Now, I recognize that not
// everyone speaks the Queen's English, so I allow both spellings of
// color.  If you're composing scenes, you don't need to worry about
// spelling :^)
static Obj *getColorField(Obj *obj) {
  if (obj == NULL) {
    throw ParseError(string("Object contains no color field"));
  }

  const dict &d = obj->getDict();
  dict::const_iterator i;
  if ((i = d.find("color")) != d.end()) {
    return (*i).second;
  }
  if ((i = d.find("colour")) != d.end()) {
    return (*i).second;
  }

  throw ParseError(string("Object contains no color field"));
}

// Get a named field out of a dictionary object.  Throw a parse
// error if the field isn't there.
static Obj *getField(Obj *obj, const string &name) {
  if (obj == NULL) {
    throw ParseError(string("Object contains no field named \"") + name + "\"");
  }

  const dict &d = obj->getDict();
  dict::const_iterator i;
  if ((i = d.find(name)) != d.end()) {
    return (*i).second;
  }

  throw ParseError(string("Object contains no field named \"") + name + "\"");
}

// Determine if the given dictionary object has the named field.
static bool hasField(Obj *obj, const string &name) {
  if (obj == NULL) {
    return false;
  }

  const dict &d = obj->getDict();
  return d.find(name) != d.end();
}

// Turn a parsed tuple into a 3D point.
static vec3f tupleToVec(Obj *obj) {
  const mytuple &t = obj->getTuple();
  verifyTuple(t, 3);
  return vec3f(t[0]->getScalar(), t[1]->getScalar(), t[2]->getScalar());
}

static void processGeometry(Obj *obj, Scene *scene, const mmap &materials,
                            TransformNode *transform) {
  string name;
  Obj *child;

  if (obj->getTypeName() == "id") {
    name = obj->getID();
    child = NULL;
  } else if (obj->getTypeName() == "named") {
    name = obj->getName();
    child = obj->getChild();
  } else {
    ostrstream oss;
    oss << "Unknown input object ";
    obj->printOn(oss);

    throw ParseError(string(oss.str()));
  }

  processGeometry(name, child, scene, materials, transform);
}

// Extract the named scalar field into ret, if it exists.
static bool maybeExtractField(Obj *child, const string &name, double &ret) {
  const dict &d = child->getDict();
  if (hasField(child, name)) {
    ret = getField(child, name)->getScalar();
    return true;
  }

  return false;
}

// Extract the named boolean field into ret, if it exists.
static bool maybeExtractField(Obj *child, const string &name, bool &ret) {
  const dict &d = child->getDict();
  if (hasField(child, name)) {
    ret = getField(child, name)->getBoolean();
    return true;
  }

  return false;
}

// Check that a tuple has the expected size.
static void verifyTuple(const mytuple &tup, size_t size) {
  if (tup.size() != size) {
    ostrstream oss;
    oss << "Bad tuple size " << tup.size() << ", expected " << size;

    throw ParseError(string(oss.str()));
  }
}

static void processGeometry(string name, Obj *child, Scene *scene,
                            const mmap &materials, TransformNode *transform) {
  if (name == "translate") {
    const mytuple &tup = child->getTuple();
    verifyTuple(tup, 4);
    processGeometry(
        tup[3], scene, materials,
        transform->createChild(mat4f::translate(vec3f(
            tup[0]->getScalar(), tup[1]->getScalar(), tup[2]->getScalar()))));
  } else if (name == "rotate") {
    const mytuple &tup = child->getTuple();
    verifyTuple(tup, 5);
    processGeometry(tup[4], scene, materials,
                    transform->createChild(mat4f::rotate(
                        vec3f(tup[0]->getScalar(), tup[1]->getScalar(),
                              tup[2]->getScalar()),
                        tup[3]->getScalar())));
  } else if (name == "scale") {
    const mytuple &tup = child->getTuple();
    if (tup.size() == 2) {
      double sc = tup[0]->getScalar();
      processGeometry(tup[1], scene, materials,
                      transform->createChild(mat4f::scale(vec3f(sc, sc, sc))));
    } else {
      verifyTuple(tup, 4);
      processGeometry(
          tup[3], scene, materials,
          transform->createChild(mat4f::scale(vec3f(
              tup[0]->getScalar(), tup[1]->getScalar(), tup[2]->getScalar()))));
    }
  } else if (name == "transform") {
    const mytuple &tup = child->getTuple();
    verifyTuple(tup, 5);

    const mytuple &l1 = tup[0]->getTuple();
    const mytuple &l2 = tup[1]->getTuple();
    const mytuple &l3 = tup[2]->getTuple();
    const mytuple &l4 = tup[3]->getTuple();
    verifyTuple(l1, 4);
    verifyTuple(l2, 4);
    verifyTuple(l3, 4);
    verifyTuple(l4, 4);

    processGeometry(tup[4], scene, materials,
                    transform->createChild(
                        mat4f(vec4f(l1[0]->getScalar(), l1[1]->getScalar(),
                                    l1[2]->getScalar(), l1[3]->getScalar()),
                              vec4f(l2[0]->getScalar(), l2[1]->getScalar(),
                                    l2[2]->getScalar(), l2[3]->getScalar()),
                              vec4f(l3[0]->getScalar(), l3[1]->getScalar(),
                                    l3[2]->getScalar(), l3[3]->getScalar()),
                              vec4f(l4[0]->getScalar(), l4[1]->getScalar(),
                                    l4[2]->getScalar(), l4[3]->getScalar()))));
  } else if (name == "trimesh" ||
             name == "polymesh") { // 'polymesh' is for backwards compatibility
    processTrimesh(name, child, scene, materials, transform);
  } else {
    SceneObject *obj = NULL;
    Material *mat;

    // if( hasField( child, "material" ) )
    mat = getMaterial(getField(child, "material"), materials);
    // else
    //     mat = new Material();

    if (name == "sphere") {
      obj = new Sphere(scene, mat);
    } else if (name == "box") {
      obj = new Box(scene, mat);
    } else if (name == "cylinder") {
      bool capped = true;
      maybeExtractField(child, "capped", capped);
      obj = new Cylinder(scene, mat, capped);
    } else if (name == "cone") {
      double height = 1.0;
      double bottom_radius = 1.0;
      double top_radius = 0.0;
      bool capped = true;

      maybeExtractField(child, "height", height);
      maybeExtractField(child, "bottom_radius", bottom_radius);
      maybeExtractField(child, "top_radius", top_radius);
      maybeExtractField(child, "capped", capped);

      obj = new Cone(scene, mat, height, bottom_radius, top_radius, capped);
    } else if (name == "square") {
      obj = new Square(scene, mat);
    } else if (name == "meteorite") {
      obj = new ParticleSystem(scene, mat);

      static_cast<ParticleSystem *>(obj)->dir =
          tupleToVec(getField(child, "direction"));
      static_cast<ParticleSystem *>(obj)->initColorMin =
          tupleToVec(getField(child, "startColorMin"));
      static_cast<ParticleSystem *>(obj)->initColorMax =
          tupleToVec(getField(child, "startColorMax"));
      static_cast<ParticleSystem *>(obj)->endColorMin =
          tupleToVec(getField(child, "endColorMin"));
      static_cast<ParticleSystem *>(obj)->endColorMax =
          tupleToVec(getField(child, "endColorMax"));

      maybeExtractField(child, "force",
                        static_cast<ParticleSystem *>(obj)->force);
      maybeExtractField(child, "minLife",
                        static_cast<ParticleSystem *>(obj)->lifeMin);
      maybeExtractField(child, "maxLife",
                        static_cast<ParticleSystem *>(obj)->lifeMax);
      maybeExtractField(child, "numEmit",
                        static_cast<ParticleSystem *>(obj)->numEmit);
      maybeExtractField(child, "maxNumParticles",
                        static_cast<ParticleSystem *>(obj)->num_of_particle);
      maybeExtractField(child, "minSpeed",
                        static_cast<ParticleSystem *>(obj)->speedMin);
      maybeExtractField(child, "maxSpeed",
                        static_cast<ParticleSystem *>(obj)->speedMax);

      static_cast<ParticleSystem *>(obj)->init();
    } else if (name == "hyperbolic") {
      std::cout << "readed Hyperbolic" << std::endl;
      obj = new Hyperbolic(scene, mat);
    }

    obj->setTransform(transform);
    scene->add(obj);
  }
}

static void processTrimesh(string name, Obj *child, Scene *scene,
                           const mmap &materials, TransformNode *transform) {
  Material *mat;

  if (hasField(child, "material"))
    mat = getMaterial(getField(child, "material"), materials);
  else
    mat = new Material();

  Trimesh *tmesh = new Trimesh(scene, mat, transform);

  const mytuple &points = getField(child, "points")->getTuple();
  for (mytuple::const_iterator pi = points.begin(); pi != points.end(); ++pi)
    tmesh->addVertex(tupleToVec(*pi));

  const mytuple &faces = getField(child, "faces")->getTuple();
  for (mytuple::const_iterator fi = faces.begin(); fi != faces.end(); ++fi) {
    const mytuple &pointids = (*fi)->getTuple();

    // triangulate here and now.  assume the poly is
    // concave and we can triangulate using an arbitrary fan
    if (pointids.size() < 3)
      throw ParseError("Faces must have at least 3 vertices.");

    mytuple::const_iterator i = pointids.begin();
    int a = (int)(*i++)->getScalar();
    int b = (int)(*i++)->getScalar();
    while (i != pointids.end()) {
      int c = (int)(*i++)->getScalar();
      if (!tmesh->addFace(a, b, c))
        throw ParseError("Bad face in trimesh.");
      b = c;
    }
  }

  bool generateNormals = false;
  maybeExtractField(child, "gennormals", generateNormals);
  if (generateNormals)
    tmesh->generateNormals();

  if (hasField(child, "materials")) {
    const mytuple &mats = getField(child, "materials")->getTuple();
    for (mytuple::const_iterator mi = mats.begin(); mi != mats.end(); ++mi)
      tmesh->addMaterial(getMaterial(*mi, materials));
  }
  if (hasField(child, "normals")) {
    const mytuple &norms = getField(child, "normals")->getTuple();
    for (mytuple::const_iterator ni = norms.begin(); ni != norms.end(); ++ni)
      tmesh->addNormal(tupleToVec(*ni));
  }

  char *error;
  if (error = tmesh->doubleCheck())
    throw ParseError(error);

  scene->add(tmesh);
}

static Material *getMaterial(Obj *child, const mmap &bindings) {
  string tfield = child->getTypeName();
  if (tfield == "id") {
    mmap::const_iterator i = bindings.find(child->getID());
    if (i != bindings.end()) {
      return (*i).second;
    }
  } else if (tfield == "string") {
    mmap::const_iterator i = bindings.find(child->getString());
    if (i != bindings.end()) {
      return (*i).second;
    }
  }
  // Don't allow binding.
  return processMaterial(child);
}

static Material *processMaterial(Obj *child, mmap *bindings)
// Generate a material from a parse sub-tree
//
// child   - root of parse tree
// mmap    - bindings of names to materials (if non-null)
// defmat  - material to start with (if non-null)
{
  Material *mat;
  mat = new Material();

  if (hasField(child, "emissive")) {
    mat->ke = tupleToVec(getField(child, "emissive"));
  }
  if (hasField(child, "ambient")) {
    mat->ka = tupleToVec(getField(child, "ambient"));
  }
  if (hasField(child, "specular")) {
    mat->ks = tupleToVec(getField(child, "specular"));
  }
  if (hasField(child, "diffuse")) {
    // load bitmap
    vec3f diffuse(0.0, 0.0, 0.0);
    std::cout << "diffuse" << std::endl;
    std::cout << child->getTypeName() << std::endl;
    Obj *di = getField(child, "diffuse");
    std::cout << di->getTypeName() << std::endl;

    if (di->getTypeName() == "tuple") {
      std::cout << "no_map" << std::endl;
      diffuse = tupleToVec(getField(child, "diffuse"));
    } else {
      std::cout << "run map" << std::endl;
      string str = getField(child, "diffuse")->getChild()->getString();
      std::cout << str << std::endl;
      char *charPtr = new char[str.size() + 1]; // +1 for null-terminator
      strcpy(charPtr, str.data());
      traceUI->texMap->loadTexture(charPtr);
    }
    mat->kd = diffuse;
  }
  if (hasField(child, "reflective")) {
    mat->kr = tupleToVec(getField(child, "reflective"));
  } else {
    mat->kr = mat->ks; // defaults to ks if none given.
  }
  if (hasField(child, "transmissive")) {
    mat->kt = tupleToVec(getField(child, "transmissive"));
  }
  if (hasField(child, "index")) { // index of refraction
    mat->index = getField(child, "index")->getScalar();
  }
  if (hasField(child, "shininess")) {
    mat->shininess = getField(child, "shininess")->getScalar();
  }

  if (bindings != NULL) {
    // Want to bind, better have "name" field:
    if (hasField(child, "name")) {
      Obj *field = getField(child, "name");
      string tfield = field->getTypeName();
      string name;
      if (tfield == "id") {
        name = field->getID();
      } else {
        name = field->getString();
      }

      (*bindings)[name] = mat;
    } else {
      throw ParseError(string("Attempt to bind material with no name"));
    }
  }

  return mat;
}

static void processCamera(Obj *child, Scene *scene) {
  if (hasField(child, "position"))
    scene->getCamera()->setEye(tupleToVec(getField(child, "position")));
  if (hasField(child, "quaternion")) {
    const mytuple &quat = getField(child, "quaternion")->getTuple();
    if (quat.size() != 4)
      throw(ParseError("Bogus quaternion."));
    else
      scene->getCamera()->setLook(quat[0]->getScalar(), quat[1]->getScalar(),
                                  quat[2]->getScalar(), quat[3]->getScalar());
  }
  if (hasField(child, "fov"))
    scene->getCamera()->setFOV(getField(child, "fov")->getScalar());
  if (hasField(child, "aspectratio"))
    scene->getCamera()->setAspectRatio(
        getField(child, "aspectratio")->getScalar());
  if (hasField(child, "viewdir") && hasField(child, "updir")) {
    scene->getCamera()->setLook(
        tupleToVec(getField(child, "viewdir")).normalize(),
        tupleToVec(getField(child, "updir")).normalize());
  }
}

static void processObject(Obj *obj, Scene *scene, mmap &materials) {
  // Assume the object is named.
  string name;
  Obj *child;
  vec3f distAttenConst(0.0, 0.0, 0.0);
  if (obj->getTypeName() == "id") {
    name = obj->getID();
    child = NULL;
  } else if (obj->getTypeName() == "named") {
    name = obj->getName();
    child = obj->getChild();
  } else {
    ostrstream oss;
    oss << "Unknown input object ";
    obj->printOn(oss);

    throw ParseError(string(oss.str()));
  }
  // color = tupleToVec(getColorField(child));

  if (name == "directional_light") {
    if (child == NULL) {
      throw ParseError("No info for directional_light");
    }

    maybeExtractField(child, "constant_attenuation_coeff", distAttenConst[0]);
    maybeExtractField(child, "linear_attenuation_coeff", distAttenConst[1]);
    maybeExtractField(child, "quadratic_attenuation_coeff", distAttenConst[2]);

    scene->add(new DirectionalLight(
        scene, tupleToVec(getField(child, "direction")).normalize(),
        tupleToVec(getColorField(child)), distAttenConst));
  } else if (name == "point_light") {
    if (child == NULL) {
      throw ParseError("No info for point_light");
    }
    maybeExtractField(child, "constant_attenuation_coeff", distAttenConst[0]);
    maybeExtractField(child, "linear_attenuation_coeff", distAttenConst[1]);
    maybeExtractField(child, "quadratic_attenuation_coeff", distAttenConst[2]);

    scene->add(new PointLight(scene, tupleToVec(getField(child, "position")),
                              tupleToVec(getColorField(child)),
                              distAttenConst));
  } else if (name == "ambient_light") {
    if (child == NULL) {
      throw ParseError("No info for ambient_light");
    }
    scene->add(new AmbientLight(scene, tupleToVec(getColorField(child)),
                                distAttenConst));
  } else if (name == "spot_light") {
    if (child == NULL) {
      throw ParseError("No info for ambient_light");
    }
    scene->add(new SpotLight(scene, tupleToVec(getField(child, "position")),
                             tupleToVec(getColorField(child)),
                             tupleToVec(getField(child, "direction")),
                             tupleToVec(getField(child, "edgeplace")),
                             distAttenConst));
  } else if (name == "sphere" || name == "box" || name == "cylinder" ||
             name == "cone" || name == "square" || name == "translate" ||
             name == "rotate" || name == "scale" || name == "transform" ||
             name == "trimesh" || name == "polymesh" ||
             name == "hyperbolic") { // polymesh is for backwards compatibility.
    processGeometry(name, child, scene, materials, &scene->transformRoot);
    // scene->add( geo );
  } else if (name == "material") {
    processMaterial(child, &materials);
  } else if (name == "camera") {
    processCamera(child, scene);
  } else if (name == "meteorite") {
    processGeometry(name, child, scene, materials, &scene->transformRoot);
  } else {
    throw ParseError(string("Unrecognized object: ") + name);
  }
}
