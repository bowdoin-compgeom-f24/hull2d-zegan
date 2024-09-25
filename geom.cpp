#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include <vector>

using namespace std; 

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
  return signed_area2D(p,q,r) == 0; 
}

/* **************************************** */
/* return 1 if c is  strictly left of ab; 0 otherwise */
int left_strictly(point2d a, point2d b, point2d c) {
  return signed_area2D(a,b,c) > 0; 
}


/* return 1 if c is left of ab or on ab; 0 otherwise */
int left_on(point2d a, point2d b, point2d c) {
  return signed_area2D(a,b,c) >= 0; 
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
  pts_unsorted is where sorted p1 and p2 are held
  start and p1stop are the indices in pts_sorted where sorted p1 and p2 begin
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
  radially sort all other points in a vector of points in relation to p0,
    using mergesort
  the first point in the given vector of points should be p0 (the point to compare to)
*/
void sort_points(vector<point2d>& pts){
  sort_points(pts, 1, pts.size());
}

/* 
  this helper method does the graham scan, once the points are sorted radially by p0
  in the given pts array, p0 should be the first point and all following points
    should be sorted radially in relation to p0
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
}

// compute the convex hull of pts, and store the points on the hull in hull
void graham_scan(vector<point2d>& pts, vector<point2d>& hull ) {

  printf("hull2d (graham scan): start\n"); 
  hull.clear(); //should be empty, but clear it to be safe

  /*
  TODO: REMOVE POINTS IN THE MIDDLE OF THE RECTANGLE INSIDE FOR EFFICIENCY
  
  
  */

  //find bottommost point p0
  int indexP0 = find_bottom_point(pts);

  //TODO: HOW TO MOVE P0 TO THE FIRST POSITION IN THE VECTOR????
  //std::rotate(indexP0, indexP0 + 1, pts.begin()) //Stack Overflow source: https://stackoverflow.com/questions/23789498/moving-a-vector-element-to-the-back-of-the-vector
  //^^NOT WORKING
  //pts.erase(indexP0);
  //^^ALSO NOT WORKING
  // patch fix for now:
  
  vector<point2d> pts_p0_first;
  pts_p0_first.clear();
  pts_p0_first.push_back(pts[indexP0]);
  for (int i = 0; i <  pts.size(); i++){
    if (i != indexP0){
      pts_p0_first.push_back(pts[i]);
    }
  }
  
  //radially sort all other points in relation to p0
  //pts = sort_points(pts, indexP0);
  sort_points(pts_p0_first);

  build_hull(pts_p0_first, hull);

  printf("hull2d (graham scan): end\n"); 
  return; 
}

