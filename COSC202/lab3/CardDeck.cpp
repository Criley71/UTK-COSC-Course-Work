/*
   Connor Riley 9/19/2022
   CS202
   Blackjack lab

   This is the card deck class for a game of black jack. This makes and assigns cards values
   from 2-11 in a default size of 52 cards. It deals cards, finds the size of the deck, and
   shuffles the deck
   */
#include "CardDeck.h"
#include <algorithm>
using namespace std;
//this creates the deck object, giving it a size and values for each card
CardDeck::CardDeck(int deckSize)
{
	size = deckSize;
	deck = new int[deckSize];

	for (int i = 0; i < deckSize; i++)
	{
		deck[i] = (i % 10) + 2;
	}
}
//deconstructor to destroy the card deck class
CardDeck::~CardDeck() = default;
//copy constructor to copy a deck into another array
const CardDeck &CardDeck::operator=(const CardDeck &obj)
{
	if (&obj != this)
	{
		if (size != obj.size)
		{
			delete[] deck;
			size = obj.size;
			deck = new int[size];
		}
		for (int i = 0; i < size; i++)
		{
			deck[i] = obj.deck[i];
		}
	}
	return (*this);
}


//returns the amount of cards left in the deck
int CardDeck::getSize()
{
	return size;
}
//this will shuffle the cards via the algorithm library
void CardDeck::shuffle()
{
	random_shuffle(&deck[0], &deck[size]);
}
//This will deal the card out to a players hand. it makes a new hand that is one larger
//and fills it with the cards from the old hand and grabs a new card from the end of the array
//this is then set to the players hand. It then removes the card from the deck via making a copy
//that is 1 card size less and fills it the cards from the deck and is then set to the game deck.
void CardDeck::deal(int *&hand, int size) {
	//make hand copy
	int *new_hand = new int [size + 1];
	for(int i = 0; i < size; i++){ //copy hand
		new_hand[i] = hand[i];
	}
	delete[] hand;
	//draw card from deck and add it to new_hand
	new_hand[size] = deck[this->size - 1];
	hand = new_hand;

	//remove card from deck	
	int* new_deck = new int[this-> size - 1];
	for(int i = 0; i < this->size-1; i++){
		new_deck[i] = deck[i];
	}
	delete[] deck;
	this->size--;
	deck = new_deck;
}
//this prints the deck of cards to check if the shuffle works.
void CardDeck::display(){
	for(int i = 0; i < size; i++){
		cout << deck[i] << " ";
	}
	cout << "\n";
}
