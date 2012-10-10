#ifdef __APPLE__ 
  #include <GLUT/glut.h>
  #include <GLUI/glui.h>
#else
  #include <GL/glut.h>
  #include <GL/glui.h>
#endif


#include <stdio.h>
#include <math.h>
#include <vector>

#include "geo/Triangle.hpp"
#include "geo/Sphere.hpp"
#include "geo/Light.hpp"
#include "geo/Scene.hpp"
#include "geo/Quad.hpp"
#include "geo/Box.hpp"
#include "geo/Asset.hpp"

#include "math/Vector.hpp"
#include "math/Color.hpp"

#include "tex/BaseTex.hpp"
#include "tex/MultiTex.hpp"

int main_window;
int glu_window;


Scene scene;

float eye[3];
float lookat[3];


BaseTex* earthTex;
MultiTex* faceTex;
MultiTex* lightTex;

Sphere* earth;
Box* box;
Asset* teapot;
Triangle* triangle;
Light* l;


// GLUI Objects
GLUI *glui;

GLUI_Rollout            *shadow_mode_rollout;
GLUI_RadioGroup         *shadow_mode;

GLUI_Rollout		*light_rollout;
GLUI_Translation	*light_xz_trans;
GLUI_Translation	*light_y_trans;

GLUI_Rollout		*teapot_rollout;
GLUI_Translation	*teapot_xz_trans;
GLUI_Translation	*teapot_y_trans;

GLUI_Rollout		*triangle_rollout;
GLUI_Translation	*triangle_xz_trans;
GLUI_Translation	*triangle_y_trans;


// Live vars
int live_shadow_mode;

float live_light_xz_trans[2];
float live_light_y_trans;
float live_light_intensity;

float live_teapot_xz_trans[2];
float live_teapot_y_trans;

float live_triangle_xz_trans[2];
float live_triangle_y_trans;


// Callback defines
#define CB_LIGHT_RESET 0
#define CB_TEAPOT_RESET 1
#define CB_TRIANGLE_RESET 2


static void display(void)
{
  // Apply live data to geometry
  Vector eyeVect(eye[0], eye[1], eye[2]);
  Vector lookVect(lookat[0], lookat[1], lookat[2]);
 
  scene.SetEye(eyeVect);
  scene.SetLook(lookVect);

  Vector light_translate(live_light_xz_trans[0], 
			 live_light_y_trans,
			 -live_light_xz_trans[1]);
  Color light_color(live_light_intensity,
		    live_light_intensity,
		    live_light_intensity, 1.0);
  
  l->SetTranslate(light_translate);
  l->SetColor(light_color);
  

  // Vector teapot_translate(live_teapot_xz_trans[0], 
  // 			  live_teapot_y_trans,
  // 			  -live_teapot_xz_trans[1]);
  // box->SetTranslate(teapot_translate);
  // teapot->SetTranslate(teapot_translate);
  
  // Vector triangle_translate(live_triangle_xz_trans[0], 
  // 			    live_triangle_y_trans,
  // 			    -live_triangle_xz_trans[1]);
  
  // triangle->SetTranslate(triangle_translate);
  
  switch (live_shadow_mode)
  {
  case 0:
    scene.SetShadowMode(Scene::NONE);
    break;
  case 1:
    scene.SetShadowMode(Scene::PROJECTIVE_SHADOWS);
    break;
  case 2:
    scene.SetShadowMode(Scene::SHADOW_VOLUMES);
    break;
  }

  scene.DrawScene();

  glutSwapBuffers();
}


// some controls generate a callback when they are changed
void glui_cb(int control)
{
  switch (control)
  {
  case CB_LIGHT_RESET:
    break;
  case CB_TEAPOT_RESET:
    break;
  case CB_TRIANGLE_RESET:
    break;
  }
  glutPostRedisplay();
}


void GLUIInit(void)
{

  GLUI* glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_RIGHT);

  // the object rollout
  shadow_mode_rollout = glui->add_rollout("Shadow Mode");

  // the radio buttons
  shadow_mode = glui->add_radiogroup_to_panel(shadow_mode_rollout, 
					      &live_shadow_mode);
  glui->add_radiobutton_to_group(shadow_mode, "None");
  glui->add_radiobutton_to_group(shadow_mode, "Proj. Shadows");
  glui->add_radiobutton_to_group(shadow_mode, "Shadow Volumes");

  
  light_rollout = glui->add_rollout("Light Translate");
  // rotation and translation controls
  // we need an extra panel to keep things grouped properly
  GLUI_Panel *light_trans_panel = glui->add_panel_to_panel(light_rollout, "", 
							   GLUI_PANEL_NONE);
  light_xz_trans = glui->add_translation_to_panel(light_trans_panel, 
						  "Translate XZ", 
						  GLUI_TRANSLATION_XY, 
						  live_light_xz_trans);

  glui->add_column_to_panel(light_trans_panel, false);
  light_y_trans =  glui->add_translation_to_panel(light_trans_panel, 
						  "Translate Y", 
						  GLUI_TRANSLATION_Y, 
						  &live_light_y_trans);

  light_xz_trans->scale_factor = 0.1f;
  light_y_trans->scale_factor = 0.1f;

  GLUI_Spinner *spin_s = glui->add_spinner_to_panel(light_rollout, 
						    "Intensity", 
						    GLUI_SPINNER_FLOAT, 
						    &live_light_intensity);
  spin_s->set_float_limits(0.0, 1.0);

  glui->add_button_to_panel(light_rollout, "Reset Light",
			    CB_LIGHT_RESET, glui_cb);




  // empty space
  glui->add_statictext("");


  teapot_rollout = glui->add_rollout("Teapot Translate");
  // rotation and translation controls
  // we need an extra panel to keep things grouped properly
  GLUI_Panel *teapot_trans_panel = glui->add_panel_to_panel(teapot_rollout, "", 
							    GLUI_PANEL_NONE);
  teapot_xz_trans = glui->add_translation_to_panel(teapot_trans_panel, 
						   "Translate XZ", 
						   GLUI_TRANSLATION_XY, 
						   live_teapot_xz_trans);

  glui->add_column_to_panel(teapot_trans_panel, false);
  teapot_y_trans =  glui->add_translation_to_panel(teapot_trans_panel, 
						   "Translate Y", 
						   GLUI_TRANSLATION_Y, 
						   &live_teapot_y_trans);

  teapot_xz_trans->scale_factor = 0.1f;
  teapot_y_trans->scale_factor = 0.1f;

  glui->add_button_to_panel(teapot_rollout, "Reset Teapot",
			    CB_TEAPOT_RESET, glui_cb);

  // empty space
  glui->add_statictext("");

  triangle_rollout = glui->add_rollout("Triangle Translate");
  // rotation and translation controls
  // we need an extra panel to keep things grouped properly
  GLUI_Panel *triangle_trans_panel = glui->add_panel_to_panel(triangle_rollout, "", 
							      GLUI_PANEL_NONE);
  triangle_xz_trans = glui->add_translation_to_panel(triangle_trans_panel, 
						   "Translate XZ", 
						   GLUI_TRANSLATION_XY, 
						   live_triangle_xz_trans);

  glui->add_column_to_panel(triangle_trans_panel, false);
  triangle_y_trans =  glui->add_translation_to_panel(triangle_trans_panel, 
						   "Translate Y", 
						   GLUI_TRANSLATION_Y, 
						   &live_triangle_y_trans);

  triangle_xz_trans->scale_factor = 0.1f;
  triangle_y_trans->scale_factor = 0.1f;

  glui->add_button_to_panel(triangle_rollout, "Reset Triangle",
			    CB_TRIANGLE_RESET, glui_cb);




  // empty space
  glui->add_statictext("");

  glui->set_main_gfx_window(main_window);
}

// mouse handling functions for the main window
// left mouse translates, middle zooms, right rotates
// keep track of which button is down and where the last position was
int cur_button = -1;
int last_x;
int last_y;

void normalize(float v[3])
{
  float l = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
  l = 1 / (float)sqrt(l);

  v[0] *= l;
  v[1] *= l;
  v[2] *= l;
}

void crossproduct(float a[3], float b[3], float res[3])
{
  res[0] = (a[1] * b[2] - a[2] * b[1]);
  res[1] = (a[2] * b[0] - a[0] * b[2]);
  res[2] = (a[0] * b[1] - a[1] * b[0]);
}

float length(float v[3])
{
  return (float)sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}


// catch mouse up/down events
void myGlutMouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN)
    cur_button = button;
  else
  {
    if (button == cur_button)
      cur_button = -1;
  }

  last_x = x;
  last_y = y;
}

// you can put keyboard shortcuts in here
void myGlutKeyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
    // quit
  case 27: 
  case 'q':
  case 'Q':
    exit(0);
    break;
  }

  glutPostRedisplay();
}

// catch mouse move events
void myGlutMotion(int x, int y)
{
  // the change in mouse position
  int dx = x-last_x;
  int dy = y-last_y;

  float scale, len, theta;
  float neye[3], neye2[3];
  float f[3], r[3], u[3];

  switch(cur_button)
  {
  case GLUT_LEFT_BUTTON:
    // translate
    f[0] = lookat[0] - eye[0];
    f[1] = lookat[1] - eye[1];
    f[2] = lookat[2] - eye[2];
    u[0] = 0;
    u[1] = 1;
    u[2] = 0;

    // scale the change by how far away we are
    scale = sqrt(length(f)) * 0.007;

    crossproduct(f, u, r);
    crossproduct(r, f, u);
    normalize(r);
    normalize(u);

    eye[0] += -r[0]*dx*scale + u[0]*dy*scale;
    eye[1] += -r[1]*dx*scale + u[1]*dy*scale;
    eye[2] += -r[2]*dx*scale + u[2]*dy*scale;

    lookat[0] += -r[0]*dx*scale + u[0]*dy*scale;
    lookat[1] += -r[1]*dx*scale + u[1]*dy*scale;
    lookat[2] += -r[2]*dx*scale + u[2]*dy*scale;

    break;

  case GLUT_MIDDLE_BUTTON:
    // zoom
    f[0] = lookat[0] - eye[0];
    f[1] = lookat[1] - eye[1];
    f[2] = lookat[2] - eye[2];

    len = length(f);
    normalize(f);

    // scale the change by how far away we are
    len -= sqrt(len)*dx*0.03;

    eye[0] = lookat[0] - len*f[0];
    eye[1] = lookat[1] - len*f[1];
    eye[2] = lookat[2] - len*f[2];

    // make sure the eye and lookat points are sufficiently far away
    // push the lookat point forward if it is too close
    if (len < 1)
    {
      printf("lookat move: %f\n", len);
      lookat[0] = eye[0] + f[0];
      lookat[1] = eye[1] + f[1];
      lookat[2] = eye[2] + f[2];
    }

    break;

  case GLUT_RIGHT_BUTTON:
    // rotate

    neye[0] = eye[0] - lookat[0];
    neye[1] = eye[1] - lookat[1];
    neye[2] = eye[2] - lookat[2];

    // first rotate in the x/z plane
    theta = -dx * 0.007;
    neye2[0] = (float)cos(theta)*neye[0] + (float)sin(theta)*neye[2];
    neye2[1] = neye[1];
    neye2[2] =-(float)sin(theta)*neye[0] + (float)cos(theta)*neye[2];


    // now rotate vertically
    theta = -dy * 0.007;

    f[0] = -neye2[0];
    f[1] = -neye2[1];
    f[2] = -neye2[2];
    u[0] = 0;
    u[1] = 1;
    u[2] = 0;
    crossproduct(f, u, r);
    crossproduct(r, f, u);
    len = length(f);
    normalize(f);
    normalize(u);

    neye[0] = len * ((float)cos(theta)*f[0] + (float)sin(theta)*u[0]);
    neye[1] = len * ((float)cos(theta)*f[1] + (float)sin(theta)*u[1]);
    neye[2] = len * ((float)cos(theta)*f[2] + (float)sin(theta)*u[2]);

    eye[0] = lookat[0] - neye[0];
    eye[1] = lookat[1] - neye[1];
    eye[2] = lookat[2] - neye[2];

    break;
  }

  last_x = x;
  last_y = y;

  glutPostRedisplay();
}

void initGeometry()
{
  eye[0] = 0;
  eye[1] = 4;
  eye[2] = 10;
  lookat[0] = 0;
  lookat[1] = 0;
  lookat[2] = 0;
  
  live_light_intensity = 1.0;


  Vector up(0.0, 1.0, 0.0);
  scene.SetUp(up);
  scene.SetFrustum(-1, 1, -1, 1, 1, 1000);
  
  Color ambLight(0.2, 0.2, 0.2, 1.0);
  scene.SetAmbient(ambLight);
  
  earthTex = new BaseTex("../tex/eoe4.rgb", GL_MODULATE, false);
  BaseTex* checkerTex = new BaseTex("../tex/checkerboard.rgb", GL_MODULATE, true);

  Vector light_pos(8.0, 5.0, 5.0);
  Color light_color = SOLID_WHITE;
  l = new Light(light_pos, light_color, GL_LIGHT0);
  scene.AddLight(l);

  Vector cent(0.0, 0.0, 0.0);
  earth = new Sphere();
  earth->PushTex(earthTex);
  earth->Generate(cent, 1.0, 5);
  earth->SetTranslate(Vector(6.0, 0.0, -4.0));
  scene.AddGeometry(earth);

  teapot = new Asset("../assets/teapot.obj");
  teapot->SetTranslate(Vector(0.0, -2.0, 0.0));
  scene.AddGeometry(teapot);
  
  Vector box_center(0.0, -3.0, 0.0);
  box = new Box(box_center, 2, 2, 2);
  box->SetColor(SOLID_DARK_RED);
  scene.AddGeometry(box);

  Vector tri1(0.0, 4.0, 0.0);
  Vector tri2(0.0, 5.0, 0.0);
  Vector tri3(2.0, 4.0, 0.0);
  Vector tnorm(0.0, 0.0, 1.0);
  triangle = new Triangle(tri1, tri2, tri3, SOLID_RED);
  triangle->SetNormals(tnorm, tnorm, tnorm);
  scene.AddOccluder(triangle);

  Vector fpt1(-10.0, -4.0, -10.0);
  Vector fpt2(-10.0, -4.0,  10.0);
  Vector fpt3(10.0, -4.0, 10.0);
  Vector fpt4(10.0, -4.0, -10.0);
  Vector fnorm(0.0, 1.0, 0.0);
  Vector ftex1(-1.0, -1.0, 0.0);
  Vector ftex2(-1.0, 1.0, 0.0);
  Vector ftex3(1.0, 1.0, 0.0);
  Vector ftex4(1.0, -1.0, 0.0);
  Quad* floor = new Quad(fpt1, fpt2, fpt3, fpt4, SOLID_WHITE);
  fnorm.normalize();
  floor->SetNormals(fnorm, fnorm, fnorm, fnorm);
  floor->PushTex(checkerTex);
  floor->PushTexCoords(ftex1, ftex2, ftex3, ftex4);
  scene.AddReceiver(floor); 

  Color lightblue(0.75, 0.75, 1.0, 1.0);

  Vector lwallpt1(-10.0, -4.0, -10.0);
  Vector lwallpt2(-10.0, 10.0, -10.0);
  Vector lwallpt3(-10.0, 10.0, 10.0);
  Vector lwallpt4(-10.0, -4.0, 10.0);
  Vector lwallnorm(1.0, 0.0, 0.0);
  Quad* lwall = new Quad(lwallpt1, lwallpt2, lwallpt3, lwallpt4, lightblue);
  lwallnorm.normalize();
  lwall->SetNormals(lwallnorm, lwallnorm, lwallnorm, lwallnorm);
  scene.AddReceiver(lwall);

  Vector rwallpt1(10.0, -4.0, -10.0);
  Vector rwallpt2(10.0, 10.0, -10.0);
  Vector rwallpt3(10.0, 10.0, 10.0);
  Vector rwallpt4(10.0, -4.0, 10.0);
  Vector rwallnorm(1.0, 0.0, 0.0);
  Quad* rwall = new Quad(rwallpt4, rwallpt3, rwallpt2, rwallpt1, lightblue);
  rwallnorm.normalize();
  rwall->SetNormals(rwallnorm, rwallnorm, rwallnorm, rwallnorm);
  scene.AddReceiver(rwall);
  
  Vector bwallpt1(-10.0, -4.0, -10.0);
  Vector bwallpt2(-10.0, 10.0, -10.0);
  Vector bwallpt3(10.0, 10.0, -10.0);
  Vector bwallpt4(10.0, -4.0, -10.0);
  Vector bwallnorm(0.0, 0.0, 1.0);
  Quad* bwall = new Quad(bwallpt1, bwallpt2, bwallpt3, bwallpt4, lightblue);
  bwallnorm.normalize();
  bwall->SetNormals(bwallnorm, bwallnorm, bwallnorm, bwallnorm);
  scene.AddReceiver(bwall);

//  scene.SetShadowMode(Scene::PROJECTIVE_SHADOWS);
}

void myGlutIdle(void)
{
  // make sure the main window is active
  if (glutGetWindow() != main_window)
    glutSetWindow(main_window);

  // just keep redrawing the scene over and over
  glutPostRedisplay();
}

// the window has changed shapes, fix ourselves up
void myGlutReshape(int	x, int y)
{
  int tx, ty, tw, th;
  GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
  glViewport(tx, ty, tw, th);

  glutPostRedisplay();
}


int main(int argc, char* argv[])
{
  
  // Initialize the Window
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL );  
  glutInitWindowSize(1000,700);
  main_window = glutCreateWindow("Spike");  

  // Set up GLUT/GLUI callbacks
  glutDisplayFunc(&display);
  GLUI_Master.set_glutReshapeFunc(myGlutReshape);
  GLUI_Master.set_glutMouseFunc(myGlutMouse);
  GLUI_Master.set_glutIdleFunc(myGlutIdle);
  glutMotionFunc(myGlutMotion);

  // Set up the GLUI interface
  GLUIInit();

  // Create the objects for our scene
  initGeometry();


  // Off we go
  glutMainLoop();
  
  return 0;
}
