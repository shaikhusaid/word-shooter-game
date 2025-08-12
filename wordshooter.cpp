//============================================================================
// Name : cookie-crush.cpp
// Author : Sibt ul Hussain
// Version :
// Copyright : (c) Reserved
// Description : Basic 2D game of Cookie Crush...
//============================================================================
#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include<fstream>
#include <cstdlib> // For rand() and srand()
#include <ctime> // For seeding random number generator


#include "util.h"
using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 60

string * dictionary;
int dictionarysize = 370099;
#define KEY_ESC 27 // A

// 20,30,30
const int bradius = 30; // ball radius in pixels...

int width = 930, height = 660;
int byoffset = bradius;

int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 5; // initially number of full fixed_rows //
float score = 0;
int **board; // 2D-arrays for holding the data...
int bwidth = 130;
int bheight = 10;
int bsx, bsy;
const int nalphabets = 26;
// new varaibles
float ballxaxis=470;
float ballyaxis=10;
static float dxball = 0;    
static float dyball = 0;   
const int collisionRadius = 30;    
static const float velocity = 20.0; 
int sht_Pos_X=0;

bool burst=false;



static int alphabtetAtShooter = rand() % 26; 

bool ifballMoving = false; // Flag to check if the ball is in motion
int array2d[10][15];
char arraych[3][15];
enum alphabets {
 AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};
GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
"x.bmp", "y.bmp", "z.bmp" };
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...


//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
 // allocate a texture name
 glGenTextures(nalphabets, tid);
 vector<unsigned char> data;
 ofstream ofile("image-data.bin", ios::binary | ios::out);
 // now load each cookies data...

 for (int i = 0; i < nalphabets; ++i) {

  // Read current cookie

  ReadImage(tnames[i], data);
  if (i == 0) {
   int length = data.size();
   ofile.write((char*)&length, sizeof(int));
  }
  ofile.write((char*)&data[0], sizeof(char) * data.size());

  mtid[i] = tid[i];
  // select our current texture
  glBindTexture(GL_TEXTURE_2D, tid[i]);

  // select modulate to mix texture with color for shading
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  // when texture area is small, bilinear filter the closest MIP map
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
   GL_LINEAR_MIPMAP_NEAREST);
  // when texture area is large, bilinear filter the first MIP map
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // if wrap is true, the texture wraps over at the edges (repeat)
  // ... false, the texture ends at the edges (clamp)
  bool wrap = true;
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
   wrap ? GL_REPEAT : GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
   wrap ? GL_REPEAT : GL_CLAMP);

  // build our texture MIP maps
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
   GL_UNSIGNED_BYTE, &data[0]);
 }
 ofile.close();

}
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
 // allocate a texture name
 glGenTextures(nalphabets, tid);

 vector<unsigned char> data;
 ifstream ifile("image-data.bin", ios::binary | ios::in);

 if (!ifile) {
  cout << " Couldn't Read the Image Data file ";
  //exit(-1);
 }
 // now load each cookies data...
 int length;
 ifile.read((char*)&length, sizeof(int));
 data.resize(length, 0);
 for (int i = 0; i < nalphabets; ++i) {
  // Read current cookie
  //ReadImage(tnames[i], data);
  /*if (i == 0) {
  int length = data.size();

  ofile.write((char*) &length, sizeof(int));
  }*/
  ifile.read((char*)&data[0], sizeof(char)* length);

  mtid[i] = tid[i];
  // select our current texture
  glBindTexture(GL_TEXTURE_2D, tid[i]);

  // select modulate to mix texture with color for shading
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  // when texture area is small, bilinear filter the closest MIP map
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
   GL_LINEAR_MIPMAP_NEAREST);
  // when texture area is large, bilinear filter the first MIP map
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // if wrap is true, the texture wraps over at the edges (repeat)
  // ... false, the texture ends at the edges (clamp)
  bool wrap = true;
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
   wrap ? GL_REPEAT : GL_CLAMP);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
   wrap ? GL_REPEAT : GL_CLAMP);

  // build our texture MIP maps
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
   GL_UNSIGNED_BYTE, &data[0]);
 }
 ifile.close();
}
void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
 int cheight = 60)
 /*Draws a specfic cookie at given position coordinate

 * sx = position of x-axis from left-bottom
 * sy = position of y-axis from left-bottom
 * cwidth= width of displayed cookie in pixels

 * cheight= height of displayed cookiei pixels.
 * */
{
 float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
  / height * 2;
 float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

 glPushMatrix();
 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, mtid[cname]);
 glBegin(GL_QUADS);
 glTexCoord2d(0.0, 0.0);
 glVertex2d(fx, fy);
 glTexCoord2d(1.0, 0.0);
 glVertex2d(fx + fwidth, fy);
 glTexCoord2d(1.0, 1.0);
 glVertex2d(fx + fwidth, fy + fheight);
 glTexCoord2d(0.0, 1.0);
 glVertex2d(fx, fy + fheight);
 glEnd();

 glColor4f(1, 1, 1, 1);

 // glBindTexture(GL_TEXTURE_2D, 0);

 glDisable(GL_TEXTURE_2D);
 glPopMatrix();

 //glutSwapBuffers();
}
int GetAlphabet() {
 return GetRandInRange(1, 26);
}

void Pixels2Cell(int px, int py, int & cx, int &cy) {
}
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
{
}
void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
 float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
  / height * 2;
 float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

 glPushMatrix();
     glColor3f(1.0f, 0.0f, 0.0f); // Example: Red color (R, G, B values between 0 and 1)

 glEnable(GL_TEXTURE_2D);
 glBindTexture(GL_TEXTURE_2D, -1);
 glBegin(GL_QUADS);
 glTexCoord2d(0.0, 0.0);
 glVertex2d(fx, fy);
 glTexCoord2d(1.0, 0.0);
 glVertex2d(fx + fwidth, fy);
 glTexCoord2d(1.0, 1.0);
 glVertex2d(fx + fwidth, fy + fheight);
 glTexCoord2d(0.0, 1.0);
 glVertex2d(fx, fy + fheight);
 glEnd();

 glColor4f(1, 1, 1, 1);

 // glBindTexture(GL_TEXTURE_2D, 0);

 glDisable(GL_TEXTURE_2D);
 glPopMatrix();

 //glutSwapBuffers();
}
/*
* Main Canvas drawing function.

* */

bool CheckCollision(float sht_Pos_X, float sht_Pos_Y, float targetX, float targetY, float r) {
    float dx = sht_Pos_X - targetX;
    float dy = sht_Pos_Y - targetY;
    float Area = dx * dx + dy * dy;
    float All_side_collision = r* 2; 
    return Area <= (All_side_collision * All_side_collision);
}
// array for characters

int storing_randomCharacters[30] = {0};



void initialBalls() {
    int startX = 10;
    int startY = height / 2 + 230;
   

    static bool initialized = false;




    // Initialize grid once
    if (!initialized) {
        srand(time(0)); // Seed random number generator
        for (int i = 0; i < 30; ++i) {
            storing_randomCharacters[i] = rand() % 26; 
           }
        initialized = true;
    }
    // storing in 2d array:
    int z=0;
            for (int i =0; i<2; ++i) {
  		  for(int j=0; j<15; j++){
  		  array2d[i][j]=storing_randomCharacters[z];
            z++;
       }
    }
                int count = 0;

    // Draw targets
    for (int printed_row = 0; printed_row < 2; ++printed_row) {
        for (int printed_column = 0; printed_column < 15; ++printed_column) {
          // print new random alphabets
            int x = startX + printed_column * 60;
            int y = startY - printed_row * 60;
            DrawAlphabet((alphabets)storing_randomCharacters[count], x, y, awidth, aheight);
           count++; 
        }
    }
}
bool collisionCheck = false;
void DisplayFunction() {
    static bool initialized = false;
    static int TimeRemaining = 120;
    static int arrayX[100], arrayY[100];
    static int shootingCharacters[100];
    static time_t timerCheck = time(0);
    static int alphabtetAtShooter = rand() % 26;
    static int score = 0;
    static int n= 0; 


    time_t currentTime = time(0);
    if (difftime(currentTime, timerCheck) >= 1) {
        if (TimeRemaining > 0) TimeRemaining--;
        timerCheck = currentTime;
    }

    // Initialize once
    if (!initialized) {
        srand(time(0));
        initialized = true;
    }

    // Set the background color
    glClearColor(1.0 /*Red Component*/, 0.0 /*Green Component*/, 
                 1.0 /*Blue Component*/, 0.0 /*Alpha component*/); 
    glClear(GL_COLOR_BUFFER_BIT); // Update the colors
    
    // Draw existing balls
    initialBalls();


    if (ifballMoving) {
        ballxaxis += dxball;
        ballyaxis += dyball;

        // multlplied minus to reverse the direction
        if (ballxaxis< 0 || ballxaxis+awidth>width) 
        dxball = -dxball;
        if (ballyaxis<0 || ballyaxis+aheight>height) 
        dyball = -dyball;

        // check if the ball is collided
        for (int printed_row = 0; printed_row < 2; ++printed_row) {
            for (int printed_column = 0; printed_column < 15; ++printed_column) {


                int stopx = 10 + printed_column * 60;
                int stopy = height / 2 + 230 - printed_row * 60;

                if (CheckCollision(ballxaxis, ballyaxis, stopx, stopy, collisionRadius)) {
                    // Handle general collision
                    arrayX[n] = stopx;
                    arrayY[n] = stopy - 60;
                   
                    shootingCharacters[n] = alphabtetAtShooter;
                    collisionCheck = 1;
                    
                    ifballMoving = 0;
                    ballxaxis = 470;
                    ballyaxis = 10;
                    alphabtetAtShooter = rand() % 26;
                    n++;
                    break;
                }
            }
        }
    }
   


    // if striking occured:
    if (collisionCheck) {
        for (int j = 0; j < n; j++) {
        	//cout<<"Value of i and j = "<<arrayX[j]<<"  "<<arrayY[j]<<endl;
            DrawAlphabet((alphabets)shootingCharacters[j], arrayX[j], arrayY[j], awidth, aheight);
   arraych[2][(arrayX[j]) / 60]=char(shootingCharacters[j]+97);
        }
    }
   




    // Draw shooter ball
    DrawAlphabet((alphabets)alphabtetAtShooter, ballxaxis, ballyaxis, awidth, aheight);

    // Draw UI elements
    DrawShooter((width / 2) - 35 + sht_Pos_X, 0, bwidth, bheight);
    DrawString(40, height - 20, width, height + 5, "Score: " + Num2Str(score), colors[BLUE_VIOLET]);
    DrawString(width/2+200, height - 20, width, height + 5, "USAID \n24i-0057", colors[BLUE_VIOLET]);
    DrawString(width / 2 - 100, height - 25, width, height, "Time Left: " + Num2Str(TimeRemaining) + " secs", colors[RED]);

    // Game-over condition
    if (TimeRemaining <= 0) {
        DrawString(width/2-100, height/2, width, height, "--------->Game Over<--------", colors[RED]);
        cout << "Game Over! Your final score: " << score<<endl;
        collisionCheck=false;
       
    }
   
   
    
 
     for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 15; ++j) { 
        if (array2d[i][j] == -1 ) {
            arraych[i][j] = char(array2d[i][j]+33);
        } 
        else {
            arraych[i][j] = char(array2d[i][j] + 97);
        }


    }
}
arraych[2][16]='\0';
ofstream outFile("burststoring.txt");

   for (int i = 0; i < 3; i++) { 
        for (int j = 0; j < 15; j++) {  
                string pattern1 = string(1, arraych[i][j]) + arraych[i+1][j+1] + arraych[i+2][j+2]; 
                string pattern2 = string(1, arraych[i+2][j]) + arraych[i+1][j+1] + arraych[i][j]; 

           string pattern3 = string(1, arraych[i+1][j]) + arraych[i+2][j+1]; 
           string pattern4 = string(1, arraych[i+2][j]) + arraych[i+1][j+1]; 
                
                string pattern5 = string(1, arraych[2][j]) + arraych[2][j+1] + arraych[2][j+2]; 
                string pattern6 = string(1, arraych[i][j]) + arraych[i+1][j] + arraych[i+2][j]; 
                
                string pattern7 = string(1, arraych[i+1][j]) + arraych[i+2][j];
                string pattern8 = string(1, arraych[i][j]) + arraych[i][j+1]; 
                

                // Checking in dictonary dictionary
                for (int k = 0; k < dictionarysize; k++) {
                    if (pattern1 == dictionary[k] || pattern2 == dictionary[k] || pattern3 == dictionary[k] || pattern4 == dictionary[k] || pattern5 == dictionary[k] || pattern6 == dictionary[k] || pattern7 == dictionary[k] || pattern8 == dictionary[k]){
                    burst=true;
                    score++;

                         cout << "Matching wordd found: " << dictionary[k] << endl;
                          outFile<<dictionary[k] << endl;
                    }
                }
            }
        }
        outFile.close();
    
// Print the array
for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 15; ++j) {
        cout << arraych[i][j] << " ";
    }
    cout << endl;  // Optional: To separate fixed_rows
}
// Render next frame
    glutSwapBuffers();
    glutPostRedisplay();

}



/* Function sets canvas size (drawing area) in pixels...
* that is what dimensions (x and y) your game will have
* Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height) {
 /*glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
* is pressed from the keyboard
*
* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
*
* */


void NonPrintableKeys(int key, int x, int y) {
    int bspeed = 10; // Ball speed
    int shooterSpeed = 10; // Shooter movement speed

    if (key == GLUT_KEY_LEFT) {
        sht_Pos_X -= shooterSpeed; 
        // Move shooter left
        if (sht_Pos_X < -width / 2) // Prevent going out of bounds
            sht_Pos_X = -width / 2;
            
if(ballyaxis==10)
ballxaxis -= bspeed;
        if(ballxaxis <0)
        ballxaxis=0;// stopping to go outside 
    } 
    else if (key == GLUT_KEY_RIGHT) {
        sht_Pos_X += shooterSpeed; 
        // to move shooter right side
        if (sht_Pos_X > width / 2) // stoping from going out
            sht_Pos_X = width / 2;
            
            if(ballyaxis==10)
             ballxaxis += bspeed;
            if(ballxaxis>870)
        ballxaxis=870;
        // Adjust ball position accordingly
    } 
    else if (key == GLUT_KEY_UP) {
        // Placeholder for up arrow key functionality
    } 
    else if (key == GLUT_KEY_DOWN) {
        // Placeholder for down arrow key functionality
    }

    glutPostRedisplay(); // Redraw the scene after updating positions
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting
*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) {
 //If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls

}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
* You will have to add the necessary code here for shooting, etc.
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */

void MouseClicked(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && !ifballMoving) {
        // Calculate direction vector from ball to mouse position
        float targetX = (float)x; // X position of the mouse
        float targetY = height - (float)y; // Y position of the mouse (flip because OpenGL origin is bottom-left)

        // Calculate direction vector
        float dx = targetX - ballxaxis;
        float dy = targetY - ballyaxis;

        // Normalize the direction vector
        float dz = sqrt(dx * dx + dy * dy);
        if (dz != 0) {
            dxball = (dx / dz) * velocity; // Scale by speed
            dyball = (dy / dz) * velocity;
            ifballMoving = true; // Mark the ball as active (in motion)
        }


        cout<<"clicked at "<<targetX<<" "<<targetY<<endl;
        
    }

    glutPostRedisplay(); // Redraw the screen
}


/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyboard
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x, int y) {
 if (key == KEY_ESC/* Escape key ASCII*/) {
  exit(1); // exit the program when escape key is pressed.
 }
}

/*
* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).
* You can use this function to animate objects and control the
* speed of different moving objects by varying the constant FPS.
*
* */
void Timer(int m) {

 glutPostRedisplay();
 glutTimerFunc(1000.0/FPS, Timer, 0);
}

/*
* our gateway main function
* */
int main(int argc, char*argv[]) {

	InitRandomizer(); // seed the random number generator...
// Allocate memory
dictionary = new string[dictionarysize];

for (int i = 0; i < 10; ++i){
    for (int j = 0; j < 15; ++j){
    if(i==2)
    continue;
        array2d[i][j] = -1;
}}

ReadWords("words_alpha.txt", dictionary);
	//Dictionary for matching the words. It contains the 370099 words.
	 // dictionary is an array of strings
	//print first 5 words from the dictionary
	for(int i=0; i <dictionarysize ; ++i)
		cout<< " word "<< i << " =" << dictionary[i] <<endl;

	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets



	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("ITCs Word Shooter"); // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0/FPS, Timer, 0);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();
	// Clean up and quit SDL and SDL_mixer 
      
	// Clean up dynamically allocated memory
   

	return 1;
	// Clean up dynamically allocated memory
	  delete[] dictionary;
    dictionary= NULL;    
}
#endif /* */

