/*
 * Author  : Berru Lafci
 * Language: C++
 * Purpose : It's a robot role-playing game. In this game we have four different types of robots:
 *			 Optimusprime, Robocop, Roomba, Bulldozer. 
 *			 To represent these robots we should define a Robot class, Humanic class and seperate class for each of them.
 *			 Robot class inherits: Humanic, Roomba, Bulldozer.
 *			 Humanic class inherits: Optimusprime, Robocop.
 *           
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#include "Robot.h"
#include "Humanic.h"
#include "Optimusprime.h"
#include "Roomba.h"
#include "Bulldozer.h"
#include "Robocop.h"

using namespace std;

// Makes a battle between two robots.
void Fight(Robot &rob1, Robot &rob2);

int main(){

	srand(time(nullptr));

	Robot rob1(2, 100, 50);
	Robot rob2(1, 100,70);

	Fight(rob1,rob2);

	Optimusprime rob3(60,60);
	Roomba rob4(60,60);

	Fight(rob3,rob4); 

	return 0;
}

/*
	Call getDamage functions and calculate hitpoints.
	Returns the winner Robot by the state of hitpoint.
*/
void Fight(Robot &rob1, Robot &rob2){

	int hitpoint1;
	int hitpoint2;

	hitpoint1 = rob1.getHitpoint();
	hitpoint2 = rob2.getHitpoint();

	while((hitpoint1 > 0) && (hitpoint2 > 0)){

		int damage1, damage2;

		// Robot1 attack
		damage1 = rob1.getDamage();
		cout << "Robot 1 made " << damage1 << " damage point." << endl;
		hitpoint2 = hitpoint2 - damage1;  // decrease of hitpoint for rival
		cout << "Robot 2 hitpoint: " << hitpoint2 << endl;
		cout << endl;

		if(hitpoint2 <= 0){ // If hitpoint2 become 0 before hitpoint1, exit from the loop
			break;
		}

		// Robot2 attack
		damage2 = rob2.getDamage();
		cout << "Robot 2 made " << damage2 << " damage point." << endl;
		hitpoint1 = hitpoint1 - damage2;  // decrease of hitpoint for rival
		cout << "Robot 1 hitpoint: " << hitpoint1 << endl;
		cout << endl;

		rob1.setHitpoint(hitpoint1);
		rob2.setHitpoint(hitpoint2);
	}

	if((hitpoint1 > 0) && (hitpoint2 <= 0)){ // Robot 1

		cout << "Robot 1 won!" << endl;
		cout << endl;
	}

	else if((hitpoint1 <= 0) && (hitpoint2 > 0)){ // Robot 2

		cout << "Robot 2 won!" << endl;
		cout << endl;
	}

	else{

		cout << "It's a tie!" << endl;
		cout << endl;
	}
}
