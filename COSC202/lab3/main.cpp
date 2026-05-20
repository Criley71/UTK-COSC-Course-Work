/*Connor Riley 9/19/2022
CS202
Blackjack lab

This is the main file of the black jack game between the user and computer dealer
*/
#include "CardDeck.h" 
#include <iostream>

using namespace std;
int main()
{
	//first we make a cardDeck object with 10 cards, and set the total of possible game outcomes to 0,
	//as well as declare the variables to check if the user hits or stands and if they want to continue
	//we then display the deck of 10 cards and then shuffle them and display them again showing that they 
	//were shuffled. we delete the deck and make a new one with 52 cards and shuffle it. Then the amount of wins
	//each player has or ties
	CardDeck *deck; 
	deck = new CardDeck(10);
	int playerWins = 0, dealerWins = 0, ties = 0;
	char hitOrStand, newGame;
	deck->display();
	deck->shuffle();
	deck->display();
	delete deck;
	cout << "\n";
	deck = new CardDeck();
	deck->shuffle();
	cout << "------------------------------------\n";
	cout << "player [" << playerWins << "] | dealer [" << dealerWins << "] | tie [" << ties << "]\n";
	cout << "------------------------------------\n";

	//Then our game begins inside the game loop. we set the hand scores to 0, deck sizes to 0,
	//and create the user and dealer decks. we set a condition to where if the deck falls below 15 cards a new 
	//deck of 52 is made for the players to use.
	do
	{
		int	 userSize = 0;
		int  dealerSize = 0;
		int  dealerScore = 0;
		int  userScore = 0;
		int* user = new int[userSize];
		int* dealer = new int[dealerSize];
		if (deck->getSize() < 15)
		{
			delete deck;
			deck = new CardDeck();
			for (int i = 0; i < (rand() % 10) + 1; i++)
			{
				deck->shuffle();
			}
		}
	//We then deal 2 cards to each player and print their hands and scores.
		for(int i = 0; i < 2; i++){
			deck->deal(dealer, dealerSize);
			deck->deal(user, userSize);
			userSize++;
			dealerSize++;
		}
		cout << "Your hand: ";
		for (int i = 0; i < userSize; i++)
		{
			cout << user[i] << " ";
			userScore += user[i];
		}
		cout <<"(" << userScore << ")" <<"\n";

		cout << "Dealer hand: ";
		for (int i = 0; i < dealerSize; i++)
		{
			cout << dealer[i] << " ";
			dealerScore += dealer[i];
		}
		cout<< "(" << dealerScore << ")" << "\n";
	//We then ask if the user wants to hit or stand, if they hit they are given another card using the deal function, if they 
	//go over 21 the game ends, if they stand the dealer then chooses if they will hit or stand.
		while (true)
		{
			if(dealerScore >= 21){
				break;
			}else if(userScore >= 21){
				break;
			}
			cout << "(h)it or (s)tand?: ";
			cin >> hitOrStand;
			if (hitOrStand == 'h')
			{
				deck->deal(user, userSize);
				userSize++;
				cout << "Your hand: ";
				userScore = 0;
				for (int i = 0; i < userSize; i++)
				{
					cout << user[i] << " ";
					userScore += user[i];

				}
				cout <<"(" << userScore << ")" <<"\n";
			}
			else
			{
				break;
			}
			if (userScore >= 21)
			{
				break;
			}
		}
	//Then the dealer begins to play until there score is at lease 17. each hit deals a new 
	//card using the deal function. Once there turn is complete a winner is then decided.
	//this only runs when the player hasnt hit black jack or busted as otherwise there is no need
	//for the game to continue
		while (userScore < 21 && dealerScore < 17)
		{
			dealerScore = 0;
			for (int i = 0; i < dealerSize; i++)
			{
				dealerScore += dealer[i];
			}
			if (dealerScore < 17)
			{
				dealerScore = 0;
				deck->deal(dealer, dealerSize);
				dealerSize++;
				cout << "Dealer hits.\n";
				cout << "Dealer hand: ";
				for (int i = 0; i < dealerSize; i++)
				{
					cout << dealer[i] << " ";
					dealerScore += dealer[i];
				}
				cout<< "(" << dealerScore << ")" << "\n";
			}
			
			if(dealerScore >= 17)
			{
				break;
			}
		}

		//The win conditions are then checked and the total wins are added. The order matters
		//as it has to check if a score is over 21 before comparing them to each other. it then prints
		//out the winner and the total wins and asks if the user would like to continue
		//if yes, each hands are emptied and the game continues with the remaining cards in the deck
		//if no the program ends
		if(userScore == dealerScore){
			cout << "Tie\n";
			ties++;
		}
		else if (userScore == 21)
		{
			cout << "You win\n";
			playerWins++;
		}else if(dealerScore == 21){
			cout << "Dealer wins\n";
			dealerWins++;
		}
		else if(userScore > 21){
			cout << "You bust.\n";
			cout << "Dealer Wins\n";
			dealerWins++;
		}
		else if(dealerScore > 21){
			cout << "Dealer Busts.\n";
			cout << "You Win\n";
			playerWins++;
		}
		else if(userScore > dealerScore){
			cout << "You win\n";
			playerWins++;
		}
		else if(userScore < dealerScore){
			cout << "Dealer Wins\n";
			dealerWins++;
		}
		cout << "------------------------------------\n";
		cout << "player [" << playerWins << "] | dealer [" << dealerWins << "] | tie [" << ties << "]\n";
		cout << "------------------------------------\n";

		cout << "Continue? y/n: ";
		cin >> newGame;
		if(newGame == 'y'){
			continue;
			delete[]user;
			delete[]dealer;
		}else if(newGame == 'n'){
			break;
		}
	} while (true);
}

