# hull2d-startup

This program comes up with a a random set of n points within a bounding box in 2D space, then uses graham scan to compute the convex hull. The graham scan uses a manually implemented mergesort function
The points and the hull are visualized using glut graphics.

## TO RUN:
compile with "make"
then populate n points and build the hull for them by calling ./hull2d n


## TO USE NON-RANDOM POINT CONFIGURATIONS:
To use different initializers (instead of randomly populating n points)
    uncomment the initializer function call of choice
    within the main function in viewhull.cpp:
//populate the points 
  //initialize_points_random(points, NPOINTS);
  //initialize_points_circle(points, NPOINTS);
  //initialize_points_cross(points, NPOINTS);
  //initialize_points_horizontal_line(points, NPOINTS);
  initialize_points_heart(points, NPOINTS);
  //initialize_points_square(points, NPOINTS);
  //initialize_points_diamond(points, NPOINTS);
  //initialize_points_two_vertical(points, NPOINTS); //from Victoria and Ziyou
  //initialize_points_vertical_line(points, NPOINTS); //from Victoria and Ziyou
  //initialize_points_1(points, NPOINTS); //from Tom
  //initialize_points_2(points, NPOINTS); //from Tom
  //initialize_points_triangle(points, NPOINTS); //from Abhi and Max
  //print_vector("points:", points);

 
