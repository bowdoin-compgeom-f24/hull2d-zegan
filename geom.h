#ifndef __geom_h
#define __geom_h

#include <vector>

using namespace std; 


typedef struct _point2d {
  int x,y; 
} point2d;



/* returns 2 times the signed area of triangle abc. The area is
   positive if c is to the left of ab, 0 if a,b,c are collinear and
   negative if c is to the right of ab
 */
int signed_area2D(point2d a, point2d b, point2d c); 


/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2d p, point2d q, point2d r);


/* return 1 if c is  strictly left of ab; 0 otherwise */
int left_strictly (point2d a, point2d b, point2d c); 


/* return 1 if c is left of ab or on ab; 0 otherwise */
int left_on(point2d a, point2d b, point2d c); 

/*
  finds a point on the hull from a vector of points pts
  selects the point with the lowest y value (highest x if tied)
*/
int find_bottom_point(vector<point2d>& pts);

/*
  the merge function for a mergesort sorting radially in counterclockwise order with respect to p0
  start and p1stop are the indices in pts_unmerged where sorted p1 and p2 begin
  p2_stop is the index in pts_sorted after p2 ends
  pts_unsorted[0] should be p0 (the point that we are sorting by)
*/
void merge_points(vector<point2d>& pts_unmerged, int start, int p1_stop, int p2_stop);

/*
  recursive mergesort function
  radially sort all other points in a vector of points in relation to p0,

  the first point in the given vector of points should be p0 (the point to compare to)
  start is the first index of the section of the vector to sort, stop is 1 + the last index to sort
*/
void sort_points(vector<point2d>& pts, int start, int stop);

/*
  caller function for recursive mergesort sort_points()
*/
void sort_points(vector<point2d>& pts);

/* 
  finds convex hull
  given an array of points already sorted with p0 first and all sequential points forted radially by p0
  using graham scan method
*/
void build_hull(vector<point2d>& pts, vector<point2d>& hull);

// compute the convex hull, given a totally unsorted list of points pts
void graham_scan(vector<point2d>& pts, vector<point2d>& hull);
  

#endif
