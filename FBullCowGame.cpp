#pragma once

#include "FBullCowGame.h"
#include <fstream>
#include <string>
#include <map>
#include <stdexcept>
#include <cstdlib>

// to make syntax Unreal friendly
#define TMap std::map
using int32 = int;

FBullCowGame::FBullCowGame() 
{ 
	InitDictionary();
} // default constructor

int32& FBullCowGame::GetCurrentTry() { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const
{
	//TMap<int32, int32> WordLengthToMaxTries{ { 3,4 },{ 4,7 },{ 5,10 },{ 6,16 },{ 7,20 } };
	//return WordLengthToMaxTries[MyHiddenWord.length()];
	return std::ceil(4 * (std::log(MyHiddenWord.length()) + (MyHiddenWord.length() - 4)));
}

FString FBullCowGame::GetHiddenWord() const {
	return MyHiddenWord;
}

void FBullCowGame::InitDictionary()
{
	FString InputFileName = "isograms.txt";
	std::ifstream InputFile;
	InputFile.open(InputFileName);
	if (InputFile.is_open()) {
		for (int32 i = 0; i < 44799; ++i) //44799 Dictionary words
		{
			std::getline(InputFile, Dictionary[i]);
		}
	}
	else throw std::runtime_error("Oh dear, could not read from input file\n");
	InputFile.close();
}

void FBullCowGame::Reset(int32 WordLength)
{
	const FString HIDDEN_WORD = GetWord(WordLength);
	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

FString FBullCowGame::GetWord(int32 WordLength) const
{
	TMap<int32, int32> WordLengthToNumIsograms{ { 5, 8013 },{ 6, 10362 },{ 7, 10799 },
	{ 8, 8181 },{ 9, 4649 },{ 10, 1980 },
	{ 11, 638 },{ 12, 146 },{ 13, 23 },
	{ 14, 6 },{ 15, 2 } };
	//Random generate int in 0 - WordLength - 1
	int32 Index = rand() % static_cast<int>(WordLengthToNumIsograms[WordLength]);
	int32 SmallerWordsInDict = 0;
	for (int32 i = 5; i < WordLength; ++i)
	{
		SmallerWordsInDict += WordLengthToNumIsograms[i];
	}
	return Dictionary[SmallerWordsInDict + Index];
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (Guess == "?")
	{
		return EGuessStatus::Hint;
	}
    else if (!IsIsogram(Guess)) // if the guess isn't an isogram
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess)) // if the guess isn't all lowercase
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (Guess.length() != GetHiddenWordLength()) // if the guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

// receives a VALID guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); // assuming same length as guess

											  // loop through all letters in the hidden word
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++) {
		// compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++) {
			// if they match then
			if (Guess[GChar] == MyHiddenWord[MHWChar]) {
				if (MHWChar == GChar) { // if they're in the same place
					BullCowCount.Bulls++; // increment bulls
				}
				else {
					BullCowCount.Cows++; // must be a cow
				}
			}
		}
	}
	if (BullCowCount.Bulls == WordLength) {
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	// treat 0 and 1 letter words as isograms
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen; // setup our map
	for (auto Letter : Word) 	// for all letters of the word
	{
		Letter = tolower(Letter); // handle mixed case
		if (LetterSeen[Letter]) {// if the letter is in the map
			return false; // we do NOT have an isogram
		}
		else {
			LetterSeen[Letter] = true;// add the letter to the map
		}
	}

	return true; // for example in cases where /0 is entered
}

bool FBullCowGame::IsLowercase(FString Word) const
{
	for (auto Letter : Word)
	{
		if (!islower(Letter)) // if not a lowercase letter
		{
			return false;
		}
	}
	return true;
}