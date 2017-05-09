#include <cmath>
#include <cstdlib>
#include <GL/gl.h>
#include <GL/glut.h>
#include "Rules.h"

void PathFinder::set_dest( Direction new_dest ){
	this->init_dest = Dest;
	this->Dest = new_dest;
	rolling_status = 0;
	ismoving =true;
}

PathFinder::PathFinder(int x_position, int y_position, int maze_width, int maze_height)
{
	old_x = current_x = 20.0 + 10.0 * x_position;
	old_y = current_y = 20.0 + 10.0 * y_position;
	recursion_stack = new int[maze_width * maze_height * 4];	// size of worst case
	stack_top = -1;
	init_dest = Dest = RIGHT;
}


void PathFinder::Move()
{
	if( rolling_status == ROLL_FACT ){
		switch(Dest) {
		case UP:
			current_y += 5;//( walk_status >= 0 )? walk_status/3.0 : -walk_status/3.0;
			break;
		case DOWN:
			current_y -= 5;//( walk_status >= 0 )? walk_status/3.0 : -walk_status/3.0;
			break;
		case LEFT:
			current_x -= 5;//( walk_status >= 0 )? walk_status/3.0 : -walk_status/3.0;
			break;
		case RIGHT:
			current_x += 5;//( walk_status >= 0 )? walk_status/3.0 : -walk_status/3.0;
			break;
		}
	}

	if(abs(old_x - current_x) >= 10.0) {
		current_x = old_x + ((Dest == RIGHT) ? 10 : -10);
		old_x = current_x;
		ismoving = false;
	}
	else if(abs(old_y - current_y) >= 10.0) {
		current_y = old_y + ((Dest == UP) ? 10 : -10);
		old_y = current_y;
		ismoving = false;
	}
}

void PathFinder::Draw()
{
	glTranslatef( 30, 50, 0 );

	
	glTranslatef(20,15,0);
	// glRotatef(rotateAngle, 0, 0, 1);
	glTranslatef(-20, -15, 0);
	//glColor3f( bodyColorR, bodyColorG, bodyColorB );
	glColor3f( 0.0, 0.0, 1.0);
	glBegin( GL_POLYGON );
			// glEdgeFlag( GL_TRUE );
	glVertex2f( 0, 0 );
	glVertex2f( 40, 0  );
	glVertex2f( 40, 40 );
	glVertex2f( 0, 40 );
			//glVertex2f( 0, 0 );
	glEnd();
}

void PathFinder::UpdateStatus() {
	if (ismoving) Move();
	if (rolling_status < ROLL_FACT) {
		rolling_status++;
		if (init_dest == Dest) rolling_status = ROLL_FACT;
		if (rolling_status == ROLL_FACT) init_dest = Dest;
	}
	//if(get_goal == true) goal_ceremony_status++;
}
