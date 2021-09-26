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
	std::cout << " --- Shuffle 2 ---\n";
	ShuffleDeck(dDeck, sDeck);
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
	

	// loop to fill up the destination deck with shuffled cards
	for (int i = 0; i < 52; i++) {
		if (cutA == cutPosition) {	// half A of the cards is empty so the next card is from half B
			dest[i] = source[cutB++];
			std::cout << "\t" << dest[i] << "\n";
		}
		else if (cutB == 52) {
			dest[i] = source[cutA++];	// half B of the cards is empty so the next card is from half A
			std::cout << dest[i] << "\n";
		}
		else if ((rand() % 2) == 0) {	// randomly selected for the next card to come from half A
			dest[i] = source[cutA++];
			std::cout << dest[i] << "\n";
		}
		else {	// randomly selected for the next card to come from half B
			dest[i] = source[cutB++];
			std::cout << "\t" << dest[i] << "\n";
		}
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
