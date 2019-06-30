// Name: Kevin Zink
// Program 3 - simple drawing program
// Computer Graphics - CSCI 4620


#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream.h>
#include <fstream>
#include <iomanip>

using namespace std;

#define MAXVERTICES 500
#define MAXWIDTH 10

GLsizei winWidth = 700, winHeight = 700; // init display window size

/****************************************/
/* Class that represents Line Segements */
/****************************************/
class Line {

    private:
        float point1 [2]; //point 1
        float point2 [2];
        int lineWidth; //stores the width of the line
        float lineColor[3];

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
            lineWidth = 1;
            lineColor[0] = 1.0;
            lineColor[1] = 1.0;
            lineColor[2] = 1.0;
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
            lineWidth = 1;
            lineColor[0] = 1.0;
            lineColor[1] = 1.0;
            lineColor[2] = 1.0;
        }

        ~Line(void) {
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

        // This function is used to overwrite the existing points
        // float * setPoints2D(float x1, float y1, float x2, float y2)
        // ----------------------------------------------------------------
        // input:  float x1, float y1, float x2, float y2
        // return:
        void setPoints2D(float x1, float y1, float x2, float y2) {
            point1 [0] = x1;
            point1 [1] = y1;
            point2 [0] = x2;
            point2 [1] = y2;
        }

        // These functions set individual points for the line segment
        void setPoint1X(float x) {
            point1 [0] = x;
        }

        void setPoint1Y(float y) {
            point1 [1] = y;
        }

        void setPoint2X(float x) {
            point2 [0] = x;
        }

        void setPoint2Y(float y) {
            point2 [1] = y;
        }

        void setWidth(int x) {
            lineWidth = x;
        }

        int getWidth(void) {
            return lineWidth;
        }

        void setColor(float r, float g, float b) {
            lineColor[0] = r;
            lineColor[1] = g;
            lineColor[2] = b;
        }

        float * getColor(void) {
            return lineColor;
        }

        // draw the Line segment
        void draw(void) {

            glLineWidth(lineWidth); //set width
         
            glColor3f (lineColor[0], lineColor[1], lineColor[2]); //set color

            glBegin(GL_LINES);
                glVertex2fv(getPoint1());
                glVertex2fv(getPoint2());
            glEnd();
        }

        // draw the Line segment with highlight
        void drawSelected(void) {

            glLineWidth(lineWidth); //set width

            glColor3f (0.90, 0.95, 0.0); //set color to noticeable bright yellow color

            glBegin(GL_LINES);
                glVertex2fv(getPoint1());
                glVertex2fv(getPoint2());
            glEnd();
        }

        void print(void) {
            printf("Point 1 X: %f\n", getPoint1()[0]);
            printf("Point 1 Y: %f\n", getPoint1()[1]);
            printf("Point 2 X: %f\n", getPoint2()[0]);
            printf("Point 2 Y: %f\n", getPoint2()[1]);
            printf("Line width: %d\n", getWidth());
            printf("Color in RGB: (%f, %f, %f)\n", lineColor[0], lineColor[1], lineColor[2]);
        }
};

/***********************************/
/* Initial coordinates for the box */
/***********************************/
vector<Line *> vect; // vector that store lines
Line * unpairedLine; // temporary line segment
typedef vector<Line *>::iterator lineIter;
lineIter selectedLine = NULL; // selected line
bool clickedOnce = false; // toggle for mouse button clicks
int nvertices = 0; // number of vertices
int width = 1; // initial line width
int mainMenuId; // main menu identifier
int colorsMenuId; // colors menu identifier
float currentColor[3] = {0.0, 0.0, 0.0}; //stores currently selected color - defaults to black
char * fileName = NULL; //stores filename - defaults to drawing.txt
double xPosition, yPosition;

/***********************************/
/*      Function Prototypes        */
/***********************************/
void saveFile(void);
void init(void);
void draw (void);
void winReshapeFcn (GLint, GLint);
void mf(int, int, int, int);
void help(void);
void kf(unsigned char, int, int);
void MenuFn(int);
void ColorFn(int);
void openFile(void);
void resetLineAttributes(void);
void clearScreen(void);
double ptSegDistSq(double, double, double, double, double, double);
void selectLine(void);
void deleteSelectedLine(void);
void moveLine(double, double);

void init(void) {
    glClearColor (1.0, 1.0, 1.0, 1.0);		// white display window
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (0.0, 1.0, 0.0, 1.0);
}

// This is the draw function
void draw(void) {
    glClear (GL_COLOR_BUFFER_BIT);		// clear display window

    // Draw the lines
    for (lineIter iter = vect.begin(); iter != vect.end(); ++iter) {
        
        if (iter == selectedLine) {
            // if the line segment is selected, color it highlighted
            (*iter)->drawSelected();
        } else {
            (*iter)->draw();
        }
    }

    glFlush();
}

void winReshapeFcn (GLint newWidth, GLint newHeight) {
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0.0, 1.0, 0.0, 1.0);
    winWidth = newWidth;
    winHeight = newHeight;
    glClear(GL_COLOR_BUFFER_BIT);
}

// This function sets the current mouse position
void mousePosition(int x, int y) {
    xPosition = 1.0 * x / winWidth;
    yPosition = 1.0 - 1.0 * y / winHeight;
}

// This function handles mouse clicks
void mf(int button, int state, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON) 
        return;
    if (state != GLUT_DOWN) 
        return;
    //if (nvertices == MAXVERTICES) 
    //    return;

    if (clickedOnce == false) {
        // set the coordinates of the first point in a temporary Line
        unpairedLine = new Line();
        unpairedLine->setPoint1X(1.0 * x / winWidth);
        unpairedLine->setPoint1Y(1.0 - 1.0 * y / winHeight);
        clickedOnce = true;

    } else if (clickedOnce == true) {
        // set the coordinates of the second point in a temporary Line, 
        // add it to the Line vector, and display it
        unpairedLine->setPoint2X(1.0 * x / winWidth);
        unpairedLine->setPoint2Y(1.0 - 1.0 * y / winHeight);
        
        unpairedLine->setWidth(width); //set width of this line 
        unpairedLine->setColor(currentColor[0], currentColor[1], currentColor[2]); //set color
        vect.push_back(unpairedLine); //add to vector

        //printf("----------Adding Line Segment with Properties----------\n");
        //unpairedLine->print();

        clickedOnce = false; //toggle "clicked" flag
      
        glutPostRedisplay(); //re-display
    }

    nvertices++; //increment vertice #
}

// This function outputs keyboard commands to the console
void help(void) {
    cout << "------Keyboard Command list------" << endl;
    cout << "S or s : Save drawing to file" << endl;
    cout << "R or r : Load drawing from file" << endl;
    cout << "S or s : Save the file" << endl;
    cout << "C or c : Clear the screen" << endl;
    cout << "t : Decrease line width" << endl;
    cout << "spacebar : Select line" << endl;
    cout << "D or d : Delete selected line" << endl;
    cout << "t : decrease line width" << endl;
    cout << "shift-t : Increase line width" << endl;
    cout << "X or x : Close program" << endl;
    cout << "H or h : Show keyboard commands" << endl;
}

void special(int key, int x, int y) {
    float delta;

    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
        delta = 0.005;
    } else {
        delta = 0.05;
    }

    switch(key) {

    case GLUT_KEY_UP:
	    moveLine(0.0, delta);
	    return;
	case GLUT_KEY_DOWN:
	    moveLine(0.0, -delta);
	    return;
	case GLUT_KEY_LEFT:
	    moveLine(-delta, 0.0);
	    return;
	case GLUT_KEY_RIGHT:
	    moveLine(delta, 0.0);
	    return;
	default:
	    return;
    }
}

void kf(unsigned char key, int x, int y)
{
    switch(key) {

    case ' ':
        //cout << "finding line" << endl;
        selectLine();
        return;

    case 0x1b: //escape key
        selectedLine = NULL;
        glutPostRedisplay(); //re-display
        return;

    case 'd':
    case 'D':
        deleteSelectedLine();
        return;

    case 's':
	case 'S':
        saveFile();
        return;

    case 'r':
	case 'R':
        openFile();
        return;

    case 'c':
	case 'C':
        clearScreen();
        return;

	case 'h':
	case 'H':
	    help();
	    return;

	case 't':			// make lines thinner
        if (width > 1) {
            width--;
            cout << "Decrementing Width to: " << width << endl;
        }
	    return;

	case 'T':			// make lines thicker
        if (width < MAXWIDTH) {
            width++;
            cout << "Incrementing Width to: " << width << endl;
        }
	    return;

	case 'x':			// terminate the program
	case 'X':
	    exit(0);

	default:
	    return;			// ignore the key
    }
}

void openFile(void) {

    char input;

    if (fileName == NULL) {
        cout << "No file specified - clearing screen" << endl;
        clearScreen();
        return;
    }

    ifstream hobi (fileName); //gets name of file to use
    clearScreen();
    resetLineAttributes();
    Line * newLine;
        
    while (hobi >> input) {

        if (input == 'c') {
            // Alter current color
            hobi >> currentColor[0] >> currentColor[1] >> currentColor[2];

        } else if (input == 'l') {
            // Insert line coordinates
            float x1, y1, x2, y2;
            hobi >> x1 >> y1 >> x2 >> y2;

            newLine = new Line(x1, y1, x2, y2);
            newLine->setWidth(width); //set width of this line 
            newLine->setColor(currentColor[0], currentColor[1], currentColor[2]); //set color

            vect.push_back(newLine); //add line to vector
  
        } else if (input == 'w') {
            // Alter width
            hobi >> width;
        
        } else {
            // finish file opening operation

            hobi.close();   //close filestream
            resetLineAttributes(); //reset width and color to defaults
            glutPostRedisplay(); //re-display
        
            cout << "Loaded drawing from file: " << fileName << endl;

            return;
        }
    }
}

void saveFile(void) {

    if (fileName == NULL) {
        cout << "No file specified: default file will be drawing.txt" << endl;
        fileName = "drawing.txt";
    }

    ofstream hobo (fileName); //gets name of file to use

    resetLineAttributes(); //reset current color and width for comparisons

    for (int i = 0; i < vect.size(); ++i) {

        // compare width values and add a width change line if necessary
        if (width != vect[i]->getWidth()) {
            hobo << "w " << vect[i]->getWidth() << endl;
        }

        // compare color values and add a color change line if necessary in format "c <r> <g> <b>"
        if (currentColor[0] != vect[i]->getColor()[0] || 
            currentColor[1] != vect[i]->getColor()[1] || 
            currentColor[2] != vect[i]->getColor()[2]) {
            hobo << "c " << vect[i]->getColor()[0] << " " 
                         << vect[i]->getColor()[1] << " " 
                         << vect[i]->getColor()[2] << endl;
        }
    
        // save end points
        hobo << "l " << vect[i]->getPoint1()[0] << " "
                     << vect[i]->getPoint1()[1] << " "
                     << vect[i]->getPoint2()[0] << " "
                     << vect[i]->getPoint2()[1] << endl;

        // cycle current color and width for next comparison
        currentColor[0] = vect[i]->getColor()[0];
        currentColor[1] = vect[i]->getColor()[1];
        currentColor[2] = vect[i]->getColor()[2];
        width = vect[i]->getWidth();
    }

    hobo << "e" << endl; //end file with "e" delimeter

    hobo.close();   //close filestream

    cout << "Saved drawing to file: " << fileName << endl;
}

// This function moves a line segment based on which direction the user presses
void moveLine(double dx, double dy) {
    if (selectedLine != NULL) {
        // Check the bounds of the potential moving
        if ((*selectedLine)->getPoint1()[0] + dx > 1.0 || (*selectedLine)->getPoint1()[0] + dx < 0.0 ||
            (*selectedLine)->getPoint1()[1] + dy > 1.0 || (*selectedLine)->getPoint1()[1] + dy < 0.0 ||
            (*selectedLine)->getPoint2()[0] + dx > 1.0 || (*selectedLine)->getPoint2()[0] + dx < 0.0 ||
            (*selectedLine)->getPoint2()[1] + dy > 1.0 || (*selectedLine)->getPoint2()[1] + dy < 0.0) {
        } else {
            (*selectedLine)->setPoint1X((*selectedLine)->getPoint1()[0] + dx);
            (*selectedLine)->setPoint1Y((*selectedLine)->getPoint1()[1] + dy);
            (*selectedLine)->setPoint2X((*selectedLine)->getPoint2()[0] + dx);
            (*selectedLine)->setPoint2Y((*selectedLine)->getPoint2()[1] + dy);
            glutPostRedisplay();
        }
    }
}

// This function deletes the selected line segment
void deleteSelectedLine(void) {
    if (selectedLine != NULL) {
        for(lineIter iter = vect.begin(); iter != vect.end(); ++iter) {
            if (iter == selectedLine) {
                delete (*selectedLine);
                vect.erase(selectedLine);
                selectedLine = NULL;
                glutPostRedisplay(); //re-display
                return;
            }
        }
    }
}

// This function selects a line segment and highlights it
void selectLine(void) {

    double lowestSquare = 0.0001;
    selectedLine = NULL;

    // calculate distance from mouse cursor to line segment
    for(lineIter iter = vect.begin(); iter != vect.end(); ++iter) {
        
        double distSq = ptSegDistSq((*iter)->getPoint1()[0], (*iter)->getPoint1()[1], 
                                    (*iter)->getPoint2()[0], (*iter)->getPoint2()[1],
                                    xPosition, yPosition);

        // if the distance is within a satisfactory amount (close enough to the cursor), highlight the line
        if (distSq < lowestSquare) {
            lowestSquare = distSq;
            selectedLine = iter;
            //cout << "Found Line Segment" << endl;
        }
    }

    glutPostRedisplay(); //re-display
}

/********************************************************************
 * Returns the square of the distance from a point to a line segment.
 * The distance measured is the distance between the specified
 * point and the closest point between the specified endpoints.  
 * If the specified point intersects the line segment in between the
 * endpoints, this method returns 0.0.     
 * param X1, Y1 the coordinates of the beginning of the 
 *			specified line segment
 * param X2, Y2 the coordinates of the end of the specified 
 *		line segment
 * param PX, PY the coordinates of the specified point being
 *		measured against the specified line segment
 * return a double value that is the square of the distance from the
 *			specified point to the specified line segment.
 * This is taken from the open source Java SDK file 2dline.java
 *******************************************************************/
double ptSegDistSq(double X1, double Y1, double X2, double Y2, double PX, double PY) {
	// Adjust vectors relative to X1,Y1
	// X2,Y2 becomes relative vector from X1,Y1 to end of segment
	X2 -= X1;
	Y2 -= Y1;
	// PX,PY becomes relative vector from X1,Y1 to test point
	PX -= X1;
	PY -= Y1;
	double dotprod = PX * X2 + PY * Y2;
	double projlenSq;

	if (dotprod <= 0.0) {
	    // PX,PY is on the side of X1,Y1 away from X2,Y2
	    // distance to segment is length of PX,PY vector
	    // "length of its (clipped) projection" is now 0.0
	    projlenSq = 0.0;
	} else {
	    // switch to backwards vectors relative to X2,Y2
	    // X2,Y2 are already the negative of X1,Y1=>X2,Y2
	    // to get PX,PY to be the negative of PX,PY=>X2,Y2
	    // the dot product of two negated vectors is the same
	    // as the dot product of the two normal vectors
	    PX = X2 - PX;
	    PY = Y2 - PY;
	    dotprod = PX * X2 + PY * Y2;
	    if (dotprod <= 0.0) {
		// PX,PY is on the side of X2,Y2 away from X1,Y1
		// distance to segment is length of (backwards) PX,PY vector
		// "length of its (clipped) projection" is now 0.0
		projlenSq = 0.0;
	    } else {
		// PX,PY is between X1,Y1 and X2,Y2
		// dotprod is the length of the PX,PY vector
		// projected on the X2,Y2=>X1,Y1 vector times the
		// length of the X2,Y2=>X1,Y1 vector
		projlenSq = dotprod * dotprod / (X2 * X2 + Y2 * Y2);
	    }
	}
	// Distance to line is now the length of the relative point
	// vector minus the length of its projection onto the line
	// (which is zero if the projection falls outside the range
	// of the line segment).
	double lenSq = PX * PX + PY * PY - projlenSq;
	if (lenSq < 0) {
	    lenSq = 0;
	}

	return lenSq;
}


// This function resets the current color and width to the defaults
void resetLineAttributes(void) {
    currentColor[0] = 0.0;
    currentColor[1] = 0.0;
    currentColor[2] = 0.0;
    width = 1;
}

// This function clears the screen and removes all lines from the vector
void clearScreen(void) {

    // destruct all line segments
    for (int i = 0; i < vect.size(); ++i) {
        delete vect[i];
    }
    vect.clear(); //clear the vector
    glutPostRedisplay(); //re-display
    selectedLine = NULL;
}

// This function handle the mouse menu
void MenuFn(int selection) {
    
    if (selection == 0) 
        // Exit
        exit(0);
}

// This function handle the mouse menu
void ColorFn(int selection) {

    if (selection == 0) {
        // Black
        currentColor[0] = 0.0;
        currentColor[1] = 0.0;
        currentColor[2] = 0.0;

    } else if (selection == 1) { 
        // Red
        currentColor[0] = 1.0;
        currentColor[1] = 0.0;
        currentColor[2] = 0.0;

    } else if (selection == 2) {
        // Green
        currentColor[0] = 0.0;
        currentColor[1] = 1.0;
        currentColor[2] = 0.0;

    } else if (selection == 3) {
        // Blue
        currentColor[0] = 0.0;
        currentColor[1] = 0.0;
        currentColor[2] = 1.0;

    } else if (selection == 4) {
        // Light blue
        currentColor[0] = 0.5;
        currentColor[1] = 0.5;
        currentColor[2] = 1.0;
    } else if (selection == 5) {
        // Purple
        currentColor[0] = 0.82;
        currentColor[1] = 0.17;
        currentColor[2] = 0.63;
    } else if (selection == 6) {
        // Olive drab
        currentColor[0] = 0.33;
        currentColor[1] = 0.52;
        currentColor[2] = 0.27;
    } else if (selection == 7) {
        // Grey
        currentColor[0] = 0.33;
        currentColor[1] = 0.32;
        currentColor[2] = 0.32;
    }
}

void main(int argc, char **argv) {

    glutInit(&argc,argv);

    help();
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow ("A Simple Drawing Program");

    init();
    glutDisplayFunc(draw);
    glutReshapeFunc(winReshapeFcn);
    glutMouseFunc(mf);
    glutKeyboardFunc(kf);
    glutSpecialFunc(special);
    glutPassiveMotionFunc(mousePosition);

    // Handle file input
    if (argc > 1) {
        fileName = argv[1];
        cout << "File specified: " << fileName << endl;
        openFile();
    }

    // Create main menu
    colorsMenuId = glutCreateMenu(ColorFn);
    glutAddMenuEntry("Black", 0);
    glutAddMenuEntry("Red", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Blue", 3);
    glutAddMenuEntry("Light Blue", 4);
    glutAddMenuEntry("Purple", 5);
    glutAddMenuEntry("Olive drab", 6);
    glutAddMenuEntry("Grey", 7);

    mainMenuId = glutCreateMenu(MenuFn);
    glutAddSubMenu("Colors", colorsMenuId);
    glutAddMenuEntry("Quit", 0);

    // Attach the menu to the right mouse button
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // Initiate input loop
    glutMainLoop();
}
