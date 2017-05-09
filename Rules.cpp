#include <cmath>
#include <cstdlib>
#include <GL/gl.h>
#include <GL/glut.h>
#include "Rules.h"

// creates a new posistion and determines destination each time the square moves
void Rules::Destination(Direction new_dest){
	this->begin = Dest;
	this->Dest = new_dest;
	rolling_status = 0;
	moving = true;
}

// sets the boundary for the square from the maze size
Rules::Rules(int x_pos, int y_pos, int maze_width, int maze_height){
	old_x = current_x = 20.0 + 10.0 * x_pos;	
	old_y = current_y = 20.0 + 10.0 * y_pos;
	recursion_stack = new int[maze_width * maze_height * 4];	
	stack_top = -1;
	begin = Dest = RIGHT;	// will always have the square start on the right side of the screen
}

// determines if the square can move 
void Rules::Move(){
	// when the square's position equals the variable of 10,
	// does each case accordingly by adding or subtracting to 10
	// basically a counter for if a move is possible
	if(rolling_status == variable){
		switch(Dest){
			case UP:
				current_y += 5;
				break;
			case DOWN:
				current_y -= 5;
				break;
			case LEFT:
				current_x -= 5;
				break;
			case RIGHT:
				current_x += 5;
		}
	}

	// calculates if it's possible for the square to move 
	// and is also part of the collision detection from points on the window
	if(abs(old_x - current_x) >= 10.0){
		current_x = old_x + ((Dest == RIGHT) ? 10 : -10);
		old_x = current_x;
		moving = false;
	}
	else if(abs(old_y - current_y) >= 10.0){
		current_y = old_y + ((Dest == UP) ? 10 : -10);
		old_y = current_y;
		moving = false;
	}
}

// draws the square that's going to move 
void Rules::Draw()
{
	glTranslatef(30, 50, 0);
	glTranslatef(20,15,0);
	glTranslatef(-20, -15, 0);
	
	glColor3f(0.0, 0.0, 1.0);
	
	glBegin(GL_POLYGON);

	glVertex2f(0, 0);
	glVertex2f(40, 0);
	glVertex2f(40, 40);
	glVertex2f(0, 40);

	glEnd();
}

// determines if the square in the current position is able to move 
// part of the collision detection
void Rules::Update() {
	if (moving){
		Move();
	} 
	if (rolling_status < variable) {
		
		rolling_status++;
		
		if (begin == Dest){
			rolling_status = variable;
		} 
		if (rolling_status == variable){
			begin = Dest;
		} 
	}
}
