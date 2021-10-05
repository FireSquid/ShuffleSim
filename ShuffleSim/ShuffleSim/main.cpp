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

void SimulateShuffles(int shuffles, int steps);


int main(int argc, char *argv[]) {

	// Reseed the random number generator
	srand(time(NULL));

	/*	Test shuffling one deck back and forth
	Deck A = CreateDeck();
	Deck B = CreateDeck();

	ShuffleDeck(A, B);
	std::cout << "-------------------\n";
	ShuffleDeck(B, A);
	std::cout << "-------------------\n";
	ShuffleDeck(A, B);

	DeleteDeck(A);
	DeleteDeck(B);
	*/

	
	std::cout << "\n\n -- 2 Shuffles --\n";
	SimulateShuffles(2, 100000);
	std::cout << "\n\n -- 3 Shuffles --\n";
	SimulateShuffles(3, 100000);
	std::cout << "\n\n -- 4 Shuffles --\n";
	SimulateShuffles(4, 100000);
	std::cout << "\n\n -- 5 Shuffles --\n";
	SimulateShuffles(5, 100000);
	std::cout << "\n\n -- 6 Shuffles --\n";
	SimulateShuffles(6, 100000);
	std::cout << "\n\n -- 7 Shuffles --\n";
	SimulateShuffles(7, 100000);

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

	int cutPosition = 26 + (rand() % 3 - 1) + (rand() % 3 - 1) + (rand() % 3 - 1) + (rand() % 3 - 1);	// position to cut the cards in (between 22 and 30 with approximately normal distribution)

	// Position in source of the bottom cards of the two cuts of the deck
	int cutA = 0;
	int cutB = cutPosition;
	

	// loop to fill up the destination deck with shuffled cards
	for (int i = 0; i < 52; i++) {

		// Number of 
		int sizeA = cutPosition - cutA;

		// If one of the cuts is empty then take the next card from the non-empty cut
		if (cutA == cutPosition) {
			dest[i] = source[cutB++];
			//std::cout << "\t" << dest[i] << "\n";
		}
		else if (cutB == 52) {
			dest[i] = source[cutA++];
			//std::cout << dest[i] << "\n";
		}

		// Randomly take the next card from the cuts with a probability proportional to the size of each cut
		else if (rand() % (52 - i) < sizeA) {
			dest[i] = source[cutA++];
			//std::cout << dest[i] << "\n";
		}
		else {
			dest[i] = source[cutB++];
			//std::cout << "\t" << dest[i] << "\n";
		}
	}
}

// Print out the deck of cards as a space separated string of numbers
void PrintDeck(Deck d) {
	// loop through and print out each card
	for (int i = 0; i < 52; i++) {
		std::cout << d[i] << " ";
	}

	std::cout << "\n\n";
}

// Simulates shuffling a deck of cards "shuffles" times and collects the resulting positions of the cards.
//	Repeats the simulation "steps" times and reports the mean variance of the card at each position
void SimulateShuffles(int shuffles, int steps) {

	// Create 2 decks
	Deck DeckA = CreateDeck();
	Deck DeckB = CreateDeck();

	// If true then use A as the source deck when shuffling, otherwise use B
	bool AisSource = true;

	// Array counting the number of occurances of each card at each position
	int** data = new int*[52];
	for (int i = 0; i < 52; i++) {
		data[i] = new int[52];	// Array counting the number of occurances of each card at position i

		// Fill in the data with zeroes
		for (int j = 0; j < 52; j++)
			data[i][j] = 0;
	}


	// Run the simulation
	for (int i = 0; i < steps; i++) {

		// Prepare the next step
		ResetDeck(DeckA);
		ResetDeck(DeckB);

		// Perform the shuffles for the step
		for (int j = 0; j < shuffles; j++) {
			if (AisSource)
				ShuffleDeck(DeckA, DeckB);
			else
				ShuffleDeck(DeckB, DeckA);

			// Switch decks
			AisSource = !AisSource;
		}

		// Note the position where each card landed
		for (int j = 0; j < 52; j++)
			data[j][(AisSource) ? (DeckA[j]) : (DeckB[j])] += 1;

	}

	/*for (int i = 0; i < 52; i++) {
		std::cout << "Pos " << i << ":";
		for (int j = 0; j < 52; j++) {
			std::cout << " " << data[i][j];
		}
	}*/

	// Expected chance of each card appearing in each position
	float exApp = 1 / 52.0f;

	// Sum of the variances of each position
	float sumVar = 0.0f;

	// Calculate the variance of cards appearing in each position
	float* variance = new float[52];
	for (int i = 0; i < 52; i++) {
		float totalSqDif = 0;	// total square difference between the number of appearances of each card and the expected appearances

		// Get the square difference for each card
		for (int j = 0; j < 52; j++) {
			float appChance = data[i][j] / (float)steps;	// Chance of card j appearing in position i
			totalSqDif += (appChance - exApp) * (appChance - exApp);
		}

		// Calculate the position's variance
		variance[i] = totalSqDif / 52;

		sumVar += variance[i];
	}

	std::cout << "-- Average Variance: " << (sumVar / 52) << "\n\n";


	// Cleanup the data and variance arrays
	for (int i = 0; i < 52; i++)
		delete[] data[i];
	delete[] data;
	delete [] variance;

	// Deallocate the decks
	DeleteDeck(DeckA);
	DeleteDeck(DeckB);
}
