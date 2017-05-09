#include <iostream>
#include <ctime>
#include <sys/time.h>
#include <cstring>
#include <GL/gl.h>
#include <GL/glut.h>
#include "MazeRunner.h"
#include "Rules.h"

using namespace std;

void display();
void reviewpoint();

static Space *space;
static int width, height;	// determines size of the maze
static int start_x, start_y;	// starting position of the square
static int end_x, end_y;	// final position the square has to reach
static double R, G, B;		// background color
static int *chosen;		// pointer of an array of open spaces
static int state = 0;		// initial state of the game
static Rules* gb_finder = NULL;	// creates a path finder object to make sure there's a path
static bool autoMode = false;
static int userInputLastDirection = -1;

static int view_Left, view_Right, view_Bottom, view_Up;

static inline Space & spaceXY(int x, int y){
	return space[y * width + x];
}

// determines what walls get removed when showing the animation
// of the entire maze 
void erase_wall(int x, int y, int dest){

	glColor3f(R, G, B);
	glBegin(GL_LINES);

	switch(dest){
		case up:
			glVertex2f((x+1)*10+0.02, (y+2)*10);
			glVertex2f((x+2)*10-0.02, (y+2)*10);
			break;

		case down:
			glVertex2f((x+1)*10+0.02, (y+1)*10);
			glVertex2f((x+2)*10-0.02, (y+1)*10);
			break;

		case right:
			glVertex2f((x+2)*10, (y+1)*10+0.02);
			glVertex2f((x+2)*10, (y+2)*10-0.02);
			break;

		case left:
			glVertex2f((x+1)*10, (y+1)*10+0.02);
			glVertex2f((x+1)*10, (y+2)*10-0.02);
			break;
	}
	glEnd();
}

// draws the maze and then determines if it can be erasable or not
void draw_maze(){
	int i;
	int x, y;

	for(i = 0 ; i < width*height ; i++){
		x = i % width;
		y = i / width;

		if(space[i].path[right] == true){
			erase_wall(x, y, right);
		}	
		if(space[i].path[up] == true){
			erase_wall(x, y, up);
		}		
		if(space[i].path[down] == true){
			erase_wall(x, y, down);
		}	
		if(space[i].path[left] == true){
			erase_wall(x, y, left);
		}		
	}
}

// generates the entire maze with animations of 
// walls being deleted with sort of a "run-time"
void gen_maze(){

	int x, y;	// current position in Space
	int dest;	// destination that can be connected
	static int length = 0;	// size of an array that can be the length
	int tmp;
	int currTime;
	static int oldTime = 0;

	// creates a new state if the original has been changed
	// allows for the actual playing of the game
	if(length == width * height){
		state = 1;
		for(int i = 0; i < width*height; i++){
			space[i].open = false;
		}
		return;
	}

	// if when the game state doesn't change
	if(length == 0){
		dest = rand()%2 + 1;	// creates the starting point as well as ends it
		if(dest == down){
			start_x = x = rand()%width;
			start_y = y = height - 1;
			spaceXY(x, y).path[up] = true;	// starting point

			end_x = x = rand()%width;
			end_y = y = 0;
			spaceXY(x, y).path[down] = true;	// end point
		}
		else{
			start_x = x = width - 1;
			start_y = y = rand()%height;
			spaceXY(x, y).path[right] = true;	// starting point
			
			end_x = x = 0;
			end_y = y = rand()%height;
			spaceXY(x, y).path[left] = true;	// ending point
		}
		chosen = new int [height * width];

		// choose the first Space randomly
		x = rand()%width;
		y = rand()%height;
		spaceXY(x, y).open = true;
		chosen[0] = width*y + x;	// store the first visited Space

		length = 1;
	}

	bool SpaceOpen = false;
	while (!SpaceOpen){
		tmp = chosen[rand()%length];	// randomly choose a Space which is visited
		x = tmp % width;
		y = tmp / width;

		dest = rand()%4;	// choose a direction to connect a nearby Space
		
		/*if the Space on the direction has visited or is out of range
		then select a Space and a directon again
		if not, connect two Spaces and store the new Space, the Space of the direction, 
		to the array chosen of visited spaces*/
		switch(dest){

		case up:
			if(y == height-1 || spaceXY(x, y + 1).open == true){
				continue;
			}

			spaceXY(x, y + 1).open = true;

			spaceXY(x, y + 1).path[ down ] = true;
			spaceXY(x, y).path[ up ] = true;

			chosen[length] = width*(y + 1) + x;
			length++;
			SpaceOpen = true;
			break;

		case down:
			if(y == 0 || spaceXY(x, y - 1).open == true){
				continue;
			}

			spaceXY(x, y - 1).open = true;

			spaceXY(x, y - 1).path[ up ] = true;
			spaceXY(x, y).path[ down ] = true;

			chosen[length] = width*(y - 1) + x;
			length++;
			SpaceOpen = true;
			break;

		case right:
			if(x == width-1 || spaceXY(x + 1, y).open == true){
				continue;
			}

			spaceXY(x + 1,  y).open = true;

			spaceXY(x + 1,  y).path[ left ] = true;
			spaceXY(x,  y).path[ right ] = true;

			chosen[length] = width * y + x + 1;
			length++;
			SpaceOpen = true;
			break;

		case left:
			if(x == 0 || spaceXY(x - 1,  y).open == true){
				continue;
			}

			spaceXY(x - 1,  y).open = true;

			spaceXY(x - 1,  y).path[ right] = true;
			spaceXY(x,  y).path[ left] = true;

			chosen[length] = width * y + x - 1;
			length++;
			SpaceOpen = true;
			break;
		}
	}
}

// was going to implement a way for users to input a maze
// size of their liking but didn't have the time
void reshape(int w, int h){
	int size = (width > height)? width : height;
	double move = (width > height)? (width-height)/2.0 : (height-width)/2.0;

	glViewport(0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	if( state == 0 ){
		if(width == size){
			view_Left = 0.0;
			view_Right = 20 + size*10;
			view_Bottom = 0.0 - move*10;
			view_Up = size*10 + 20 - move*10;
		}
		else{
			view_Left = 0.0 - move*10;
			view_Right = 20 + size*10 - move*10;
			view_Bottom = 0.0;
			view_Up = size*10 + 20;
		}
	}

	gluOrtho2D(view_Left, view_Right, view_Bottom, view_Up);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

void display(){
	double x;

	glClearColor(R, G, B, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1-R, 1-G, 1-B);

	// the initial size of the maze
	glLoadIdentity();
	glBegin(GL_LINES);
	for(x = 1 ; x < width+2 ; x++){
		glVertex2f(x*10, 10.0 );
		glVertex2f(x*10, height*10+10.0);
	}
	for(x = 1 ; x < height+2; x++){
		glVertex2f(10.0 , x*10);
		glVertex2f(width*10+10.0 , x*10);
	}
	glEnd();

	draw_maze();

	if(gb_finder != NULL){
		const double SHIFTFACTOR_X = -10.0;
		const double SHIFTFACTOR_Y = -11.5;

		glLoadIdentity();
		glTranslatef(gb_finder->CurrentX() + SHIFTFACTOR_X, gb_finder->CurrentY() + SHIFTFACTOR_Y, 0);
		glScalef(0.1, 0.1, 1);
		gb_finder->Draw();
	}

	glutSwapBuffers();
}

void path_finding(){
	static Rules finder(::start_x, ::start_y, ::width, ::height);
	static int x = ::start_x;
	static int y = ::start_y;

	if (gb_finder == NULL){
		gb_finder = &finder;	// referenced here to be allowed in other function
	}

	finder.Update();
	if(finder.Moving()){
		return;
	}

	// when it reaches the end, then it makes sure it's at the end
	// and changes the state of the game
	if(x == ::end_x && y == ::end_y){	
		::state++;
		gb_finder->End();
		return;
	}

	// contains the stack of information that determines if the 
	// square can move or not
	if(autoMode){
		spaceXY(x, y).open = true;	
		// this first determines if the square can move from its 
		// initial position and then adds info to the stack
		if(finder.emptyStack() || finder.Stack_Top() < noDirection){
			if(spaceXY(x,  y).path[down] == true && y > 0 && spaceXY(x, y-1).open == false){
				finder.Destination(Rules::DOWN);
				y--;
				spaceXY(x, y).open = true;
				finder.Stack_Push(down);
				return;
			}
			else finder.Stack_Push(notDown);
		}
		// part of the stack that determines if the square can go down
		if(finder.Stack_Top() == notDown){
			if(spaceXY(x, y).path[left] == true && x > 0 && spaceXY(x-1, y).open == false){
				finder.Destination(Rules::LEFT);
				x--;
				spaceXY(x, y).open = true;
				finder.Stack_Push(left);
				return;
			}
			else finder.Stack_Push(notLeft);
		}
		// part of the stack that determines if the square can go left
		if(finder.Stack_Top() == notLeft){
			if(spaceXY(x, y).path[right] == true && x < ::width-1 && spaceXY(x+1, y).open == false){
				finder.Destination(Rules::RIGHT);
				x++;
				spaceXY(x, y).open = true;
				finder.Stack_Push(right);
				return;
			}
			else finder.Stack_Push(notRight);
		}
		// part of the stack that determines if square can go right
		if(finder.Stack_Top() == notRight){
			if(spaceXY(x, y).path[up] == true && y < ::height-1 && spaceXY(x, y+1).open == false){
				finder.Destination(Rules::UP);
				y++;
				spaceXY(x, y).open = true;
				finder.Stack_Push(up);
				return;
			}
			else finder.Stack_Push(notUp);
		}
	
		int temp_dest;
		finder.Stack_Pop();
		finder.Stack_Pop();
		finder.Stack_Pop();
		finder.Stack_Pop();
		temp_dest = finder.Stack_Pop();
		
		if(temp_dest == down){
			y++, finder.Destination(Rules::UP);
		} 
		else if(temp_dest == left){
			x++, finder.Destination(Rules::RIGHT);
		} 
		else if(temp_dest == right){
			x--, finder.Destination(Rules::LEFT);
		} 
		else if(temp_dest == up){
			y--, finder.Destination(Rules::DOWN);
		} 
	}
	else{
		if (userInputLastDirection > -1){
			switch (userInputLastDirection){
			case up:
				if(spaceXY(x, y).path[up] == true && y < ::height-1 && spaceXY(x, y+1).open == false){
					finder.Destination(Rules::UP);
					y++;
				}
				break;
			case down:
				if(spaceXY(x,  y).path[down] == true && y > 0 && spaceXY(x, y-1).open == false){
					finder.Destination(Rules::DOWN);
					y--;
				}
				break;
			case right:
				if(spaceXY(x, y).path[right] == true && x < ::width-1 && spaceXY(x+1, y).open == false){
					finder.Destination(Rules::RIGHT);
					x++;
				}
				break;
			case left:
				if(spaceXY(x, y).path[left] == true && x > 0 && spaceXY(x-1, y).open == false){
					finder.Destination(Rules::LEFT);
					x--;
				}
				break;
			}
			userInputLastDirection = -1;
		}
	}
}

void Finish(){
	static int count = 0;
	static int oldTime = 0;

	count++;
	if(count > 100){
		state++;
		gb_finder->Update();
	}
}

void specialKeyFunc(int key, int x, int y){
	switch (key){
		case GLUT_KEY_RIGHT:
			userInputLastDirection = right;
			break;
		case GLUT_KEY_LEFT:
			userInputLastDirection = left;
			break;
		case GLUT_KEY_DOWN:
			userInputLastDirection = down;
			break;
		case GLUT_KEY_UP:
			userInputLastDirection = up;
			break;
	}

	reviewpoint();
	display();
}

// determines if the square has reached the end of the game
void reviewpoint(){
	if(gb_finder == NULL) return;	
		double move = (width-height)/2.0;
		if( width >= height ){
			view_Left = 0.0;
			view_Right = 20 + width*10;
			view_Bottom = 0.0 - move*10;
			view_Up = width*10 + 20 - move*10;
		}
		else{
			view_Left = 0.0 + move*10;
			view_Right = 20 + height*10 + move*10;
			view_Bottom = 0.0;
			view_Up = height*10 + 20;
		}
}

// the game idles a bit to show the animation of the
// maze being created
void idle(){
	if (state == 0){
		gen_maze();
	}
	else if (state == 1){
		path_finding();
		reviewpoint();
	}
	else if (state == 2){
		Finish();
	}
	else if (state == 3){
		exit(0);
	}
	display();
}

int main(int argc, char **argv){
	width = 10;
	height = 10;
	
	space = new Space[width * height];

	R = 6.0;
	G = 6.0;
	B = 6.0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow ("Maze Runner");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutSpecialFunc(specialKeyFunc);

	glutMainLoop();

	return 0;
}
