/* viewpoints.cpp, Laura Toma
   
   What it does: Draws a set of points in the default 2D ortho
   projection.  Includes a tentative function for printing and drawing
   a list of points (assumed to be a convex hull).
   
   This code is provided as a startup for your 2d hull.  Change it as
   needed to work with your project.
*/

#include "geom.h"
#include "rtimer.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <strings.h>

//to compile on both apple and unix platform
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>
using namespace std; 




//pre-defined colors for convenience 
GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat gray[3] = {0.5, 0.5, 0.5};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat magenta[3] = {1.0, 0.0, 1.0};
GLfloat cyan[3] = {0.0, 1.0, 1.0};
/* from https://www.opengl.org/discussion_boards/showthread.php/132502-Color-tables  */
GLfloat brown[3] = { 0.647059, 0.164706, 0.164706}; 
GLfloat DarkBrown[3] = { 0.36, 0.25, 0.20}; 
GLfloat DarkTan[3] = { 0.59, 0.41, 0.31};
GLfloat Maroon[3]= { 0.556863, 0.137255, 0.419608}; 
GLfloat DarkWood[3] = { 0.52, 0.37, 0.26}; 
GLfloat  Copper[3] = { 0.72,  0.45,  0.20};
GLfloat green1[3] = {.5, 1, 0.5};
GLfloat green2[3] = {0.0, .8, 0.0};
GLfloat green3[3] = {0.0, .5, 0.0};
GLfloat ForestGreen[3] = { 0.137255, 0.556863, 0.137255};
GLfloat MediumForestGreen[3] = { 0.419608 , 0.556863 , 0.137255}; 
GLfloat LimeGreen[3] ={ 0.196078,  0.8 , 0.196078}; 
GLfloat Orange[3] = { 1, .5, 0}; 
GLfloat Silver[3] = { 0.90, 0.91, 0.98};
GLfloat Wheat[3] = { 0.847059 , 0.847059, 0.74902}; 





/* global variables */

//desired number of points, entered by the user on the command line
int NPOINTS;

//the vector of points
//note: needs to be global in order to be rendered
vector<point2d>  points;

//the convex hull 
//note: needs to be global in order to be rendered
vector<point2d>  hull; 



//window size for the graphics window
const int WINDOWSIZE = 500; 

/* currently there are 4 different ways to initialize points.  The
   user can cycle through them by pressing 'i'. Check out the display()
   function.
*/
int NB_INIT_CHOICES = 4; 
int  POINT_INIT_MODE = 0; //the first inititalizer





/********************************************************************/
/* forward declarations of functions */

//print label, then the vector 
void print_vector(const char* label, vector<point2d> p); 



/* render the points. Each point is drawn as a small square.  */
void draw_points(vector<point2d> pts); 

/* Render the hull; the points on the hull are expected to be in
   boundary order (either ccw or cw), otherwise it will look
   zig-zaagged.  
*/
  void draw_hull(vector<point2d> hull); 


void display(void);
void keypress(unsigned char key, int x, int y);

// initializer function
void initialize_points_circle(vector<point2d>& pts, int n); 
void initialize_points_horizontal_line(vector<point2d>&pts, int n);
void initialize_points_random(vector<point2d>&pts, int n) ;
void initialize_points_cross(vector<point2d>&pts, int n) ;

//you'll add more 


/********************************************************************/






/* ****************************** */
/* Initializes pts with n points on two circles.  The points are in the
   range [0, WINSIZE] x [0, WINSIZE].
*/ 
void initialize_points_circle(vector<point2d>& pts, int n) {

  printf("\ninitialize points circle\n"); 
  //clear the vector just to be safe 
  pts.clear(); 

  n = n/2; //we'll generaate two circles, n/2 points each
  double  step = 2* M_PI/n; 
  int radius = 100; 

  point2d p; 
  for (int i=0; i<n; i++) {
    p.x = WINDOWSIZE/2+ radius*cos(i*step); 
    p.y = WINDOWSIZE/2+ radius*sin(i*step); 
    pts.push_back(p); 
  }

  radius /= 2; 
  for (int i=0; i<n; i++) {
    p.x = WINDOWSIZE/2+ radius*cos(i*step); 
    p.y = WINDOWSIZE/2+ radius*sin(i*step); 
    pts.push_back(p); 
  }
  
}



/* ****************************** */
/* Initializes pts with n points on a line.  The points are in the
   range [0, WINSIZE] x [0, WINSIZE].
*/ 
void initialize_points_horizontal_line(vector<point2d>& pts, int n) {

  printf("\ninitialize points line\n"); 
  //clear the vector just to be safe 
  pts.clear(); 
  
  point2d p; 
  for (int i=0; i<n; i++) {
    p.x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE)); 
    p.y =  WINDOWSIZE/2; 
    pts.push_back(p); 
  }
}




/* ****************************** */
/* Initializes pts with n random points.  The points are in the
   range [0, WINSIZE] x [0, WINSIZE].
*/ 
void initialize_points_random(vector<point2d>& pts, int n) {

   printf("\ninitialize points random\n"); 
  //clear the vector just to be safe 
  pts.clear(); 

  point2d p; 
  for (int i=0; i<n; i++) {
    p.x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE)); 
    p.y =  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    pts.push_back(p); 
  }
}




/* ****************************** */
/* Initializes pts with n points on a cross-like shape.  The points are
   in the range (0,0) to (WINSIZE,WINSIZE).
*/ 
void initialize_points_cross(vector<point2d>& pts, int n) {
  
  printf("\ninitialize points cross\n"); 
  //clear the vector just to be safe 
  pts.clear(); 
  
  point2d p; 
  for (int i=0; i<n; i++) {
    if (i%2 == 0) {
      
      p.x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE)); 
      p.y =  random() % ((int)(.7*WINDOWSIZE))  / 5;
      p.y += (int)((1-.7/5)*WINDOWSIZE/2);
    };
    if (i%2 == 1)  {
      
      p.x = random() % ((int)(.7*WINDOWSIZE)) / 5; 
      p.x +=  (int)((1-.7/5)*WINDOWSIZE/2);
      p.y =  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    }
   
    pts.push_back(p); 
    
  }//for i

}

/* ****************************** */
/* Initializes pts with n points on the sides of a square.  The square
   has the range (WINSIZE/4,WINSIZE/4) to (3*WINSIZE/4,3*WINSIZE/4).
*/ 
void initialize_points_square(vector<point2d>& pts, int n) {
  printf("\ninitialize points square\n"); 

  //clear the vector just to be safe 
  pts.clear(); 
  int width = WINDOWSIZE / 2;
  int start = WINDOWSIZE / 4;

  //4 sides of the square w/ evenly distrubuted points
  for (int i = 0; i < n/4; i++){
    point2d ptop;
    point2d pbottom;
    double offset = (double)4 / n * width;
    double dist = offset * i;
    //bottom and top of square
    ptop.x = start + dist + offset;
    pbottom.x = start + dist;
    //bottom
    pbottom.y = start;
    pts.push_back(pbottom);
    //top
    ptop.y = start + width;
    pts.push_back(ptop);

    point2d pleft;
    point2d pright;
    //sides of the square
    pleft.y = start + dist + offset;
    pright.y = start + dist;
    //left side
    pleft.x = start;
    pts.push_back(pleft);
    //right side of square
    pright.x = start + width;
    pts.push_back(pright);
  }

  //put the extra points on the middle diagonal:
  for (int i = 0; i < (n % 4); i++){
    point2d p;
    p.x = start + (i+1)*(width / 4);
    p.y = start + (i+1)*(width / 4);
    pts.push_back(p);
  }
}

/* ****************************** */
/* Initializes pts with n points on the sides of a square, rotated 45 degrees from the x and y axes.  The square
   The square is centered in the window with sidelength WINSIZE/4 * SQRT(2) so that it has width WINSIZE/2
*/ 
void initialize_points_diamond(vector<point2d>& pts, int n) {
  printf("\ninitialize points square\n"); 

  //clear the vector just to be safe 
  pts.clear(); 
  int width = WINDOWSIZE / 2; //from left corner to right corner (so sidelength is sqrt(2)/2 times this)
  int center = WINDOWSIZE / 2;

  //4 sides of the square w/ evenly distrubuted points, but the square is rotated 45 degrees CCW
  //for loop evenly distributes points along each of the four edges at once, distrubting in clockwise direction
  for (int i = 0; i < n/4; i++){
    double offset = (double)2 / n * width * i; // to distribute 1/4 of all the points at equal intervals
    
    point2d pNW; //point on northwest edge
    pNW.x = center - ((double)width / 2) + offset;
    pNW.y = center + offset;
    pts.push_back(pNW);

    point2d pSE; //point on southeast edge
    pSE.x = center + ((double)width / 2) - offset;
    pSE.y = center - offset;
    pts.push_back(pSE);

    point2d pSW; //point on southwest edge
    pSW.x = center - offset;
    pSW.y = center - ((double)width / 2) + offset;
    pts.push_back(pSW);

    point2d pNE; //point on northeast edge
    pNE.x = center + offset;
    pNE.y = center + ((double)width / 2) - offset;
    pts.push_back(pNE);
  }

  //put the extra points in the middle:
  for (int i = 0; i < (n % 4); i++){
    point2d p;
    p.x = center - ((double)width / 2) + (i+1)*(width / 4);
    p.y = center;
    pts.push_back(p);
  }
}

/* ****************************** */
/* Initializes pts with n points on the edges of a heart. 
    within the x=[0,WINDOWSIZE] y=[0,WINDOWSIZE] window.
*/ 
void initialize_points_heart(vector<point2d>& pts, int n) { 

  //clear the vector just to be safe 
  pts.clear(); 
  
  double t = 2*M_PI / n;
  point2d p;

  for (int i = 0; i<n; i++){
    double a = i*t;
    p.x = 16 * sin(a) * sin(a) * sin(a);
    p.y = 13 * cos(a) - 5 * cos(2 * a) - 2 * cos(3 * a) - cos(4 * a);
    p.x *= (WINDOWSIZE/100);
    p.y *= (WINDOWSIZE/100);
    p.x += (WINDOWSIZE / 2);
    p.y += (WINDOWSIZE / 2);
    pts.push_back(p);
  }
  printf("\ninitialized points in a heart\n");
  //printf("\ninitialized %lu points in a heart\n", pts.size());
}

/*
  initializer written by Ziyou Hu and Victoria Figueroa
  initializes points in two vertical lines
*/
void initialize_points_two_vertical(vector<point2d>& pts, int n) {
  
  printf("\ninitialize points that make two vertical lines.\n"); 
    //clear the vector just to be safe 
  pts.clear(); 
  
  point2d p; 
  for (int i=0; i<n/2; i++) {
    p.y = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE)); 
    p.x =  WINDOWSIZE/2; 
    pts.push_back(p); 
  }
    for (int i=0; i<n/2; i++) {
    p.y = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE)); 
    p.x =  WINDOWSIZE/4; 
    pts.push_back(p); 
  }
}


/*
  initializer written by Ziyou Hu and Victoria Figueroa
  initializes points in one vertical line
*/
void initialize_points_vertical_line(vector<point2d>&pts, int n){
    printf("\ninitialize points line\n"); 
  //clear the vector just to be safe 
  pts.clear(); 
  
  point2d p; 
  for (int i=0; i<n; i++) {
    p.y = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE)); 
    p.x =  WINDOWSIZE/2; 
    pts.push_back(p); 
  }
}

/*
  function written by Tom Han
  initializes points in the shame of a number 1
*/
void initialize_points_1(vector<point2d>&pts, int n){
  printf("\ninitialize points 1\n"); 
  pts.clear();
  assert(pts.size() == 0);

  point2d p;
  int pos, pos2;
  for (int i = 0; i < n; i++)
  {
    switch (i % 7) {
      case 0:
      case 1:
        // Base
        p.y = random() % (int)(.1*WINDOWSIZE);
        p.y += (int) (0.15 * WINDOWSIZE);
        p.x = random() % (int)(0.3*WINDOWSIZE);
        p.x += (int) (0.35 * WINDOWSIZE);
        break;
      case 2:
        pos = random() % (int) (0.1*WINDOWSIZE);
        p.x = pos; p.y = pos;
        p.x += (int) (0.4 * WINDOWSIZE);
        p.y += (int) (0.7*WINDOWSIZE);
        pos2 = random() % (int) (0.05*WINDOWSIZE);
        p.x -= pos2;
        p.y += pos2;
        break;
      case 3:
        // Colinear case
        p.x = (int)(0.25*WINDOWSIZE) + random() % ((int)(0.5*WINDOWSIZE));
        p.y = (int) (0.10*WINDOWSIZE);
        break;
      default:
        //stem
        p.y = (int) (0.25*WINDOWSIZE);
        p.y += random() % (int) (0.6*WINDOWSIZE);
        p.x = (int) (0.45*WINDOWSIZE);
        p.x += random() % (int) (0.1*WINDOWSIZE);
        break;
    }
    pts.push_back(p);
  }
}

/*
  function written by Tom Han
  initializes points in the shape of a number 2
*/
void initialize_points_2(vector<point2d>&pts, int n){
  printf("\ninitialize points 2\n"); 
  pts.clear();
  assert(pts.size() == 0);

  point2d p;
  int x_noise, y_noise;
  int pos, pos2;
  for (int i = 0; i < n; i++)
  {
    switch (i % 3) {
      case 0:
        p.x = (int)(0.25*WINDOWSIZE) + random() % ((int)(0.5*WINDOWSIZE));
        p.y = (int) (0.15*WINDOWSIZE);
        break;
      case 1:
        pos = random() % (int)(0.5*WINDOWSIZE);
        p.x = pos; p.y = pos;
        p.x += (int) (0.25*WINDOWSIZE);
        p.y += (int) (0.15*WINDOWSIZE);
        break;
      case 2: 
        pos2 = random() % 180;
        p.x = (int)(0.5 * WINDOWSIZE) + (int) ((0.25*WINDOWSIZE) * cos((M_PI * pos2)/180));
        p.y = (int)(0.65 * WINDOWSIZE) + (int) ((0.25*WINDOWSIZE) * sin((M_PI * pos2)/180));
        break;
    }
    x_noise = random() % ((int) (0.05*WINDOWSIZE));
    y_noise = random() % ((int) (0.05*WINDOWSIZE));
    p.x += x_noise;
    p.y += y_noise;
    pts.push_back(p);
  }
}

/* function written by Abhi Nagireddygari and Max Danenhower
    initializes points randomly inside a triangle with three points
      at the vertices
*/
void initialize_points_triangle(vector<point2d>&pts, int n) {
  printf("\ninitialize points triangle\n");
  pts.clear();
  point2d p;
  for (int i = 0; i < n; i++) {
    // first three points make the points of the triangle
    if (i == 0) {
      p.x = WINDOWSIZE/2;
      p.y = WINDOWSIZE;
    }
    else if (i == 1) {
      p.x = 0;
      p.y = 0;
    }
    else if (i == 2) {
      p.x = WINDOWSIZE;
      p.y = 0;
    }
    // all the other points are randomly distributed in the triangle
    else {
      double a = random() / static_cast<double>(RAND_MAX);
      double b = random() / static_cast<double>(RAND_MAX);
      if ((a + b) > 1) {
        a = 1 - a;
        b = 1 - b;
      }
      p.x = a*WINDOWSIZE*0.5 + b*WINDOWSIZE;
      p.y = a*WINDOWSIZE;
    }
    pts.push_back(p);
  }
}


/* ****************************** */
/* print the vector of points */
void print_vector(const char* label, vector<point2d> points) {
  
  printf("%s ", label);
  for (int i=0; i< points.size(); i++) {
    printf("[%3d,%3d] ", points[i].x, points[i].y);
  }
  printf("\n");
}





/* ****************************** */
int main(int argc, char** argv) {

  //read number of points from user
  if (argc!=2) {
    printf("usage: viewPoints <nbPoints>\n");
    exit(1); 
  }
  NPOINTS = atoi(argv[1]); 
  printf("you entered n=%d\n", NPOINTS);
  assert(NPOINTS >0); 

  //populate the points 
  initialize_points_random(points, NPOINTS);
  //initialize_points_circle(points, NPOINTS);
  //initialize_points_cross(points, NPOINTS);
  //initialize_points_horizontal_line(points, NPOINTS);
  //initialize_points_heart(points, NPOINTS);
  //initialize_points_square(points, NPOINTS);
  //initialize_points_diamond(points, NPOINTS);
  //initialize_points_two_vertical(points, NPOINTS); //from Victoria and Ziyou
  //initialize_points_vertical_line(points, NPOINTS); //from Victoria and Ziyou
  //initialize_points_1(points, NPOINTS); //from Tom
  //initialize_points_2(points, NPOINTS); //from Tom
  //initialize_points_triangle(points, NPOINTS); //from Abhi and Max
  //print_vector("points:", points);

  //compute the convex hull 
  Rtimer rt1; 
  rt_start(rt1); 
  graham_scan(points, hull); 
  rt_stop(rt1); 
  print_vector("hull:", hull);
  
  //print the timing 
  char buf [1024]; 
  rt_sprint(buf,rt1);
  printf("hull time:  %s\n\n", buf);
  fflush(stdout); 

 
  //start the rendering 
  /* initialize GLUT  */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);
  glutInitWindowPosition(100,100);
  glutCreateWindow(argv[0]);

  /* register callback functions */
  glutDisplayFunc(display); 
  glutKeyboardFunc(keypress);

  /* init GL */
  /* set background color black*/
  glClearColor(0, 0, 0, 0);   
  
  /* give control to event handler */
  glutMainLoop();
  return 0;
}




/* ****************************** */
/* This is the function that renders the window. We registered this
   function as the "displayFunc". It will be called by GL everytime
   the window needs to be rendered.
 */
void display(void) {

  glClear(GL_COLOR_BUFFER_BIT);
  //clear all modeling transformations
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity(); 


  /* The default GL window is [-1,1]x[-1,1]x[-1,1] with the origin in
     the center. The camera is at (0,0,0) looking down negative
     z-axis.  

     The points are in the range [0, WINSIZE] x [0, WINSIZE] so they
     need to be mapped to [-1,1]x [-1,1] */
  
  
  //scale and translation are represented as a transformation matrix M
  //we add the scale then the translation, but when M is applied to a point
  //  the translation is applied first, then the scale is applied
  //  this is all done by representing the transformation with an additional dimension
  //  as a 3D homogeneous coordinates
  //then scale the points to [0,2]x[0,2]
  glScalef(2.0/WINDOWSIZE, 2.0/WINDOWSIZE, 1.0);  
  //first translate the points to [-WINDOWSIZE/2, WINDOWSIZE/2]
  glTranslatef(-WINDOWSIZE/2, -WINDOWSIZE/2, 0); 
 
  draw_points(points);
  draw_hull(hull); 

  /* execute the drawing commands */
  glFlush();
}




/* ****************************** */
/* draw the points. each point is drawn as a small square
*/
void draw_points(vector<point2d> points){

  const int R= 1;
  //draw polygon filled or line 
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //set drawing color 
  glColor3fv(yellow);   
  
  for (int i=0; i< points.size(); i++) {
    //draw a small square centered at (points[i].x, points[i].y)
    glBegin(GL_POLYGON);
    glVertex2f(points[i].x -R,points[i].y-R);
    glVertex2f(points[i].x +R,points[i].y-R);
    glVertex2f(points[i].x +R,points[i].y+R);
    glVertex2f(points[i].x -R,points[i].y+R);
    glEnd();
  }
} //draw_points 





/* ****************************** */
/* Draw the hull; the points on the hull are expected to be in
   boundary order (either ccw or cw) or else it will look
   zig-zaagged. To render the hull we'll draw lines between
   consecutive points */
void draw_hull(vector<point2d> hull){

  //set color 
  glColor3fv(red);   //this should be a constant
  
  if (hull.size() >0) {
    int i; 
    for (i=0; i< hull.size()-1; i++) {
      
      //draw a line from  i to i+1
      glBegin(GL_LINES);
      glVertex2f(hull[i].x, hull[i].y); 
      glVertex2f(hull[i+1].x, hull[i+1].y); 
      glEnd();
    }
    
    //draw a line from the last point to the first point
    i =  hull.size()-1; 
    glBegin(GL_LINES);
    glVertex2f(hull[i].x, hull[i].y); 
    glVertex2f(hull[0].x, hull[0].y); 
    glEnd();
  }//if (hull not empty)
}



/* ****************************** */
/* Handler for key presses. called whenever a key is spressed */
void keypress(unsigned char key, int x, int y) {
  switch(key) {
  case 'q':
    exit(0);
    break;

  case 'i':
    //when the user presses 'i', we want to re-initialize the points and
    //recompute the hull
    POINT_INIT_MODE = (POINT_INIT_MODE+1) % NB_INIT_CHOICES; 
    switch (POINT_INIT_MODE) {
    case 0: 
      initialize_points_circle(points, NPOINTS); 
      break; 
    case 1: 
      initialize_points_cross(points, NPOINTS); 
      break; 
    case 2: 
      initialize_points_horizontal_line(points, NPOINTS); 
      break; 
    case 3: 
      initialize_points_random(points, NPOINTS); 
      break; 
    } //switch 
    //we changed the points, so we need to recompute the hull
    graham_scan(points, hull); 

    //we changed stuff, so we need to tell GL to redraw
    glutPostRedisplay();

  } //switch (key)

}//keypress


