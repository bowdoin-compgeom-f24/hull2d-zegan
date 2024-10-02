#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include <vector>

using namespace std; 

//threshold for double comparison to zero
double MARGIN = 0.000001; //10^(-6)

/* **************************************** */
/* returns the signed area of triangle abc. The area is positive if c
   is to the left of ab, and negative if c is to the right of ab
 */
int signed_area2D(point2d a, point2d b, point2d c) {
  int Ax = b.x - a.x;
  int Ay = b.y - a.y;
  int Bx = c.x - a.x;
  int By = c.y - a.y;
  return (Ax * By) - (Ay * Bx); 
}

/* **************************************** */
/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2d p, point2d q, point2d r) {
  //for double comparison == 0 ?, consider a very small interval around 0
  double result = signed_area2D(p,q,r);
  return (result < MARGIN) && (result > -MARGIN); // ==0 0
}

/* **************************************** */
/* return 1 if c is  strictly left of ab; 0 otherwise */
int left_strictly(point2d a, point2d b, point2d c) {
  //for double comparison to 0, consider a very small interval around 0 to be == 0
  return signed_area2D(a,b,c) > MARGIN; // > 0
}


/* return 1 if c is left of ab or on ab; 0 otherwise */
int left_on(point2d a, point2d b, point2d c) {
  //for double comparison to 0, consider a very small interval around 0 to be == 0
  return signed_area2D(a,b,c) > -MARGIN; // >= 0
}

/*
  finds a point on the hull from a vector of points pts
  selects the point with the lowest y value
  if there are multiple points with the lowest y value, 
    it selects the point with the highest x value among them
*/
int find_bottom_point(vector<point2d>& pts){
  int indexP0 = 0;
  point2d p0 = pts[0];
  for (int i=1; i < pts.size(); i++){
    point2d Pi = pts[i];
    if (Pi.y < p0.y){
      p0 = Pi;
      indexP0 = i;
    }else if(Pi.y == p0.y){ //if tied, pick rightmost point
      if (Pi.x > p0.x){
        p0 = Pi;
        indexP0 = i;
      }
    }
  }
  return indexP0;
}


/*
  the merge function for a mergesort sorting radially in counterclockwise order with respect to p0
  start and p1stop are the indices in pts_unmerged where sorted p1 and p2 begin
  p2_stop is the index in pts_sorted after p2 ends

  pts_unsorted[0] should be p0 (the point that we are sorting by)
*/
void merge_points(vector<point2d>& pts_unmerged, int start, int p1_stop, int p2_stop){
  vector<point2d> pts_merged;
  //copy pts_merged into pts_unmerged
  for (int i = start; i < p2_stop; i++){
    point2d p;
    p.x = pts_unmerged[i].x;
    p.y = pts_unmerged[i].y;
    pts_merged.push_back(p);
  }

  int next_p1 = start; //pointer to pts_unsorted where next p1 point is
  int next_p2 = p1_stop; //pointer to pts_unsorted where next p2 point is
  point2d p0 = pts_unmerged[0];
  for (int i = 0; i < (p2_stop - start); i++){
    
    if ((next_p1 < p1_stop) && (next_p2 < p2_stop)){
      if (left_strictly(p0, pts_unmerged[next_p1], pts_unmerged[next_p2])){
        pts_merged[i].x = pts_unmerged[next_p1].x;
        pts_merged[i].y = pts_unmerged[next_p1].y;
        next_p1 += 1;
      }else if (collinear(p0, pts_unmerged[next_p1], pts_unmerged[next_p2])){
      //if two points are collinear with p0, then sort by their x and y distance from p0
        if (abs(p0.x - pts_unmerged[next_p1].x) < abs(p0.x - pts_unmerged[next_p2].x)){
          pts_merged[i].x = pts_unmerged[next_p1].x;
          pts_merged[i].y = pts_unmerged[next_p1].y;
          next_p1 += 1;
          //NOTE: another option would be do delete the closer point, but this would require reallocating the array
        } else if (abs(p0.y - pts_unmerged[next_p1].y) < abs(p0.y - pts_unmerged[next_p2].y)){ //if the points have the same x
          pts_merged[i].x = pts_unmerged[next_p1].x;
          pts_merged[i].y = pts_unmerged[next_p1].y;
          next_p1 += 1;
        } else{ // p2 point is closer than p1 point
          pts_merged[i].x = pts_unmerged[next_p2].x;
          pts_merged[i].y = pts_unmerged[next_p2].y;
          next_p2 += 1;
        }
      } else{ //p1 point is left of p2
        pts_merged[i].x = pts_unmerged[next_p2].x;
          pts_merged[i].y = pts_unmerged[next_p2].y;
        next_p2 += 1;
      }
    } else if (next_p1 < p1_stop){ //only points left in P1
      pts_merged[i].x = pts_unmerged[next_p1].x;
      pts_merged[i].y = pts_unmerged[next_p1].y;
      next_p1 += 1;
    } else{ //if (next_p2 < p2_stop) //only points left in P2
      pts_merged[i].x = pts_unmerged[next_p2].x;
      pts_merged[i].y = pts_unmerged[next_p2].y;
      next_p2 += 1;
    }
  }

  //copy pts_merged into pts_unmerged
  for (int i = 0; i < (p2_stop - start); i++){
    pts_unmerged[start + i].x = pts_merged[i].x;
    pts_unmerged[start + i].y = pts_merged[i].y;
  }
}

/*
  recursive sort helper function
  
  radially sort all other points in a vector of points in relation to p0,
    using mergesort
  the first point in the given vector of points should be p0 (the point to compare to)

  start is the first index of the section of the vector to sort, stop is 1 + the last index to sort
*/
void sort_points(vector<point2d>& pts, int start, int stop){
  //base case: only 1 or 0 points (excluding P0)
  if ((stop - start) < 2){
    return;
  } else { //recursive case: recurse on 2 halves then merge
    int midpoint = start + ((stop - start) / 2);
    sort_points(pts, start, midpoint);
    sort_points(pts, midpoint, stop);
    merge_points(pts, start, midpoint, stop);
  }
}

/*
  caller function for recursive mergesort sort_points()
    which radially sorts points with respect to p0
*/
void sort_points(vector<point2d>& pts){
  sort_points(pts, 1, pts.size());
}

/* 
  finds convex hull
  given an array of points already sorted with p0 first and all sequential points forted radially by p0
  using graham scan method
*/
void build_hull(vector<point2d>& pts, vector<point2d>& hull){
  hull.push_back(pts[0]); //add p0 to the hull
  hull.push_back(pts[1]); //push the next point onto the hull
  int i = 2;
  while(i < pts.size()){
    if (left_strictly(hull[hull.size()-2], hull[hull.size()-1], pts[i])){
      hull.push_back(pts[i]);
      i++;
    }else{
      while (!left_strictly(hull[hull.size()-2], hull[hull.size()-1], pts[i])){
        hull.pop_back();
        //NOTE: collinear points have been sorted in order going out, so the later point should be kept on the hull
        //and the previous point should be removed (so it is the same procedure as for convex --> right-of)
      }
    }
  }
  //finally, check the last point with the first point on the hull:
  while(hull.size() > 2 && !left_strictly(hull[hull.size()-2], hull[hull.size()-1], hull[0])){
    hull.pop_back();
  }
}

/*
  finds the extreme points in the x and y directions
  and returns a vector of points with only these and the points
  OUTSIDE of the quadrilateral that they create
*/
vector<point2d> delete_middle_points(vector<point2d>& pts){
  vector<point2d> pts_extreme;
  vector<point2d> pts_outside;
  int i_xmax = 0;
  int i_xmin = 0;
  int i_ymax = 0;
  int i_ymin = 0;
  int xmax = pts[0].x;
  int xmin = pts[0].x;
  int ymax = pts[0].y;
  int ymin = pts[0].y;
  for (int i = 1; i < pts.size(); i++){
    int px = pts[i].x;
    int py = pts[i].y;
    if (px > xmax){
      i_xmax = i;
      xmax = px;
    }
    if (px < xmin){
      i_xmin = i;
      xmin = px;
    }
    if (py > ymax){
      i_ymax = i;
      ymax = py;
    }
    if (py < ymin){
      i_ymin = i;
      ymin = py;
    }
  }
  //push these four points on counterclockwise
  //there may be duplicates, and that is okay
  pts_extreme.push_back(pts[i_xmax]);
  pts_extreme.push_back(pts[i_ymax]);
  pts_extreme.push_back(pts[i_xmin]);
  pts_extreme.push_back(pts[i_ymin]);
  //and push onto included points vector
  for (int i = 0; i < pts_extreme.size(); i++){
    pts_outside.push_back(pts_extreme[i]);
  }
  
  //iterate through every point in the whole collection, to add included points to pts_outside
  for (int i = 0; i < pts.size(); i++){
    point2d p = pts[i];
    //walk around the quadrilateral (or triangle or line) CCW and include only points strictly outside one of the edges
    //if there are duplicate points in the pts_extreme collection, nothing will happen since left_on will
        //always return false since signed_area will always return 0 (as if point c is "on" the line)
    for (int j = 0; j < pts_extreme.size(); j++){
      int jnext = (j + 1) % pts_extreme.size();
      if (!left_on(pts_extreme[j], pts_extreme[jnext], p)){
        pts_outside.push_back(p);
        continue;
      }
    }
  }

  return pts_outside;
}

// compute the convex hull of pts, and store the points on the hull in hull
void graham_scan(vector<point2d>& pts, vector<point2d>& hull ) {

  printf("hull2d (graham scan): start\n"); 
  hull.clear(); //should be empty, but clear it to be safe

  //remove points cointained within the quadrilateral (or triangle) with points at x and y extremes
  vector<point2d> pts_include = delete_middle_points(pts);
  //vector<point2d> pts_include = pts;
  
  //find bottommost point p0
  int indexP0 = find_bottom_point(pts_include);
  //to move P0 to the front of the vector, swap with the first point:
  int p0x = pts_include[indexP0].x; //save copy of x and y of P0
  int p0y = pts_include[indexP0].y;
  pts_include[indexP0].x = pts_include[0].x; //overwrite x and y of P0 with pts[0]
  pts_include[indexP0].y = pts_include[0].y;
  pts_include[0].x = p0x; //copy saved values back into 0th position
  pts_include[0].y = p0y;
  
  //radially sort all other points in relation to p0
  sort_points(pts_include);

  build_hull(pts_include, hull);

  

  printf("hull2d (graham scan): end\n"); 
  return; 
}

