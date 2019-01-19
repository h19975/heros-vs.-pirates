#include <time.h>
#include <stdlib.h>
#include <stdio.h>

struct HeroType
{
	char name[20];
	int strength;
	int armour;
	int health;
};

struct PirateType
{
	int strength;
	int health;
};

struct ArrayType
{
	struct PirateType **arrOfPirateType;
	int curNumOfPirate; // store the current number of pirates in the array
};

// this function takes in a HeroType and a PirateType
// return 1 if hero wins, return 0 if pirate wins
int fight(struct HeroType *heroType, struct PirateType *pirateType) {
	while (1) {
		//hero attacks pirate
		pirateType->health -= heroType->strength;
		//pirate dies
		if (pirateType->health <= 0) {
			heroType->health += 3;
			return 1;
		//pirate survives, attacks hero
		} else {
			//random generates temporary strength for pirates
			int tempStrength = rand() % (pirateType->strength+1);
			tempStrength += pirateType->strength;
			//printf("tempstrength: %d\n", tempStrength);
			heroType->health -= (tempStrength - heroType->armour);
			//printf("hero blood: %d\n", heroType->health);
			if (heroType->health <= 0) {
				return 0;
			}
		}
	}
}

int main(int argc, char ** argv)
{
	srand(time(NULL));
	
	//initialize heros
	struct HeroType timmy = {"Timmy", 5, 5, 30};
	struct HeroType harold = {"Harold", 7 , 3, 30};
	
	//initialize counters
	int timmyWon = 0;
	int haroldWon = 0;
	int bothHerosWon = 0;
	int pirateWon = 0;
	
	//100 runs of simulated battle
	for (int i = 0; i < 100; i++) {
		//printf("-----------------run-----------------\n");
		//initialize ArrayType
		struct PirateType **arrOfPirates = malloc(sizeof(struct PirateType*)*10);
		for (int i = 0; i < 10; i++) {
			//random generate strength for pirates
			int strength = rand() % 4;
			strength += 3;
			struct PirateType * pirate = malloc(sizeof(struct PirateType));
			pirate->strength = strength;
			pirate->health = 20;
			//push each pirate to the back of the ArrayType structure
			arrOfPirates[i] = pirate;
			//printf("pirate strength %d\n", strength);
		}
		struct ArrayType arrType = {arrOfPirates, 10};

		//determine which hero fights next
		int nextTim = 1;
		
		while ((timmy.health > 0 || harold.health > 0) && arrType.curNumOfPirate != 0) {
			int result;
			//timmy fights pirates
			if (nextTim == 1) {
				result = fight(&timmy, arrType.arrOfPirateType[arrType.curNumOfPirate-1]);
				//update who fights next
				if (harold.health > 0) {
						nextTim = 0;
				}
			} else {
				result = fight(&harold, arrType.arrOfPirateType[arrType.curNumOfPirate-1]);
				//update who fights next
				if (timmy.health > 0) {
					nextTim = 1;
				}
			}
			//if hero wins, remove pirate in the back
			//printf("result: %d\n", result);
			if (result == 1) {
				free(arrType.arrOfPirateType[arrType.curNumOfPirate-1]);
				arrType.curNumOfPirate--;
			}
		}
		
		//determine result and update counter
		if (arrType.curNumOfPirate == 0) {
			if (timmy.health > 0 && harold.health > 0) {
				bothHerosWon++;
			} else if (timmy.health > 0) {
				timmyWon++;
			} else {
				haroldWon++;
			}
		} else {
			//delete rest pirates
			for (int i = 0; i < arrType.curNumOfPirate; i++) {
				free(arrType.arrOfPirateType[i]);
			}
			pirateWon++;
		}
		
		//reset heroes' health indicators
		timmy.health = 30;
		harold.health = 30;
		free(arrType.arrOfPirateType);
	}
	
	//print out result
	printf("winning rate for Timmy alone is: %d/100\n", timmyWon);
	printf("winning rate for Harold alone is: %d/100\n", haroldWon);
	printf("winning rate for both heros is: %d/100\n", bothHerosWon);
	printf("winning rate for pirates is: %d/100\n", pirateWon);
	return 0;
}