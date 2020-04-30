/*
* Josephus_Problem.cpp
* By Benjamin Lyne
* 2020-04-30: Successfully compiled on Linux using g++ -pedantic -Wall -Wextra -std=c++17 -o Josephus_Problem Josephus_Problem.cpp
* 2020-04-30: Verified using test cases (41, 3)(25, 16)(50, 8) and https://www.geogebra.org/m/ExvvrBbR
* Calculates the location at which the surviving soldier will be standing given n soldiers with every i soldier removed.
*
* This classic problem dates back to Roman times. There are 41 soldiers arranged in a circle. Every third soldier is to 
* be killed by their captors, continuing around the circle until only one soldier remains. He is to be freed. Assuming 
* you would like to stay alive, at what position in the circle would you stand?
* (https://edabit.com/challenge/J5uja7eL2LzktEzKo)
*/

#include <map>
#include <iostream>
#include <stdexcept>

#define ALIVE 1
#define DEAD 0
using std::map;

unsigned josephus (unsigned n, unsigned i);//prototype for the josephus function, which will do the "heavy lifting"

int main(){//dummy main to call josephus function with desired n, i
	unsigned n, i;
	std::cout << "Program Start" << std::endl;
	std::cin >> n;
	std::cin >> i;
	auto a = josephus (n, i);
	std::cout << a << std::endl;//Since the output is an integer (the position of the surviving soldier) no need to define an operator
	return 0;
}

unsigned josephus (unsigned n, unsigned i){
/* 
* Plan: We need some way to track which soldiers are "dead" and "alive" so that we can go in a circle. Using nested loops
* won't cut it. To that end, let's lay out the steps:
*	1. Initialize the soldiers using a map. Each soldier has a value (from 1 to n) and a boolean (alive/dead)
* 	2. While there is more than 1 soldier "alive," loop through the remaining living soldiers
*		a. On every i'th soldier ( if ( (clock%i) == 0) ) set the bool to DEAD
*		b. Increase dead soldier counter by 1
*	3. When the number of soldiers dead is one less than the total number ( if(counter == n-1) ), exit the loop
*		a. Find the ALIVE element and return it(using auto&[el,state] and return el).
* 
* Notes: 
*	"counter" and "clock" in the above plan starts at 1
*	"counter" is called "have_died" in the code below
*	errors and the trivial case are not considered in this code
*	Logic for (clock%i == 0): if i == 3, clock == 1, 2, 3, 4, 5, 6... then clock%i == 1, 2, 0, 1, 2, 0... 
*		ie there is a 0 at every element we want to "kill"
*/

//Step 1: Initialize
	unsigned have_died {1};
	unsigned clock {1};
	map<unsigned,bool> soldiers {};
	for (unsigned g = 1; g <= n; g++) soldiers[g] = ALIVE;

//Step 2: Excecute algoritm

	unsigned q {0};//q MUST be initialized outside the scope of the ENTIRE loop, but is an indexing variable
	//though there is no index 0, because of the do-while loop q MUST start at 0 for proper indexing
	while(have_died < n){//loop ends when 1 soldier is left alive
		//Find the next living element without advancing the "counter"
		do {//a do-while loop MUST be used so q++ is run at least once per iteration
			q++;
			if (q == n+1) q -= n;//automatically "overflows" q to prevent std::out_of_range
		}while( !soldiers.at(q) );//this will advance q until the index matches the next living element
		if (clock%i == 0){
			soldiers.at(q) = DEAD;
			have_died++;//Now that we have "killed" 1 soldier, reduce the amount of living soldiers by 1.
		}
		clock++;//clock increases every cycle of the loop, until infinity (integer overflow notwithstanding)
	}

//Step 3: Output
	for (auto[el,alive]: soldiers){//alive == true if still alive and false if dead
			if (alive) return el;
	}
	throw std::runtime_error("Error: All soldiers are dead.");//Code should never reach here, for debugging only.
}