/* This is the console executable, that makes use of the BullCow class
This acts as the view in a MVC pattern, and is responsible for all
user interaction. For game logic see the FBullCowGame class.
NOTE: The following additions were made to the original code
1. The user can enter a number to select the size of the word they wish to guess.
2. The program pulls from a text file to create a dictionary of isograms.
2a. See IsogramMaker for making the text file of isograms.
3. Users can sacrifice a turn for a guess - a random letter in the word.
4. The number of max tries has been replaced by a formula - experiments needed.
5. When the game ends, win or lose, the hidden word is revealed.
Unfortunately, even though the words were pulled from the scrabble dictionary - some are rather obscure.
*/
#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"
#include <cstdlib>
#include <ctime>

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

// function prototypes as outside a class
void PrintIntro();
int32 GetWordLength();
void PrintInfo(int32);
void PlayGame(int32);
FText GetValidGuess();
bool AskToPlayAgain();
void PrintGameSummary();

FBullCowGame BCGame; // instantiate a new game, which we re-use across plays

int main()
{
	srand(time(0));
	//Struggling with rng, just throw away two values
	rand();
	rand();
	bool bPlayAgain = false;
	do {
		PrintIntro();
		int32 WordLength = 0;
		while (WordLength == 0) {
			WordLength = GetWordLength();
		}
		PrintInfo(WordLength);
		PlayGame(WordLength);
		bPlayAgain = AskToPlayAgain();
	} while (bPlayAgain);

	return 0; // exit the application
}

void PrintIntro()
{
	std::cout << "Welcome to Bulls and Cows, a fun word game.\n";
	std::cout << std::endl;
	std::cout << "          }   {         ___ " << std::endl;
	std::cout << "          (o o)        (o o) " << std::endl;
	std::cout << "   /-------\\ /          \\ /-------\\ " << std::endl;
	std::cout << "  / | BULL |O            O| COW  | \\ " << std::endl;
	std::cout << " *  |-,--- |              |------|  * " << std::endl;
	std::cout << "    ^      ^              ^      ^ " << std::endl;
	std::cout << std::endl;
	return;
}

int32 GetWordLength() 
{
	std::cout << "Please enter the length of the word you would like to use.\n";
	std::cout << "This must be between 5 and 15 inclusive, for example 7: ";
	int32 WordLength = 0;
	std::cin >> WordLength;
	if (std::cin.fail()) {
		WordLength = 0;
		std::cin.clear();
		std::cin.ignore(INT_MAX, '\n');
	}
	if (WordLength < 5 || WordLength > 15)
	{
		std::cout << "Not a valid word length. Word length must be between 5 and 15 inclusive.\n";
		std::cout << "Please enter a new length: ";
		WordLength = 0;
	}
	return WordLength;
}

void PrintInfo(int32 WordLength) 
{
	std::cout << "Great! Can you guess what " << WordLength;
	std::cout << " long isogram I am thinking of?\n";
	std::cout << "An isogram is a word without any repeated letters.\n";
	std::cout << "A bull is a correct letter in the isogram in the correct position.\n";
	std::cout << "A cow is a correct letter in the isogram but in the wrong position\n";
	std::cout << "If you are stuck, type ? to sacrifice a guess for a hint!	Good luck!\n\n";
}

// plays a single game to completion
void PlayGame(int32 WordLength)
{
	BCGame.Reset(WordLength);
	int32 MaxTries = BCGame.GetMaxTries();
	// loop asking for guesses while the game
	// is NOT won and there are still tries remaining
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries) {
		FText Guess = GetValidGuess();

		// submit valid guess to the game, and receive counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << "\n\n";
	}

	PrintGameSummary();
	return;
}

// loop continually until the user gives a valid guess
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		// get a guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries();
		std::cout << ". Enter your guess: ";
		std::getline(std::cin, Guess);

		// check status and give feedback
		Status = BCGame.CheckGuessValidity(Guess);
		int32 Letter;
		switch (Status) {
		case EGuessStatus::Hint:
			if ((BCGame.GetCurrentTry()) == BCGame.GetMaxTries())
			{
				std::cout << "Come on! Your last try has to be a guess\n";
				break;
			}
			//Generate a random int
			Letter = rand() % static_cast<int>(BCGame.GetHiddenWordLength());
			std::cout << "So you want a hint, well the letter in position " << Letter + 1 << " is: ";
			std::cout << BCGame.GetHiddenWord()[Letter] << std::endl;
			++BCGame.GetCurrentTry();
			break;
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word witout repeating letters.\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase letters.\n\n";
			break;
		default:
			// assume the guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK); // keep looping until we get no errors
	return Guess;
}

bool AskToPlayAgain()
{
	std::cout << "The hidden word was " << BCGame.GetHiddenWord() << std::endl;
	std::cout << "Do you want to play again with a new hidden word (y/n)? ";
	FText Response = "";
	std::getline(std::cin, Response);
	return (Response[0] == 'y') || (Response[0] == 'Y');
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "WELL DONE - YOU WIN!\n";
	}
	else
	{
		std::cout << "Better luck next time!\n";
	}
}