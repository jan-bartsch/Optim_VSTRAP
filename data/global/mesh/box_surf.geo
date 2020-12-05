// Gmsh project created on Wed Aug  1 12:56:42 2018 Version 3.0.6-Linux64
SetFactory("OpenCASCADE");

boxSize = 100;
division = 6;
gridSize = boxSize/division;

Point(1) = {-boxSize/2,-boxSize/2,-boxSize/2, gridSize};
Point(2) = { boxSize/2,-boxSize/2,-boxSize/2, gridSize};
Point(3) = { boxSize/2, boxSize/2,-boxSize/2, gridSize};
Point(4) = {-boxSize/2, boxSize/2,-boxSize/2, gridSize};
Point(5) = {-boxSize/2,-boxSize/2, boxSize/2, gridSize};
Point(6) = { boxSize/2,-boxSize/2, boxSize/2, gridSize};
Point(7) = { boxSize/2, boxSize/2, boxSize/2, gridSize};
Point(8) = {-boxSize/2, boxSize/2, boxSize/2, gridSize};

Line (1) = {1,2};
Line (2) = {2,3};
Line (3) = {3,4};
Line (4) = {4,1};
Line (5) = {5,6};
Line (6) = {6,7};
Line (7) = {7,8};
Line (8) = {8,5};
Line (9) = {1,5};
Line (10) = {2,6};
Line (11) = {3,7};
Line (12) = {4,8};

Line Loop(1) = {1,2,3,4};
Line Loop(2) = {5,6,7,8};
Line Loop(3) = {1,10,5,9};
Line Loop(4) = {2,11,6,10};
Line Loop(5) = {3,12,7,11};
Line Loop(6) = {4,9,8,12};

Plane Surface(1) = {1};
Plane Surface(2) = {2};
Plane Surface(3) = {3};
Plane Surface(4) = {4};
Plane Surface(5) = {5};
Plane Surface(6) = {6};

Reverse Surface{1};

Transfinite Surface {1} = {4,3,2,1};
Transfinite Surface {2} = {5,6,7,8};
Transfinite Surface {3} = {1,2,6,5};
Transfinite Surface {4} = {2,3,7,6};
Transfinite Surface {5} = {3,4,8,7};
Transfinite Surface {6} = {4,1,5,8};

Physical Surface ("Plus", 1) = {1};
Physical Surface ("Minus", 2) = {2};
Physical Surface ("Other1", 3) = {3};
Physical Surface ("Other2", 4) = {4};
Physical Surface ("Other3", 5) = {5};
Physical Surface ("Other4", 6) = {6};

