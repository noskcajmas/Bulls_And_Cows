/* The game logic (no view code or direct user interaction). 
The game is a simple guess the word game based on Mastermind.
*/

#include "FBullCowGame.h"
#include <map>

#define TMap std::map // to make syntax Unreal friendly

FBullCowGame::FBullCowGame() { Reset(); } // default constructor

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const 
{
	TMap<int32, int32> WordLengthToMaxTries{ {3,5}, {4,7}, {5,9}, {6,13}, {7,18} }; // TODO let the player choose the word size
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = "planet"; // TODO make a selection of words

	MyHiddenWord = HIDDEN_WORD;
	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

bool FBullCowGame::IsIsogram(FString Word) const 
{ 
	// treat 0 and 1 letter words as isograms
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen; // set up TMap

	// for each letter in the word 
	for (auto Letter : Word)
	{
		Letter = tolower(Letter); // handle mixed case

		if (LetterSeen[Letter]) // if the map already contains the letter
		{ 
			return false; 
		}
		else 
		{ 
			LetterSeen[Letter] = true; 
		}
	}
	return true; 
}

bool FBullCowGame::IsLowerCase(FString Word) const
{
	if (Word.length() <= 1) { return true; } // ignore zero length words

	for (auto Letter : Word)
	{
		if (!islower(Letter) && Letter != ' ')
		{
			return false;
		}
		else if ( Letter == '\n')
		{
			return true;
		}
	}
	return true;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsIsogram(Guess)) // if the guess isn't an isogram
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowerCase(Guess)) // if the  guess isn't all in lowercase
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (GetHiddenWordLength() != Guess.length()) // if the guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

// recieves a VALID guess, increments turn, and returns count.
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;

	// loop through all letters in the hidden word
	for (int32 HiddenWordChar = 0; HiddenWordChar < GetHiddenWordLength(); HiddenWordChar++)
	{
		// compare latters against the guess
		for (int32 GuessChar = 0; GuessChar < Guess.length(); GuessChar++) 
		{
			// if they match
			if (MyHiddenWord[HiddenWordChar] == Guess[GuessChar]) 
			{
				if (HiddenWordChar == GuessChar) 
				{ 
					BullCowCount.Bulls++; // increment bulls
				}
				else 
				{
					BullCowCount.Cows++; // increment cows
				}
			}
		}
	}

	// check to see if game has been won
	if (BullCowCount.Bulls == GetHiddenWordLength()) {
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}
	return BullCowCount;

}
