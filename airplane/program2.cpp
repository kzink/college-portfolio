// Name: Kevin Zink
// Assignment #2
// Computer Graphics - CSCI 4620

#include<stdio.h>
#include<GL/glut.h>
#include<iostream.h>
#include<vector>
#include<math.h>

using namespace std;

static const int UPDATE_RATE = 10; //rate at which to refresh screen in milliseconds
static const float SCALE_AMOUNT = 1.0015; //amount to scale plane by
static const float ROTATION_AMOUNT = 0.03;   //amount to rotate plane by
static const float X_DISPLACEMENT_AMOUNT = 1; //X offset for display vector

GLsizei winWidth = 900, winHeight = 700;	// init display window size

int mainMenuId; // main menu identifier
int update = 1; //flag to determine if the timer should run

// This class represents a line segment.
// It contains 2 coordinate points to represent the line.
class Line {

    private:
        float point1 [3]; //point 1
        float point2 [3];

    public:
        // This is the default - it sets the points to 0
        // Line(void)
        // ---------------------------------------------
        // input:  -
        // return: -
        Line(void) {
            point1 [0] = 0;
            point1 [1] = 0;
            point1 [2] = 0;
            point2 [0] = 0;
            point2 [1] = 0;
            point2 [2] = 0;
        }

        // This is a constructor that takes 4 coordinates
        // Line(float xcoor1, float ycoor1, float xcoor2, float ycoor2)
        // ------------------------------------------------------------
        // input:  float, float, float, float
        // return: -
        Line(float xcoor1, float ycoor1, float xcoor2, float ycoor2) {
            point1 [0] = xcoor1;
            point1 [1] = ycoor1;
            point1 [2] = 0;
            point2 [0] = xcoor2;
            point2 [1] = ycoor2;
            point2 [2] = 0;
        }

        Line(float xcoor1, float ycoor1, float zcoor1, float xcoor2, float ycoor2, float zcoor2) {
            point1 [0] = xcoor1;
            point1 [1] = ycoor1;
            point1 [2] = zcoor1;
            point2 [0] = xcoor2;
            point2 [1] = ycoor2;
            point2 [2] = zcoor2;
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

        // This function creats a line segment in 3 dimensions
        void setPoints3D(float x1, float y1, float z1, float x2, float y2, float z2) {
            point1 [0] = x1;
            point1 [1] = y1;
            point1 [2] = z1;
            point2 [0] = x2;
            point2 [1] = y2;
            point2 [2] = z2;
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
};

// This class represents an Airplane, which is composed of a vector of Line segments
class Airplane {

    private:
        vector<Line *> vect; //this vector holds Line segments
        float x_offset;

    public:
        Airplane(void) {
            insertPlane();
        }

        // This function centers the plane about the point 0,0
        void center(void) {

            float max_y = 0;
            float max_x = 0;
            float min_x = 1000;
            float min_y = 1000;

            // if structure used to determine mins and maxes
            for (int i = 0; i < vect.size(); ++i) {
                if (vect[i]->getPoint1()[1] > max_y) {
                    max_y = vect[i]->getPoint1()[1];
                }
                if (vect[i]->getPoint2()[1] > max_y) {
                    max_y = vect[i]->getPoint2()[1];
                }
                if (vect[i]->getPoint1()[0] > max_x) {
                    max_x = vect[i]->getPoint1()[0];
                }
                if (vect[i]->getPoint2()[0] > max_x) {
                    max_x = vect[i]->getPoint2()[0];
                }
                if (vect[i]->getPoint1()[1] < min_y) {
                    min_y = vect[i]->getPoint1()[1];
                }
                if (vect[i]->getPoint2()[1] < min_y) {
                    min_y = vect[i]->getPoint2()[1];
                }
                if (vect[i]->getPoint1()[0] < min_x) {
                    min_x = vect[i]->getPoint1()[0];
                }
                if (vect[i]->getPoint2()[0] < min_x) {
                    min_x = vect[i]->getPoint2()[0];
                }
            }

            // finally, apply the offsets to the points in the vector
            for (i = 0; i < vect.size(); ++i) {
                vect[i]->setPoint1Y(vect[i]->getPoint1()[1] - (min_y + max_y) / 2);
                vect[i]->setPoint2Y(vect[i]->getPoint2()[1] - (min_y + max_y) / 2);
                vect[i]->setPoint1X(vect[i]->getPoint1()[0] - min_x);
                vect[i]->setPoint2X(vect[i]->getPoint2()[0] - min_x);
            }

            // This starts the plane just off the screen
            x_offset = -max_x;
        }

        // Destructor
        ~Airplane(void) {
            emptyVector();
        }

        // This function makes OpenGL calls to draw the plane
        void Draw(void) {
            
            // This loop applies movement transformations to the plane (moves plane UP to center it and RIGHT to apply MOVEMENT)
            for (int i = 0; i < vect.size(); ++i) {

                float x1 = vect[i]->getPoint1()[0];
                float x2 = vect[i]->getPoint2()[0];
                float y1 = vect[i]->getPoint1()[1];
                float y2 = vect[i]->getPoint2()[1];
        
                // Take each point and move it x_offset units to the right
                x1 += x_offset;
                x2 += x_offset;
                y1 = y1 + winHeight / 2;
                y2 = y2 + winHeight / 2;
                
                // apply changes to Vector
                vect[i]->setPoints2D(x1, y1, x2, y2);
            }


            // Draw the lines
            glBegin(GL_LINES);
                for (i = 0; i < vect.size(); ++i) {
                    glVertex2fv(vect[i]->getPoint1());
                    glVertex2fv(vect[i]->getPoint2());
                }
            glEnd();


            // This loop un-does what the first loop in this function did; it moves the points back to center around
            // the point (0, 0) 
            for (i = 0; i < vect.size(); ++i) {

                float x1 = vect[i]->getPoint1()[0];
                float x2 = vect[i]->getPoint2()[0];
                float y1 = vect[i]->getPoint1()[1];
                float y2 = vect[i]->getPoint2()[1];
        
                // Subtract x_offset from each point to move it back
                x1 -= x_offset;
                x2 -= x_offset;
                y1 = y1 - winHeight / 2;
                y2 = y2 - winHeight / 2;

                // apply changes to Vector
                vect[i]->setPoints2D(x1, y1, x2, y2);
            }
        }

        // This function empties the vector of all of its contents
        void emptyVector(void) {
            for (int i = 0; i < vect.size(); ++i)
                delete vect[i];
            vect.clear();
        }

        // This function Rotates the points in the vector by the inputted amount
        void Rotate(float theta) {

            // This function multiplies the original airplane points by a scalar amount.
            for (int i = 0; i < vect.size(); i++) {

                float x1 = vect[i]->getPoint1()[0];
                float x2 = vect[i]->getPoint2()[0];
                float y1 = vect[i]->getPoint1()[1];
                float y2 = vect[i]->getPoint2()[1];
                float z1 = vect[i]->getPoint1()[2];
                float z2 = vect[i]->getPoint2()[2];

                float y1_new = y1 * cos(theta) + z1 * sin(theta);
                float z1_new = -y1 * sin(theta) + z1 * cos(theta);

                float y2_new = y2 * cos(theta) + z2 * sin(theta);
                float z2_new = -y2 * sin(theta) + z2 * cos(theta);

                vect[i]->setPoints3D(x1, y1_new, z1_new, x2, y2_new, z2_new); // put points back into the vector
            }
        }

        // This function alters a variable that Translates the points in the vector
        void Translate(float move) {
            x_offset += move;
        }

        // This function scales the points in the vector by a inputted amount
        void Expand(float scale) {
            // ------------------------SCALE TRANSFORMATION----------------------------
            // This function multiplies the original airplane points by a scalar amount.
            for (int i = 0; i < vect.size(); i++) {

                float x1 = vect[i]->getPoint1()[0];
                float x2 = vect[i]->getPoint2()[0];
                float y1 = vect[i]->getPoint1()[1];
                float y2 = vect[i]->getPoint2()[1];
                float z1 = vect[i]->getPoint1()[2];
                float z2 = vect[i]->getPoint2()[2];

                // Take each point and multiple times the scalar
                x1 *= scale;
                y1 *= scale;
                x2 *= scale;
                y2 *= scale;
                z1 *= scale;
                z2 *= scale;

                // put points back into the vector
                vect[i]->setPoints3D(x1, y1, z1, x2, y2, z2); 
            }
        }

        // returns the Xoffset used to determine if the shape has clipped off the screen
        float getXOffset(void) {
            return x_offset;
        }

        // inserts a 3D tetrahedron into the vector
        void insertTetrahedron(void) {
            float t = (200 - sqrt(200*200 + 4*3*100*100))/(2*3);
            vect.push_back(new Line(100, 0, 0, 0, 100, 0));
            vect.push_back(new Line(0, 100, 0, 0, 0, 100));
            vect.push_back(new Line(0, 0, 100, 100, 0, 0));
            vect.push_back(new Line(100, 0, 0, t, t, t));
            vect.push_back(new Line(0, 100, 0, t, t, t));
            vect.push_back(new Line(0, 0, 100, t, t, t));
           
            center();
        }

        // inserts credit text into vector
        void insertCredits(void) {
            vect.push_back(new Line(57.8, 255.6, 75.8, 156.5));
            vect.push_back(new Line(56.8, 258.2, 100.4, 234.7));
            vect.push_back(new Line(100.4, 234.7, 76.7, 211.3));
            vect.push_back(new Line(73.9, 208.6, 123.2, 195.6));
            vect.push_back(new Line(125.1, 196.9, 75.8, 159.1));
            vect.push_back(new Line(77.7, 176.0, 100.4, 191.7));
            vect.push_back(new Line(99.5, 191.7, 71.0, 195.6));
            vect.push_back(new Line(71.0, 195.6, 79.6, 182.6));
            vect.push_back(new Line(68.2, 223.0, 85.3, 234.7));
            vect.push_back(new Line(85.3, 234.7, 64.4, 243.9));
            vect.push_back(new Line(64.4, 243.9, 65.4, 225.6));
            vect.push_back(new Line(142.1, 259.5, 170.6, 210.0));
            vect.push_back(new Line(142.1, 259.5, 153.5, 262.1));
            vect.push_back(new Line(153.5, 262.1, 176.3, 223.0));
            vect.push_back(new Line(167.7, 211.3, 194.3, 263.4));
            vect.push_back(new Line(192.4, 260.8, 200.0, 250.4));
            vect.push_back(new Line(199.6, 249.8, 150.0, 150.3));
            vect.push_back(new Line(150.3, 151.3, 138.7, 159.6));
            vect.push_back(new Line(139.0, 160.3, 166.8, 211.4));
            vect.push_back(new Line(223.7, 261.9, 221.2, 246.5));
            vect.push_back(new Line(223.7, 261.9, 232.5, 245.5));
            vect.push_back(new Line(232.5, 245.5, 221.2, 247.8));
            vect.push_back(new Line(217.8, 182.7, 213.7, 168.0));
            vect.push_back(new Line(217.5, 182.4, 231.5, 172.0));
            vect.push_back(new Line(231.5, 171.3, 213.4, 168.0));
            vect.push_back(new Line(90.0, 116.2, 92.8, 26.0));
            vect.push_back(new Line(90.9, 117.9, 105.0, 121.2));
            vect.push_back(new Line(94.0, 27.0, 105.9, 29.7));
            vect.push_back(new Line(105.9, 29.7, 105.0, 67.4));
            vect.push_back(new Line(105.0, 121.6, 105.0, 87.8));
            vect.push_back(new Line(105.0, 87.8, 138.7, 125.6));
            vect.push_back(new Line(138.7, 125.6, 155.0, 124.2));
            vect.push_back(new Line(155.0, 123.2, 115.0, 78.8));
            vect.push_back(new Line(105.6, 66.1, 168.7, 22.0));
            vect.push_back(new Line(114.3, 80.5, 172.5, 37.0));
            vect.push_back(new Line(171.5, 38.0, 167.8, 22.7));
            vect.push_back(new Line(170.3, 129.2, 181.8, 26.0));
            vect.push_back(new Line(181.8, 26.0, 232.5, 33.4));
            vect.push_back(new Line(233.4, 33.4, 227.5, 53.4));
            vect.push_back(new Line(227.5, 53.4, 190.3, 46.1));
            vect.push_back(new Line(190.3, 46.1, 187.1, 76.5));
            vect.push_back(new Line(187.1, 76.5, 203.4, 82.5));
            vect.push_back(new Line(203.4, 82.5, 200.3, 98.2));
            vect.push_back(new Line(200.3, 98.2, 185.6, 93.2));
            vect.push_back(new Line(185.6, 93.2, 182.8, 118.2));
            vect.push_back(new Line(182.8, 118.2, 220.6, 130.2));
            vect.push_back(new Line(220.6, 130.2, 212.8, 147.6));
            vect.push_back(new Line(212.8, 147.6, 168.7, 130.6));
            vect.push_back(new Line(222.8, 150.6, 246.5, 35.7));
            vect.push_back(new Line(222.8, 150.0, 233.7, 156.3));
            vect.push_back(new Line(234.3, 156.3, 255.3, 41.0));
            vect.push_back(new Line(245.6, 37.0, 255.3, 41.0));
            vect.push_back(new Line(255.0, 41.0, 260.6, 158.3));
            vect.push_back(new Line(260.6, 158.6, 272.8, 159.3));
            vect.push_back(new Line(272.8, 159.3, 264.6, 40.0));
            vect.push_back(new Line(264.6, 40.0, 255.6, 42.0));
            vect.push_back(new Line(287.8, 160.3, 294.3, 40.0));
            vect.push_back(new Line(297.1, 161.0, 305.0, 42.4));
            vect.push_back(new Line(284.6, 39.0, 317.1, 46.4));
            vect.push_back(new Line(317.1, 46.4, 318.7, 35.7));
            vect.push_back(new Line(318.7, 35.7, 284.0, 28.0));
            vect.push_back(new Line(284.0, 28.0, 284.0, 39.0));
            vect.push_back(new Line(280.0, 159.6, 303.7, 162.6));
            vect.push_back(new Line(303.7, 162.6, 300.9, 177.7));
            vect.push_back(new Line(300.9, 177.7, 272.8, 176.0));
            vect.push_back(new Line(272.8, 176.0, 281.2, 159.0));
            vect.push_back(new Line(321.5, 176.7, 332.1, 37.0));
            vect.push_back(new Line(331.8, 177.3, 342.1, 40.0));
            vect.push_back(new Line(332.5, 37.0, 343.1, 39.7));
            vect.push_back(new Line(321.2, 178.7, 331.8, 179.3));
            vect.push_back(new Line(331.8, 179.3, 370.3, 45.1));
            vect.push_back(new Line(370.9, 46.4, 380.9, 46.1));
            vect.push_back(new Line(380.6, 46.1, 340.3, 180.7));
            vect.push_back(new Line(340.3, 180.7, 330.6, 179.0));
            vect.push_back(new Line(380.6, 47.4, 368.4, 184.4));
            vect.push_back(new Line(367.5, 186.4, 378.7, 186.4));
            vect.push_back(new Line(378.7, 186.4, 387.8, 48.7));
            vect.push_back(new Line(387.8, 48.7, 379.6, 47.1));
            center();
        }

        // Inserts plane points into vector
        void insertPlane(void) {
            vect.push_back(new Line(33.4, 159.0, 37.5, 201.7));
            vect.push_back(new Line(37.8, 202.4, 48.7, 214.8));
            vect.push_back(new Line(48.7, 214.8, 66.8, 224.8));
            vect.push_back(new Line(66.8, 224.8, 80.0, 221.4));
            vect.push_back(new Line(80.0, 221.4, 89.0, 206.4));
            vect.push_back(new Line(88.1, 206.7, 89.6, 159.3));
            vect.push_back(new Line(89.6, 159.3, 187.8, 160.6));
            vect.push_back(new Line(187.5, 161.0, 211.2, 154.3));
            vect.push_back(new Line(211.8, 154.6, 237.5, 153.6));
            vect.push_back(new Line(237.5, 153.6, 257.1, 163.3));
            vect.push_back(new Line(257.5, 163.3, 249.0, 186.7));
            vect.push_back(new Line(248.7, 186.7, 245.3, 158.0));
            vect.push_back(new Line(247.5, 188.4, 254.0, 182.4));
            vect.push_back(new Line(257.1, 164.3, 252.8, 182.7));
            vect.push_back(new Line(88.7, 185.4, 195.3, 182.4));
            vect.push_back(new Line(219.3, 155.6, 220.9, 181.7));
            vect.push_back(new Line(234.0, 154.0, 231.5, 182.7));
            vect.push_back(new Line(220.3, 182.4, 231.5, 182.0));
            vect.push_back(new Line(219.3, 183.7, 214.3, 192.4));
            vect.push_back(new Line(214.3, 192.4, 215.6, 201.4));
            vect.push_back(new Line(215.9, 201.4, 224.3, 206.7));
            vect.push_back(new Line(225.3, 205.4, 234.3, 205.7));
            vect.push_back(new Line(234.3, 205.4, 238.1, 198.1));
            vect.push_back(new Line(237.5, 198.7, 237.8, 193.4));
            vect.push_back(new Line(237.8, 193.4, 231.2, 184.4));
            vect.push_back(new Line(220.3, 172.7, 202.1, 188.7));
            vect.push_back(new Line(219.3, 167.0, 198.7, 187.7));
            vect.push_back(new Line(198.7, 187.7, 198.7, 187.7));
            vect.push_back(new Line(198.4, 187.7, 206.2, 211.4));
            vect.push_back(new Line(202.5, 189.4, 207.5, 209.4));
            vect.push_back(new Line(214.6, 201.4, 220.6, 199.1));
            vect.push_back(new Line(220.6, 199.1, 224.0, 201.7));
            vect.push_back(new Line(224.0, 201.7, 226.8, 200.1));
            vect.push_back(new Line(226.8, 199.4, 225.6, 197.1));
            vect.push_back(new Line(225.0, 196.4, 222.1, 196.4));
            vect.push_back(new Line(221.8, 196.4, 220.9, 198.4));
            vect.push_back(new Line(229.0, 200.1, 230.3, 201.4));
            vect.push_back(new Line(230.6, 201.4, 233.4, 201.4));
            vect.push_back(new Line(233.4, 201.4, 234.3, 199.1));
            vect.push_back(new Line(234.3, 198.4, 231.5, 195.4));
            vect.push_back(new Line(231.2, 195.4, 228.7, 195.4));
            vect.push_back(new Line(228.7, 195.4, 227.8, 197.4));
            vect.push_back(new Line(228.1, 197.7, 228.1, 200.4));
            vect.push_back(new Line(233.4, 200.7, 236.5, 203.4));
            vect.push_back(new Line(233.4, 198.4, 236.5, 201.7));
            vect.push_back(new Line(220.6, 197.7, 214.3, 199.7));
            vect.push_back(new Line(226.5, 199.1, 227.1, 199.1));
            vect.push_back(new Line(232.5, 174.3, 241.8, 164.3));
            vect.push_back(new Line(232.5, 166.7, 240.0, 160.3));
            vect.push_back(new Line(241.2, 165.7, 245.6, 167.7));
            vect.push_back(new Line(239.0, 161.6, 245.0, 163.6));
            vect.push_back(new Line(213.7, 185.0, 214.3, 201.4));
            vect.push_back(new Line(214.3, 201.4, 225.3, 214.8));
            vect.push_back(new Line(225.3, 214.8, 236.2, 202.1));
            vect.push_back(new Line(236.2, 201.4, 239.3, 186.0));
            vect.push_back(new Line(219.3, 190.7, 231.8, 192.4));
            vect.push_back(new Line(231.8, 192.4, 227.8, 185.0));
            vect.push_back(new Line(227.8, 185.0, 219.0, 191.4));
            vect.push_back(new Line(223.4, 191.4, 225.3, 188.4));
            vect.push_back(new Line(225.3, 191.7, 225.9, 190.0));
            vect.push_back(new Line(228.1, 192.4, 227.8, 189.4));
            vect.push_back(new Line(229.6, 192.7, 228.4, 189.4));
            vect.push_back(new Line(225.6, 190.7, 224.6, 189.4));
            vect.push_back(new Line(203.7, 181.7, 195.9, 182.4));
            vect.push_back(new Line(210.0, 181.4, 219.6, 180.4));
            vect.push_back(new Line(231.2, 177.7, 245.9, 176.0));
            vect.push_back(new Line(255.0, 174.3, 324.6, 171.3));
            vect.push_back(new Line(238.1, 154.0, 323.4, 148.9));
            vect.push_back(new Line(180.6, 184.4, 204.6, 291.6));
            vect.push_back(new Line(204.6, 291.6, 245.0, 288.3));
            vect.push_back(new Line(245.0, 288.3, 271.8, 172.0));
            vect.push_back(new Line(176.8, 159.6, 197.8, 21.0));
            vect.push_back(new Line(197.8, 20.0, 245.9, 16.7));
            vect.push_back(new Line(245.9, 16.7, 271.2, 151.3));
            vect.push_back(new Line(325.0, 172.7, 324.0, 148.9));
            vect.push_back(new Line(324.3, 159.6, 330.3, 159.3));
            vect.push_back(new Line(313.1, 211.8, 341.2, 119.5));
            vect.push_back(new Line(312.1, 117.2, 349.6, 206.4));
            vect.push_back(new Line(89.3, 159.6, 32.5, 159.6));
            vect.push_back(new Line(84.3, 160.0, 70.0, 94.8));
            vect.push_back(new Line(70.0, 94.8, 47.8, 95.5));
            vect.push_back(new Line(48.7, 96.2, 36.5, 158.3));
            vect.push_back(new Line(44.0, 211.4, 59.3, 248.8));
            vect.push_back(new Line(83.4, 213.8, 83.1, 250.5));
            vect.push_back(new Line(83.1, 250.5, 57.8, 249.2));
            vect.push_back(new Line(322.8, 148.9, 310.0, 131.9));
            vect.push_back(new Line(310.3, 133.2, 265.9, 132.2));
            vect.push_back(new Line(180.9, 133.9, 78.7, 136.9));
            vect.push_back(new Line(30.9, 159.3, 38.4, 142.3));
            vect.push_back(new Line(198.1, 20.0, 174.3, 145.3));
            vect.push_back(new Line(245.6, 17.3, 271.2, 135.3));
            vect.push_back(new Line(180.3, 139.3, 266.5, 133.6));
            vect.push_back(new Line(183.7, 118.5, 262.8, 110.2));
            vect.push_back(new Line(186.5, 98.2, 258.7, 91.8));
            vect.push_back(new Line(188.1, 83.1, 255.6, 73.1));
            vect.push_back(new Line(191.2, 64.1, 253.1, 54.7));
            vect.push_back(new Line(218.4, 54.4, 207.1, 49.7));
            vect.push_back(new Line(207.1, 48.7, 206.8, 40.0));
            vect.push_back(new Line(206.8, 39.7, 213.7, 34.0));
            vect.push_back(new Line(213.7, 34.0, 225.0, 35.0));
            vect.push_back(new Line(225.9, 34.7, 231.8, 44.0));
            vect.push_back(new Line(231.2, 44.0, 227.5, 55.4));
            vect.push_back(new Line(227.5, 55.4, 216.5, 55.1));
            vect.push_back(new Line(219.3, 52.1, 212.5, 48.1));
            vect.push_back(new Line(211.8, 47.1, 211.8, 40.7));
            vect.push_back(new Line(211.8, 40.7, 224.0, 39.4));
            vect.push_back(new Line(224.0, 39.4, 227.1, 45.1));
            vect.push_back(new Line(226.8, 45.1, 223.7, 53.4));
            vect.push_back(new Line(223.1, 53.1, 218.1, 52.1));
            vect.push_back(new Line(218.7, 48.4, 214.3, 45.1));
            vect.push_back(new Line(214.3, 45.1, 222.5, 43.0));
            vect.push_back(new Line(222.5, 43.0, 223.1, 48.1));
            vect.push_back(new Line(223.1, 48.1, 218.4, 48.1));
            vect.push_back(new Line(186.5, 211.8, 263.4, 213.1));
            vect.push_back(new Line(185.0, 200.1, 198.7, 199.4));
            vect.push_back(new Line(205.3, 199.1, 211.8, 198.1));
            vect.push_back(new Line(237.8, 197.4, 265.3, 195.4));
            vect.push_back(new Line(190.3, 227.8, 258.7, 226.5));
            vect.push_back(new Line(193.7, 244.8, 255.3, 242.5));
            vect.push_back(new Line(196.8, 259.9, 250.6, 257.9));
            vect.push_back(new Line(201.2, 276.6, 247.5, 274.9));
            vect.push_back(new Line(214.6, 274.9, 220.6, 262.5));
            vect.push_back(new Line(220.6, 262.5, 227.8, 270.9));
            vect.push_back(new Line(226.8, 270.9, 209.6, 269.5));
            vect.push_back(new Line(209.6, 269.5, 227.1, 265.5));
            vect.push_back(new Line(227.1, 265.5, 214.6, 274.9));
            vect.push_back(new Line(203.4, 214.4, 210.9, 208.4));
            vect.push_back(new Line(209.6, 208.4, 212.1, 219.4));
            vect.push_back(new Line(212.1, 219.4, 203.1, 214.4));
            vect.push_back(new Line(93.4, 154.3, 160.3, 154.0));
            vect.push_back(new Line(101.2, 147.6, 148.1, 147.3));
            vect.push_back(new Line(111.8, 142.3, 136.8, 141.3));
            vect.push_back(new Line(197.1, 182.4, 186.5, 175.3));
            vect.push_back(new Line(186.5, 175.3, 185.6, 162.3));
            vect.push_back(new Line(63.7, 215.4, 54.3, 176.3));
            vect.push_back(new Line(54.3, 176.3, 80.9, 196.7));
            vect.push_back(new Line(78.7, 196.4, 45.6, 203.7));
            vect.push_back(new Line(45.9, 203.4, 74.0, 181.0));
            vect.push_back(new Line(73.4, 181.4, 62.8, 215.8));
            vect.push_back(new Line(100.3, 183.0, 100.3, 165.0));
            vect.push_back(new Line(93.1, 183.0, 107.1, 182.7));
            vect.push_back(new Line(108.7, 182.0, 109.0, 166.7));
            vect.push_back(new Line(108.7, 173.3, 118.4, 174.3));
            vect.push_back(new Line(115.6, 183.0, 116.8, 166.7));
            vect.push_back(new Line(124.3, 180.4, 123.1, 164.6));
            vect.push_back(new Line(123.1, 164.6, 132.8, 166.0));
            vect.push_back(new Line(123.1, 173.0, 128.7, 173.3));
            vect.push_back(new Line(124.0, 180.0, 132.8, 180.7));
            vect.push_back(new Line(148.1, 180.0, 140.0, 167.0));
            vect.push_back(new Line(146.5, 179.3, 151.5, 166.7));
            vect.push_back(new Line(144.3, 174.0, 151.2, 173.7));
            vect.push_back(new Line(160.0, 180.0, 152.8, 174.0));
            vect.push_back(new Line(152.8, 174.0, 159.6, 167.7));
            vect.push_back(new Line(163.1, 180.0, 163.1, 167.7));
            vect.push_back(new Line(163.1, 167.7, 170.0, 169.0));
            vect.push_back(new Line(164.3, 174.0, 169.6, 174.7));
            vect.push_back(new Line(162.1, 179.0, 169.3, 179.3));

            center(); //center plane
        }
};

// Function Prototypes
void UpdateFn(int);
void MenuFn(int);
void winReshapeFcn (GLint, GLint);
void init(void);

// Airplane object
Airplane * DisplayPlane = new Airplane();

// OpenGL Init function
void init(void) {
    glClearColor (1.0, 1.0, 1.0, 1.0);  // white display window
    glMatrixMode (GL_PROJECTION);
    gluOrtho2D (0.0, winWidth, 0.0, winHeight);
}

// This function updates the plane's points as it moves across the screen
void UpdateFn(int val) {

    if (update == 1) {

        // Do transformations
        DisplayPlane->Translate(X_DISPLACEMENT_AMOUNT);
        DisplayPlane->Expand(SCALE_AMOUNT);
        DisplayPlane->Rotate(ROTATION_AMOUNT);

        // Post (send a message requesting) update of the dislay
	    glutPostRedisplay();

         // Setup for another timer
        if (DisplayPlane->getXOffset() < winWidth) {
            glutTimerFunc(UPDATE_RATE, UpdateFn, 0);
        } else {
            update = 0;
        }
    }
}

// This function handle the mouse menu
void MenuFn(int selection) {

    // Exit
    if (selection == 0) 
        exit(0);

    // Restart Plane
    else if (selection == 1) {
        
        DisplayPlane->emptyVector();
        DisplayPlane->insertPlane();

        // Reset timer flag and start Update function again if needed
        if (update == 0) {
            update = 1;
            glutTimerFunc(UPDATE_RATE, UpdateFn, 0);
        } 
    } 
    
    // Stop
    else if (selection == 2) {
        update = 0;
    } 
    
    // Start
    else if (selection == 3) {
        update = 1;
        glutTimerFunc(UPDATE_RATE, UpdateFn, 0);
    } 

    // Credits
    else if (selection == 4) {
        DisplayPlane->emptyVector();
        DisplayPlane->insertCredits();
        if (update == 0) {
            update = 1;
            glutTimerFunc(UPDATE_RATE, UpdateFn, 0);
        }
    }

    // Tetrahedron
    else if (selection == 5) {
        DisplayPlane->emptyVector();
        DisplayPlane->insertTetrahedron();
        if (update == 0) {
            update = 1;
            glutTimerFunc(UPDATE_RATE, UpdateFn, 0);
        }
    }
}

// Reshape Function
void winReshapeFcn (GLint newWidth, GLint newHeight) {
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0, GLdouble(newWidth), 0, GLdouble(newHeight));
    winWidth = newWidth;
    winHeight = newHeight;
    glClear(GL_COLOR_BUFFER_BIT);
}

// This is the draw function.  
// It makes the openGL calls to draw the plane.
void drawPlane (void) {
    glClear (GL_COLOR_BUFFER_BIT);	// clear display window
    
    glColor3f (0.0, 0.0, 0.0);  // Black lines
    
    // Draw planes
    DisplayPlane->Draw();

    glFlush();
}

void main(int argc, char **argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(winWidth, winHeight);

    glutCreateWindow ("A Veritable Flying Circus!");

    init();
    glutDisplayFunc(drawPlane);
    glutReshapeFunc(winReshapeFcn);

    // Create main menu
    mainMenuId = glutCreateMenu(MenuFn);
    glutAddMenuEntry("Start", 3);
    glutAddMenuEntry("Stop", 2);
    glutAddMenuEntry("Restart (Airplane)", 1);
    glutAddMenuEntry("Quit", 0);
    glutAddMenuEntry("Credits", 4);
    glutAddMenuEntry("Tetrahedron 3D", 5);

    // Attach the menu to the right mouse button
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // Start the Update Function
    glutTimerFunc(UPDATE_RATE, UpdateFn, 0);

    glutMainLoop();
}