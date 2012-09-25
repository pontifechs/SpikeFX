
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

#include "math/Vector.hpp"
#include "math/Color.hpp"

#include "tex/Texture.hpp"

int main_window;
int glu_window;


Scene scene;

float eye[3];
float lookat[3];


Texture* earthTex;
Texture* faceTex;
Texture* checkerTex;
Texture* brickTex;
Texture* lightTex;

Sphere* earth;
Box* box;


// GLUI Objects
GLUI *glui;
GLUI_Rollout		*object_rollout;
GLUI_Rotation		*object_rotation;
GLUI_Translation	*object_xz_trans;
GLUI_Translation	*object_y_trans;
GLUI_RadioGroup         *sphere_tex_mode;

GLUI_Rollout		*anim_rollout;
GLUI_Button		*start_button;
GLUI_Button             *stop_button;
GLUI_Button             *reset_button;

GLUI_Checkbox           *draw_light_check;

// Live vars
float live_object_rotation[16];
float live_object_xz_trans[2];
float live_object_y_trans;


bool live_face_anim;
float live_face_alpha;
float live_anim_speed;
int live_sphere_tex_mode;

int live_draw_lightmap;


// Callback defines
#define CB_TRANSFORM_RESET 0
#define CB_DISSOLVE_START 1
#define CB_DISSOLVE_STOP 2
#define CB_DISSOLVE_RESET 3
#define CB_TOGGLE_LIGHTMAP 4


static void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  

  // Apply live data to geometry
  Vector eyeVect(eye[0], eye[1], eye[2]);
  Vector lookVect(lookat[0], lookat[1], lookat[2]);
 
  scene.SetEye(eyeVect);
  scene.SetLook(lookVect);

  switch(live_sphere_tex_mode)
  {
  case 0:
    earthTex->SetMode(GL_MODULATE);
    break;
  case 1:
    earthTex->SetMode(GL_REPLACE);
    break;
  case 2:
    earthTex->SetMode(GL_BLEND);
    break;
  case 3:
    earthTex->SetMode(GL_DECAL);
    break;
  }

  if (live_face_anim)
  {
    live_face_alpha += live_anim_speed / 10.0;
    if (live_face_alpha >= 1.0)
    {
      live_face_alpha = 0;
    }
  }
  
  faceTex->SetAlpha(live_face_alpha);

  earth->SetRotate(live_object_rotation);
  box->SetRotate(live_object_rotation);

  Vector translate(live_object_xz_trans[0], 
		   live_object_y_trans,
    		   -live_object_xz_trans[1]);
  earth->SetTranslate(translate);
  box->SetTranslate(translate);

  if (live_draw_lightmap)
  {
    lightTex->SetAlpha(0.75);
  }
  else {
    lightTex->SetAlpha(0.0);
  }

  scene.DrawScene();

  glutSwapBuffers();
}


// some controls generate a callback when they are changed
void glui_cb(int control)
{
  switch(control)
  {
  case CB_TRANSFORM_RESET:
    // Zero out the translations/rotation
    live_object_rotation[0] = 1;
    live_object_rotation[1] = 0;
    live_object_rotation[2] = 0;
    live_object_rotation[3] = 0;

    live_object_rotation[4] = 0;
    live_object_rotation[5] = 1;
    live_object_rotation[6] = 0;
    live_object_rotation[7] = 0;

    live_object_rotation[8] = 0;
    live_object_rotation[9] = 0;
    live_object_rotation[10] = 1;
    live_object_rotation[11] = 0;

    live_object_rotation[12] = 0;
    live_object_rotation[13] = 0;
    live_object_rotation[14] = 0;
    live_object_rotation[15] = 1;

    live_object_xz_trans[0] = 0;
    live_object_xz_trans[1] = 0;
    live_object_y_trans = 0;    

    break;
  case CB_DISSOLVE_START:
    live_face_anim = true;
    break;
  case CB_DISSOLVE_STOP:
    live_face_anim = false;
    break;
  case CB_DISSOLVE_RESET:
    live_face_anim = false;
    live_face_alpha = 0.0f;
    break;
  }


  glutPostRedisplay();
}


void GLUIInit(void)
{

  GLUI* glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_RIGHT);

  // the object rollout
  object_rollout = glui->add_rollout("Object");

  // the radio buttons
  sphere_tex_mode = glui->add_radiogroup_to_panel(object_rollout, 
						  &live_sphere_tex_mode);
  glui->add_radiobutton_to_group(sphere_tex_mode, "GL_MODULATE");
  glui->add_radiobutton_to_group(sphere_tex_mode, "GL_REPLACE");
  glui->add_radiobutton_to_group(sphere_tex_mode, "GL_BLEND");
  glui->add_radiobutton_to_group(sphere_tex_mode, "GL_DECAL");

  // rotation and translation controls
  // we need an extra panel to keep things grouped properly
  GLUI_Panel *transform_panel = glui->add_panel_to_panel(object_rollout, "", 
							 GLUI_PANEL_NONE);
  object_rotation = glui->add_rotation_to_panel(transform_panel, "Rotation", 
						live_object_rotation);
  object_rotation->reset();

  glui->add_column_to_panel(transform_panel, false);
  object_xz_trans = glui->add_translation_to_panel(transform_panel, 
						   "Translate XZ", 
						   GLUI_TRANSLATION_XY, 
						   live_object_xz_trans);

  glui->add_column_to_panel(transform_panel, false);
  object_y_trans =  glui->add_translation_to_panel(transform_panel, 
						   "Translate Y", 
						   GLUI_TRANSLATION_Y, 
						   &live_object_y_trans);

  object_xz_trans->scale_factor = 0.1f;
  object_y_trans->scale_factor = 0.1f;

  glui->add_button_to_panel(object_rollout, "Reset Object Transform", 
			    CB_TRANSFORM_RESET, glui_cb);

  // empty space
  glui->add_statictext("");
  // the walk control
  anim_rollout = glui->add_rollout("Animation");

  start_button = glui->add_button_to_panel(anim_rollout, 
					   "Start", 
					   CB_DISSOLVE_START,
					   glui_cb);
  stop_button = glui->add_button_to_panel(anim_rollout, 
					  "Stop", 
					  CB_DISSOLVE_STOP,
					  glui_cb);
  reset_button = glui->add_button_to_panel(anim_rollout, 
					   "Reset", 
					   CB_DISSOLVE_RESET,
					   glui_cb);

  GLUI_Spinner *spin_s = glui->add_spinner_to_panel(anim_rollout, 
						    "Speed", 
						    GLUI_SPINNER_FLOAT, 
						    &live_anim_speed);

  spin_s->set_float_limits(0.1, 10.0);


  glui->set_main_gfx_window(main_window);

  glui->add_checkbox("Draw Light Mapped Floor", 
		     &live_draw_lightmap, 
		     CB_TOGGLE_LIGHTMAP, 
		     glui_cb);

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
  eye[2] = 20;
  lookat[0] = 0;
  lookat[1] = 0;
  lookat[2] = 0;

  Vector up(0.0, 1.0, 0.0);
  scene.SetUp(up);

  scene.SetFrustum(-1, 1, -1, 1, 1, 1000);
  
  Color ambLight(0.2, 0.2, 0.2, 1.0);
  scene.SetAmbient(ambLight);
  
  live_face_alpha = 0;


  earthTex = new Texture("../tex/eoe4.rgb", 1.0, GL_MODULATE, false, GL_TEXTURE0);
  faceTex = new Texture("../tex/mdudley.rgb", live_face_alpha, GL_MODULATE, false,  GL_TEXTURE1);
  checkerTex = new Texture("../tex/checkerboard.rgb", 1.0, GL_MODULATE, true, GL_TEXTURE0);
  brickTex = new Texture("../tex/brick.rgb", 1.0, GL_MODULATE, true, GL_TEXTURE0); 
  lightTex = new Texture("../tex/lightmap.rgb", 0.7, GL_MODULATE, false, GL_TEXTURE1);
  
  Vector light_pos(5.0, 5.0, 5.0);
  Color light_color = SOLID_WHITE;
  Light l(light_pos, light_color, GL_LIGHT0);
  scene.AddLight(l);

  Vector cent(0.0, 2.0, 0.0);
  earth = new Sphere();
  earth->RegisterTex1(earthTex);
  earth->RegisterTex2(faceTex);
  earth->Generate(cent, 2.0, 5);
  scene.AddGeometry(earth);



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
  floor->SetNormals(fnorm, fnorm, fnorm, fnorm);
  floor->RegisterTex1(checkerTex);
  floor->RegisterTex2(lightTex);
  floor->SetTex1Coords(ftex1, ftex2, ftex3, ftex4);
  floor->SetTex2Coords(ftex1, ftex2, ftex3, ftex4);
  scene.AddGeometry(floor); 

  Vector lwallpt1(-10.0, -4.0, -10.0);
  Vector lwallpt2(-10.0, 10.0, -10.0);
  Vector lwallpt3(-10.0, 10.0, 10.0);
  Vector lwallpt4(-10.0, -4.0, 10.0);
  Vector lwallnorm(1.0, 0.0, 0.0);
  Quad* lwall = new Quad(lwallpt1, lwallpt2, lwallpt3, lwallpt4, SOLID_WHITE);
  lwall->SetNormals(lwallnorm, lwallnorm, lwallnorm, lwallnorm);
  lwall->RegisterTex1(brickTex);
  lwall->SetTex1Coords(ftex1, ftex2, ftex3, ftex4);
  scene.AddGeometry(lwall);

  
  Vector rwallpt1(10.0, -4.0, -10.0);
  Vector rwallpt2(10.0, 10.0, -10.0);
  Vector rwallpt3(10.0, 10.0, 10.0);
  Vector rwallpt4(10.0, -4.0, 10.0);
  Vector rwallnorm(1.0, 0.0, 0.0);
  Quad* rwall = new Quad(rwallpt1, rwallpt2, rwallpt3, rwallpt4, SOLID_WHITE);
  rwall->SetNormals(rwallnorm, rwallnorm, rwallnorm, rwallnorm);
  rwall->RegisterTex1(brickTex);
  rwall->SetTex1Coords(ftex1, ftex2, ftex3, ftex4);
  scene.AddGeometry(rwall);

  
  Vector bwallpt1(-10.0, -4.0, -10.0);
  Vector bwallpt2(-10.0, 10.0, -10.0);
  Vector bwallpt3(10.0, 10.0, -10.0);
  Vector bwallpt4(10.0, -4.0, -10.0);
  Vector bwallnorm(0.0, 0.0, 1.0);
  Quad* bwall = new Quad(bwallpt1, bwallpt2, bwallpt3, bwallpt4, SOLID_WHITE);
  bwall->SetNormals(bwallnorm, bwallnorm, bwallnorm, bwallnorm);
  bwall->RegisterTex1(brickTex);
  bwall->SetTex1Coords(ftex1, ftex2, ftex3, ftex4);
  scene.AddGeometry(bwall);


  Vector boxCent(0.0, -2.0, 0.0);
  box = new Box(boxCent, 2, 2, 2);
  box->SetColor(SOLID_DARK_RED);
  box->SetCenter(cent);
  scene.AddGeometry(box);

}

void myGlutIdle(void)
{
  // make sure the main window is active
  if (glutGetWindow() != main_window)
    glutSetWindow(main_window);

  // just keep redrawing the scene over and over
  glutPostRedisplay();
}

int main(int argc, char* argv[])
{
  
  // Initialize the Window
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );  
  glutInitWindowSize(700,700);
  main_window = glutCreateWindow("Spike");  

  // Set up GLUT/GLUI callbacks
  glutDisplayFunc(&display);
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


