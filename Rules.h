#ifndef __RULES_H__
#define __RULES_H__

#define variable 10

class Rules {
public:
	// four directions the square can move
	enum Direction {UP = 0, DOWN = 1, RIGHT = 2, LEFT = 3};
	
	// constructor that determines all dimensions 
	Rules(int x_pos, int y_pos, int maze_width, int maze_height);
	// destructor for stack information
	~Rules() {delete recursion_stack;}

	bool Moving() {return moving;}	// determines if the square is "moving"
	void Destination(Direction new_dest);
	void Move();
	
	void Draw();	// draws the square
	void Update();	// information about square

	double CurrentX() {return current_x;}	// returns the x position of the finder
	double CurrentY() {return current_y;}	// returns the  y position of the finder
	void End() {goal = true;}	// creates the end position and determines if the suare has reached it

	// stack of positions that determines if the path is viable
	bool emptyStack() {return stack_top == -1;}
	void Stack_Push(int dest) {recursion_stack[++stack_top] = dest;}
	int Stack_Pop() {return (stack_top == -1)? recursion_stack[stack_top+1]: recursion_stack[stack_top--];}
	int Stack_Top() {return recursion_stack[stack_top];}

private:
	// the square's position and direction (old and current) when choosing a path
	double current_x;
	double current_y;
	double old_x;
	double old_y;
	int Dest;
	int begin;
	bool goal;

	// variables for the square's "movement"
	bool moving;
	int rolling_status;

	// variables for the stack of finding positions
	int* recursion_stack;
	int stack_top;
};

#endif
