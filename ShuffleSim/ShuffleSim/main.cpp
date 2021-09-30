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
		//std::cout << ((nextFromA) ? "" : "\t") << dest[i] << "\n";

		// Decide randomly whether or not to switch to taking cards from the other cut.
			//	Half the time the switch will happen
		nextFromA = (rand() % 3 > 0) ? (!nextFromA) : (nextFromA);
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
