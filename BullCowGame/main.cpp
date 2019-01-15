/*
This is the console executable, that makes use of the BullCow class.
This acts as the view in the MVC pattern, and is responsible for all user interaction. 
For game logic, see the FBullCowGame class.
*/

#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

// function prototypes as outside a class
void PrintIntro();
void PlayGame();
void PrintGameSummary();
FText GetValidGuess();
bool AskToPlayAgain();

FBullCowGame BCGame; // instantiate a new game, which we re-use accross plays

int main()
{
	bool bPlayAgain = false;
	do 
	{
		PrintIntro();
		PlayGame();
		PrintGameSummary();
		bPlayAgain = AskToPlayAgain();
	}
	while(bPlayAgain);
	
	return 0;
}

// introduce the game
void PrintIntro()
{

	std::cout << "\nWelcome to Bulls and Cows, a fun word game.\n\n";

	std::cout << "              }   {       ___              \n";
	std::cout << "             ( o o )     (o o)             \n";
	std::cout << "     /------- \\  /      \\ / -------\\    \n";
	std::cout << "    / | BULL |  O          O |  COW  |\\   \n";
	std::cout << "   *  |-,--- |               |------ | *   \n";
	std::cout << "      ^      ^               ^       ^     \n";

	std::cout << "\nCan you guess the isogram I'm thinking of?\n";
	std::cout << std::endl;
	return;
}

// plays a single game to completion
void PlayGame()
{
	BCGame.Reset();

	int32 MaxTries = BCGame.GetMaxTries();

	// loop for the number of turns while the game is NOT won
	// and there are still tries remaining
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries)
	{
		FText Guess = GetValidGuess();

		// submit valid guess to the game, and recieve counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << "\n\n";
	}
	return;
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "Well done - You Won!\n\n";
	}
	else
	{
		std::cout << "Ah Well, better luck next time.\n\n";
	}
}

// loop continually until the user gives a valid guess
FText GetValidGuess()
{
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	FText Guess = "";
	do
	{
		// get a guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries() << ". Enter your guess: ";
		std::getline(std::cin, Guess);

		// check guess status and give feedback
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter your guess in lower case.\n";
			break;
		default:
			// assume code is valid
			break;
		}
		std::cout << std::endl;
	} while (Status != EGuessStatus::OK); // keep looping until we get no errors
	return Guess;
}

bool AskToPlayAgain()
{
	std::cout << "Do you want to play again (y/n)? ";
	FText Response = "";
	std::getline(std::cin, Response);
	return (Response[0] == 'y') || (Response[0] == 'Y');
}
