#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cfloat>

#include "gluvi.h"
#include "fluidsim.h"
#include "openglutils.h"
#include "array2_utils.h"

using namespace std;

//Try changing the grid resolution
int grid_resolution = 96;
scalar timestep = 0.005;
scalar grid_width = 100.0;
int current_frame = 0;
int total_frames = 240;
bool isScreenshot = true;
FluidSim sim;

//Gluvi stuff
//-------------
// Bottom Left Height
Gluvi::PanZoom2D cam(-0.1, -0.1, 1.2);
double oldmousetime;
Vector2s oldmouse;
void display();
void mouse(int button, int state, int x, int y);
void drag(int x, int y);
void timer(int junk);
bool save_screenshot(std::string filename, int w, int h);


//Boundary definition - several circles in a circular domain.

Vector2s c0(50,50), c1(70,50), c2(30,35), c3(50,70);
Vector2s s0(10,5);
scalar rad0 = 40,  rad1 = 10,  rad2 = 10,   rad3 = 10;
Vector2s o0(0.0, 0.0);

//Main testing code
//-------------
int main(int argc, char **argv)
{
  
  //Setup viewer stuff
  glPointSize(1);
  Gluvi::init("Basic Fluid Solver with Static Variational Boundaries", &argc, argv);
  Gluvi::camera=&cam;
  Gluvi::userDisplayFunc=display;
  Gluvi::userMouseFunc=mouse;
  Gluvi::userDragFunc=drag;  
  glutTimerFunc(1000, timer, 0);
  
  //Set up the simulation
  sim.initialize(o0, grid_width, grid_resolution, grid_resolution, 1.0);

  sim.root_boundary = new FluidSim::Boundary(c0, Vector2s(40.0, 40.0), FluidSim::BT_BOX, true);
  
  sim.root_sources = NULL;

  sim.update_boundary();
  sim.init_random_particles();

  Gluvi::run();
  
  delete sim.root_boundary;
  
  return 0;
}


void display(void)
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  sim.render();
  if (isScreenshot && current_frame - 1 != total_frames) {
    char filename[256];
    current_frame += 1;
    sprintf(filename, "Frame%04d.TGA", current_frame);
    std::string str_filename(filename);
    save_screenshot(str_filename, 720, 720);
  }
}

void mouse(int button, int state, int x, int y)
{
  Vector2s newmouse;
  cam.transform_mouse(x, y, newmouse.data());
  //double newmousetime=get_time_in_seconds();
  
  oldmouse=newmouse;
  //oldmousetime=newmousetime;
}

void drag(int x, int y)
{
  Vector2s newmouse;
  cam.transform_mouse(x, y, newmouse.data());
  //double newmousetime=get_time_in_seconds();
  
  oldmouse=newmouse;
  //oldmousetime=newmousetime;
}

void timer(int junk)
{
  if (current_frame - 1 == total_frames) 
    return;

  sim.advance(timestep);
  
  glutPostRedisplay();
  glutTimerFunc(30, timer, 0);
  
}

bool save_screenshot(string filename, int w, int h)
{ 
  //This prevents the images getting padded 
 // when the width multiplied by 3 is not a multiple of 4
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
 
  int nSize = w*h*3;
  // First let's create our buffer, 3 channels per Pixel
  char* dataBuffer = (char*)malloc(nSize*sizeof(char));
 
  if (!dataBuffer) return false;
 
   // Let's fetch them from the backbuffer  
   // We request the pixels in GL_BGR format, thanks to Berzeger for the tip
   glReadPixels((GLint)0, (GLint)0,
    (GLint)w, (GLint)h,
     GL_BGR, GL_UNSIGNED_BYTE, dataBuffer);
 
   //Now the file creation
   FILE *filePtr = fopen(filename.c_str(), "wb");
   if (!filePtr) return false;
 
 
   unsigned char TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};
   unsigned char header[6] = { w%256,w/256,
             h%256,h/256,
             24,0};
   // We write the headers
   fwrite(TGAheader,  sizeof(unsigned char),  12, filePtr);
   fwrite(header, sizeof(unsigned char),  6,  filePtr);
   // And finally our image data
   fwrite(dataBuffer, sizeof(GLubyte),  nSize,  filePtr);
   fclose(filePtr);
 
   free(dataBuffer);
 
  return true;
}





