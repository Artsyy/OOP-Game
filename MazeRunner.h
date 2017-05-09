#ifndef __MAZERUNNER_H__
#define __MAZERUNNER_H__

// already initialized values for directions
#define	up		0
#define	down	1
#define right	2
#define left	3

#define noDirection 4	// the square cannot go anywhere else
#define notUp	4	// the square cannot go up
#define notDown	5	// the square cannot go down
#define notRight 6	// the square cannot go right
#define notLeft	7	// the square cannot go left

#define BeginingTime 20

typedef struct Space{
	bool open;	// variable for determining if the spaces next to the square is viable
	bool path[4];	// each path direction available

	Space(){
		open = false;
		path[0] = false;
		path[1] = false;
		path[2] = false;
		path[3] = false;
	}
} Space;

#endif
