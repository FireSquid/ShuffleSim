/**

	Project:	ShuffleSim
	File:		main.cpp
	Developer:	Peter Reynolds
	Date:		September 25, 2021

*/

#include <stdlib.h>
#include <time.h>
#include <iostream>


// Deck of cards as an array of ordered numbers
typedef int* Deck;


Deck CreateDeck();
void DeleteDeck(Deck d);
void ResetDeck(Deck d);
void ShuffleDeck(Deck source, Deck dest);
void PrintDeck(Deck d);


int main(int argc, char *argv[]) {

	// Create 2 new decks
	Deck sDeck = CreateDeck();
	Deck dDeck = CreateDeck();

	// Shuffle one deck into the other a few times
	std::cout << " --- Shuffle 1 ---\n";
	ShuffleDeck(sDeck, dDeck);

	PrintDeck(dDeck);

	std::cout << " --- Shuffle 2 ---\n";
	ShuffleDeck(dDeck, sDeck);

	PrintDeck(sDeck);

	std::cout << " --- Shuffle 3 ---\n";
	ShuffleDeck(sDeck, dDeck);

	// Print out the resulting shuffled deck
	PrintDeck(dDeck);

	// Delete the 2 decks
	DeleteDeck(sDeck);
	DeleteDeck(dDeck);


	return 0;
}


// Creates a new deck array and returns it as a pointer
Deck CreateDeck() {

	// Create a new array of integers as the deck
	Deck d = new int[52];

	// Set the deck to the numbers 0-51 in order
	ResetDeck(d);

	return d;
}

// Delete the deck pointer to cleanup allocated memory
void DeleteDeck(Deck d) {

	delete[] d;
}

// Reset the deck back to an ordered array of the numbers 0-51
void ResetDeck(Deck d) {

	// Fill the array with the numbers 0 through 51, inclusive
	for (int i = 0; i < 52; i++) {
		d[i] = i;
	}
}

// Shuffle the cards of the source deck, storing the results in the destination deck.
//	Each step is printed out.
void ShuffleDeck(Deck source, Deck dest) {

	// Reseed the random number generator
	srand(time(NULL));

	int cutPosition = 26;	// position to cut the cards in

	int cutA = 0;			// bottom card of the first half of the cut deck
	int cutB = cutPosition;	// bottom card of the second half of the cut deck
	
	bool nextFromA = ((rand() % 2) > 0);	// If true then the next card inserted into dest will be from cutA, Else it will be from cutB

	// loop to fill up the destination deck with shuffled cards
	for (int i = 0; i < 52; i++) {

		//std::cout << "Next: " << source[cutA] << " - " << source[cutB] << "\n";

		if (cutA == cutPosition) {	// half A of the cards is empty so the next card is from half B
			dest[i] = source[cutB++];
			
		}
		else if (cutB == 52) {
			dest[i] = source[cutA++];	// half B of the cards is empty so the next card is from half A
		}
		else {	// next card will come from the currently selected cut
			dest[i] = source[(nextFromA)?(cutA++):(cutB++)];
		}

		// Print out the next card inserted to visualize the algorithm
		std::cout << ((nextFromA) ? "" : "\t") << dest[i] << "\n";

		// Decide randomly whether or not to switch to taking cards from the other cut.
			//	Half the time the switch will happen
		nextFromA = (rand() % 2 > 0) ? (!nextFromA) : (nextFromA);
	}
	std::cout << "\n";
}

// Print out the deck of cards as a space separated string of numbers
void PrintDeck(Deck d) {
	// loop through and print out each card
	for (int i = 0; i < 52; i++) {
		std::cout << d[i] << " ";
	}

	std::cout << "\n\n";
}
