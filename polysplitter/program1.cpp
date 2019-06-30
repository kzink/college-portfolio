// Name: Kevin Zink
// Assignment #1
// Computer Graphics - CSCI 4620

#include<stdio.h>
#include<GL/glut.h>
#include<iostream.h>
#include<vector>

using namespace std;

GLsizei winWidth = 350, winHeight = 350;	// init display window size

static const int MAX_VERTICES = 200;

// This class represents a polygon.
// It contains all the coordinate points that make up the polygon
class Polygon { 

    private:
        GLsizei vertices; // the number of vertices in this polygon
        GLfloat v[MAX_VERTICES][2]; // this stores the actual vertices

    public:

        // Default constructor - sets all elements in vertex array to 0
        // Polygon(void)
        // ------------------------------------------------------------
        //   input:  -
        //   return: -
        Polygon(void) {
            vertices = 0;
            for (int i = 0; i < MAX_VERTICES; i++) {
                v[i][0] = 0;
                v[i][1] = 0;
            }
        }

        // Default destructor
        // ~Polygon(void)
        // -------------------
        //   input:  -
        //   return: -
        ~Polygon(void) {
        }

        // This function adds an inputted vertex point
        // addPoint(floatx, float y)
        // -------------------------------------------
        //   input:  float x, float y
        //   return: -
        void addPoint(float x, float y) { 
            // no more than the max vertices should be necessary
            if (vertices > MAX_VERTICES) {
                printf("Error: trying to add point; too many vertices\n");
                exit(1);
            }
            v[vertices][0] = x;
            v[vertices][1] = y;
            vertices++;
        }

        // This function returns an X coordinate
        // getX(int index)
        // -------------------------------------
        //   input:  int index
        //   return: float
        float getX(int index) {
            return v[index][0];
        }

        // This function returns a Y coordinate
        // getY(int index)
        // ------------------------------------
        //   input:  int index
        //   return: float
        float getY(int index) {
            return v[index][1];
        }

        // This function initiates drawing of the polygon
        // draw()
        // ----------------------------------------------
        //   input:  -
        //   return: -
        void draw() {
            glVertexPointer(2, GL_FLOAT, 0, v);
            glDrawArrays(GL_POLYGON, 0, vertices);
        }

        // This function returns the size of the polygon
        // getSize(void)
        // ---------------------------------------------
        //   input:  -
        //   return: int
        GLsizei getSize(void) {
            return vertices;
        }

        // This function returns the cross product of the line segments created by the 
        // inputted point index and the next 2 sequential points
        // getCrossProduct(int i)
        // --------------------------------------------------------------------------
        //   input:  -
        //   return: int
        float getCrossProduct(int i) {
            int index1 = i;
            int index2 = (i + 1) % vertices;
            int index3 = (i + 2) % vertices;

            float cp = getX(index1) * (getY(index2) - getY(index3)) 
                + getX(index2) * (getY(index3) - getY(index1)) 
                + getX(index3) * (getY(index1) - getY(index2));
            //printf("Cross product of %d, %d = %f\n", index1, index2, cp);
            return cp;
        }

        // This function prints out al the points of this Polygon
        // printArray(void)
        // ------------------------------------------------------
        //   input:  -
        //   return: -
        void printArray(void) {
            for (int i = 0; i < vertices; ++i) {
                printf("#%d    {%f, %f}\n", i, getX(i), getY(i));
            }
            printf("\n");
        }
};

// This class represents a line segment.
// It contains 2 coordinate points to represent the line.
class Line {

    private:
        float point1 [2]; //point 1
        float point2 [2];

    public:

        // This is the default - it sets the points to 0
        // Line(void)
        // ---------------------------------------------
        // input:  -
        // return: -
        Line(void) {
            point1 [0] = 0;
            point1 [1] = 0;
            point2 [0] = 0;
            point2 [1] = 0;
        }

        // This is a constructor that takes 4 coordinates
        // Line(float xcoor1, float ycoor1, float xcoor2, float ycoor2)
        // ------------------------------------------------------------
        // input:  float, float, float, float
        // return: -
        Line(float xcoor1, float ycoor1, float xcoor2, float ycoor2) {
            point1 [0] = xcoor1;
            point1 [1] = ycoor1;
            point2 [0] = xcoor2;
            point2 [1] = ycoor2;
        }

        ~Line(void) {
        }

        // This function adds 4 coordinates
        // void add(float xcoor1, float ycoor1, float xcoor2, float ycoor2)
        // ----------------------------------------------------------------
        // input:  float, float, float, float
        // return: -
        void add(float xcoor1, float ycoor1, float xcoor2, float ycoor2) {
            point1 [0] = xcoor1;
            point1 [1] = ycoor1;
            point2 [0] = xcoor2;
            point2 [1] = ycoor2;
        }

        // This function returns the coordinates for point1
        // float * getPoint1(void)
        // ----------------------------------------------------------------
        // input:  -
        // return: float *
        float * getPoint1(void) {
            return point1;
        }

        // This function returns the coordinates for point2
        // float * getPoint2(void)
        // ----------------------------------------------------------------
        // input:  -
        // return: float *
        float * getPoint2(void) {
            return point2;
        }
};

vector<Polygon *> polygons; // vector of polygons
vector<Line *> lines; //vector of lines

//keeps track of which polygon is the first one (which will be filled green)
Polygon * firstPoly; 
float firstPolyPointOneX, firstPolyPointOneY, firstPolyPointTwoX, firstPolyPointTwoY;
bool foundGreen = false;

// This function initiates the window
void init(void)
{
    glClearColor (1.0, 1.0, 1.0, 1.0);		// white display window
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (0.0, winWidth, 0.0, winHeight);
}

// This function handles window resizing
void winReshapeFcn (GLint newWidth, GLint newHeight)
{
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));
    glClear(GL_COLOR_BUFFER_BIT);
}

// This function draws the polygons and lines
void displayFunc(void)
{
    glClear(GL_COLOR_BUFFER_BIT); //clear screen

    glColor3f (0.0, 1.0, 0.0); //the first polygon is green
    firstPoly->draw();

    glColor3f (1.0, 0.0, 0.0); //all remaining polygons are red
    for (int i = 0; i < polygons.size(); ++i) {
        if (polygons[i] != firstPoly) {
            polygons[i]->draw();
        }
    }

    // Cycle through vector of line segments and draw them
    glColor3f (0.0, 0.0, 0.0); //black
    glBegin(GL_LINES);
    for (i = 0; i < lines.size(); ++i) {
        glVertex2fv(lines[i]->getPoint1());
        glVertex2fv(lines[i]->getPoint2());
    }
    glEnd();

    glFlush();
}

// This function tests for concavity recursively
// It is passed a Polygon object to test
void testConcavity( Polygon * mPoly ) {

    for (int i = 0; i < mPoly->getSize(); ++i) {
        int index1 = i;
        int index2 = (i + 1) % mPoly->getSize();
        int index3 = (i + 2) % mPoly->getSize();
        int index4;

        //printf("Vertex Index %d = (%f, %f)\n", i, mPoly->getX(i), mPoly->getY(i));

        // If concavity is detected, split the polygon along this line segment
        if (mPoly->getCrossProduct(i) < 0) {
            //printf("Concavity detected between line segments from points #'s: %d and %d\n", index1, index2);

            float rmin = 0;
            int minIndex3, minIndex4;

            // This detects which line will be intersected and determines the point
            for (int j = 2; j < mPoly->getSize() - 1; ++j) {
                index3 = (i + j) % mPoly->getSize();
                index4 = (i + j + 1) % mPoly->getSize();

                float Ay = mPoly->getY(i);
                float Ax = mPoly->getX(i);
                float By = mPoly->getY(index2);
                float Bx = mPoly->getX(index2);
                float Cy = mPoly->getY(index3);
                float Cx = mPoly->getX(index3);
                float Dy = mPoly->getY(index4);
                float Dx = mPoly->getX(index4);
                float r, s;

                float denom = ( ( (Bx - Ax) * (Dy - Cy) ) -
                        ( (By - Ay) * (Dx - Cx) ) );

                //printf("denom = %f\n", denom);

                // if denom is not almost 0, determine r and s values
                if ( !(-0.01 < denom && denom < 0.01) ) {
                    /*------------------------------------------------------
                      (Ay-Cy)(Dx-Cx)-(Ax-Cx)(Dy-Cy)
                      r = -----------------------------  (eqn 1) MUST BE > 1
                      (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)
                      ------------------------------------------------------*/
                    r = ( ( (Ay - Cy) * (Dx - Cx) ) -
                            ( (Ax - Cx) * (Dy - Cy) ) ) / denom;

                    /*-----------------------------------------------------
                      (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
                      s = -----------------------------  (eqn 2) MUST BE BETWEEN 0 and 1
                      (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)
                      ------------------------------------------------------*/
                    s = ( ( (Ay - Cy) * (Bx - Ax) ) -
                            ( (Ax - Cx) * (By - Ay) ) ) / denom;

                    //special case
                } else {
                    float cp = Ax * (By - Cy) + Bx * (Cy - Ay) + Cx * (Ay - By);

                    if (cp == 0) {
                        float denom2 = (Bx - Ax), r1, r2;

                        if ( !(-0.01 < denom2 && denom2 < 0.01) ) {
                            r1 = (Cx - Ax) / (Bx - Ax);
                            r2 = (Dx - Ax) / (Bx - Ax);
                        } else {
                            r1 = (Cy - Ay) / (By - Ay);
                            r2 = (Dy - Ay) / (By - Ay);
                        }

                        //printf("r1 = %f, r2 = %f\n", r1, r2);

                        if (r1 > r2) {
                            r = r2;
                        } else { 
                            r = r1;
                        }

                        s = 0;
                    } else {
                        // snub values - ignore actual values
                        r = -1;
                        s = 0;
                    }
                }

                // If intersection Exists
                // printf ("%d, %f, %f\n", j, r, s);

                // Self-intersecting polygon detection
                if (r > 0 && r <= 1 && s >= 0 && s <= 1) {
                    printf("Unacceptable Input: Self-intersecting Polygon.\n");
                    //mPoly->printArray();
                    printf("Intersection between the line segments from point #'s: {%d, %d} and {%d, %d}.\n", index1, index2, index3, index4);
                    exit(-1);
                }

                // if a line intersection is detected
                if ( r > 1 && s >= 0 && s < 1) {
                    //we want to use the line with the smallest r value
                    //if rmin equals 0, we haven't set an rmin yet (initial condition case)
                    if (rmin == 0 || r < rmin) {
                        rmin = r;
                        minIndex3 = index3;  //keep track of this line segment's vertices
                        minIndex4 = index4;
                    }
                }
            }

            // split the polygon if a line intersection was found
            if (rmin != 0) {
                //printf("rmin = %f\n", rmin);
                index3 = minIndex3;
                index4 = minIndex4;
                float r = rmin;

                float Ay = mPoly->getY(i);
                float Ax = mPoly->getX(i);
                float By = mPoly->getY(index2);
                float Bx = mPoly->getX(index2);

                //printf("Intersection determined between line segments from point #'s: %d, %d to %d, %d\n", 
                //        index1, index2, index3, index4);

                /*DETERMINATION OF INTERSECTION POINT
                  -----------------------------------------------------
                  Let P be the position vector of the intersection point, then
                  Px and Py are the points of the intersection

                  Px=Ax+r(Bx-Ax)
                  Py=Ay+r(By-Ay)
                  -----------------------------------------------------*/
                float Px = Ax + r * (Bx - Ax);
                float Py = Ay + r * (By - Ay);
                //printf("Intersection point is at (%f, %f)\n", Px / 30, Py / 30);

                //Add line segment to lines vector
                lines.push_back(new Line(mPoly->getX(index2), mPoly->getY(index2), Px, Py));

                //Construction of 2 separate Polygons (split from the main polygon)
                Polygon * poly1 = new Polygon();
                Polygon * poly2 = new Polygon();

                // Points for Polygon #1
                poly1->addPoint(mPoly->getX(index1), mPoly->getY(index1)); //point 1
                poly1->addPoint(Px, Py); //point2 is the intersection point
                // remaining points for polygon 1
                for (int k = index4; k != index1; k = (k + 1) % mPoly->getSize()) {
                    poly1->addPoint(mPoly->getX(k), mPoly->getY(k));  
                }

                // Points for Polygon #2
                poly2->addPoint(mPoly->getX(index2), mPoly->getY(index2)); //starting point for poly2
                for (k = (index2 + 1) % mPoly->getSize(); k != index4; k = (k + 1) % mPoly->getSize()) {
                    //printf("k={%f, %f} P={%f, %f} k=%d mPolysize=%d\n", mPoly->getX(k) / 30, mPoly->getY(k) / 30, Px / 30, Py / 30, k, mPoly->getSize());

                    // we don't add a point if it is the same as the intersection point (it's added later)
                    if (mPoly->getX(k) == Px && mPoly->getY(k) == Py) {
                        //printf("Px = x, Py = y\n");
                    } else {
                        poly2->addPoint(mPoly->getX(k), mPoly->getY(k));
                    }
                }
                poly2->addPoint(Px, Py); //the end point for poly2 is the intersection point

                //poly1->printArray();
                //poly2->printArray();

                // This recursively calls the concavity test on the newly created polygons
                testConcavity(poly1);
                testConcavity(poly2);

                delete mPoly; //delete the original polygon that's no longer needed from the heap

                return; //break out of function
            } 
            //printf("\n\n");
        }
    }

    // If no concavity is detected, this polygon is ready to be added to the polygon vector
    polygons.push_back(mPoly);

    //printf("Adding polygon to vector\n");
}

void main(int argc, char **argv) {

    Polygon * mPoly = new Polygon(); //starting polygon
    int numVertices = 0; //holds total number of vertices

    printf ("Input the number of vertices to display and a coordinate for each vertex.\n");
    printf ("Syntax: N x1 y1 x2 y2 ...\nExample Input: 3 100 50 300 300 300 50\n\nInput: ");
    scanf("%d", &numVertices);

    if (numVertices < 3 || numVertices > MAX_VERTICES) {
        printf( "Unacceptable Input: the number you entered, %d, is not valid.\n The number of vertices must be between the values of 3 and 20.\n", 
                numVertices);
        exit(1);
    }

    float x = 0, y = 0;
    int xmax = 0, ymax = 0;
    //printf ("Input the coordinates for each vertex in the format x1 y1 x2 y2 ... : ");

    for (int i = 0; i < numVertices; ++i) {
        if (scanf("%f", &x) != 1) {
            printf("Error reading vertex index %d\n", i);
            exit(1);
        }
        if (scanf("%f", &y) != 1) {
            printf("Error reading vertex index %d\n", i);
            exit(1);
        }

        // This keep track of the first line segment's points
        if (i == 0) {
            firstPolyPointOneX = x;
            firstPolyPointOneY = y;
            //printf("Point 1: {%f, %f}\n", firstPolyPointOneX / 30, firstPolyPointOneY / 30);
        }
        if (i == 1) {
            firstPolyPointTwoX = x;
            firstPolyPointTwoY = y;
            //printf("Point 2: {%f, %f}\n", firstPolyPointTwoX / 30, firstPolyPointTwoY / 30);
        }

        // keeps track of maximum x and y values to alter the window size.
        if (x > xmax) {
            xmax = x;
        }
        if (y > ymax) {
            ymax = y;
        }

        // This checks the bounds of the inputted vertex
        if (x < 0) {
            printf("X coordinate, %f, is out of bounds - point must be greater than 0.\n", x);
            exit(1);
        }
        if (y < 0) {
            printf("Y coordinate, %f, is out of bounds - point must be greater than 0.\n", y);
            exit(1);
        }

        mPoly->addPoint(x, y); //add point to the polygon
    } 

    // This tests the polygon for duplicate vertices
    for (i = 0; i < mPoly->getSize(); ++i) {
        for (int j = i + 1; j < mPoly->getSize(); ++j) {
            if (mPoly->getX(i) == mPoly->getX(j) && mPoly->getY(i) == mPoly->getY(j)) {
                printf("Unacceptable Input: repeated vertices detected between point #'s: %d and %d\n", i, j);
                exit(-1);
            }
        }
    }

    // This tests for collinear vertices
    for (i = 0; i < mPoly->getSize(); ++i) {
        int index1 = i;
        int index2 = (i + 1) % mPoly->getSize();
        int index3 = (i + 2) % mPoly->getSize();

        // if collinear vertices are found
        if (mPoly->getCrossProduct(i) == 0) {
            printf("Unacceptable Input: collinear vertex detected between points #'s: %d, %d, and %d\n.", index1, index2, index3);
            exit(-1);
        }
    }

    // This tests the polygon recursively for concavity
    testConcavity(mPoly);

    //for (i = 0; i < polygons.size(); ++i) {
    //    polygons[i]->printArray();
    //}

    // This tests to see which polygon should be colored green.
    // We check to see the polygon contains the first inputted coordinate, then we check to see if
    // the polygon contains a line segment that is collinear with the second inputted coordinate. 
    for (i = 0; i < polygons.size() && !foundGreen; i++) {
        for (int j = 0; j < polygons[i]->getSize() && !foundGreen; j++) {
            //if the polygon contains the first inputted coordinate
            if ( (firstPolyPointOneX == polygons[i]->getX(j)) && (firstPolyPointOneY == polygons[i]->getY(j)) ) {
                int index1 = j;
                int index3 = (j + 1) % polygons[i]->getSize();

                float cp = polygons[i]->getX(index1) * (firstPolyPointTwoY - polygons[i]->getY(index3)) 
                    + firstPolyPointTwoX * (polygons[i]->getY(index3) - polygons[i]->getY(index1)) 
                    + polygons[i]->getX(index3) * (polygons[i]->getY(index1) - firstPolyPointTwoY);

                //printf("CP = %f\n", cp);

                // if a collinear case is found, we know this will be the green polygon.
                if (cp < .01 && cp > -.01) {
                    //printf("Found green polygon via cross product between points %d, %d\n", index1, index3);
                    firstPoly = polygons[i];
                    foundGreen = true;
                }
            }
        }
    }

    // If no green polygon is found for whatever reason, it is set to the first polygon in the vector.
    if (!foundGreen) {
        firstPoly = polygons[0];
    }

    // set window x and y maximum
    winWidth = xmax + 20;
    winHeight = ymax + 50;
    //printf("winWidth = %d, winHeight = %d\n", winWidth, winHeight);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow ("Concave Polygon Splitter");
    glEnableClientState (GL_VERTEX_ARRAY);

    // Final output
    printf("\nTotal number of lines drawn: %d.\nTotal number of polygons drawn: %d.\n", lines.size(), polygons.size());

    // Drawing of Polygons
    init();
    glutDisplayFunc(displayFunc);
    glutReshapeFunc(winReshapeFcn);
    glutMainLoop();
}
