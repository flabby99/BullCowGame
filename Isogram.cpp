/* The purpose of this program is to read from a list of words as a .txt input file.
Then output a list of isograms in the input file as a new .txt file.
*/

#include <fstream>
#include <string>
#include <map>
#include <set>
#include <stdexcept>

// to make syntax Unreal friendly
#define TMap std::map
using int32 = int;
using FString = std::string;
using FText = std::string;

bool IsIsogram(FString);
struct StringComp;
std::set<FString, StringComp> ProcessInputFile(TMap<int32, int32>&);
void OutputFile(const std::set<FString, StringComp>& , const TMap<int32, int32>&);

//Check if the input string is an isogram - no duplicate letters.
bool IsIsogram(FString Word)
{
	TMap<char, bool> LetterSeen;
	for (auto Letter : Word) 	// for all letters of the word
	{
		if (!isalpha(Letter)) return false;
		else if (LetterSeen[Letter]) // if the letter is in the map 
		{	
			return false; // we do not have an isogram
		}
		else 
		{
			LetterSeen[Letter] = true; // add the letter to the map
		}
	}

	return true; //Word does not have duplicates or space, assumed to be isogram.
}

//Functor StringComp compares two strings in length order and then regular string order.
struct StringComp 
{
	bool operator() (const FString & lhs, const FString &rhs) const
	{
		if (lhs.length() != rhs.length()) return lhs.length() < rhs.length();
		else return lhs < rhs;
		//return lhs.compare(rhs);
	}
};

//Process the input text file to return a set of isograms from the file
//NumIsograms maps an integer to the number of isograms that length
std::set<FString, StringComp> ProcessInputFile(TMap<int32, int32> &NumIsograms) 
{
	FString buffer;
	FString InputFileName = "scrabble.txt";
	std::set<FString, StringComp> IsogramSet;
	std::ifstream InputFile;
	InputFile.open(InputFileName);
	if (InputFile.is_open()) 
	{
		while (std::getline(InputFile, buffer))
		{	
			if (buffer.length() > 4) {
				if (IsIsogram(buffer))
				{
					IsogramSet.insert(buffer);
					NumIsograms[buffer.length()]++;
				}
			}
		}
		InputFile.close();
	}
	else throw std::runtime_error("Oh dear, could not read from input file\n");
	return IsogramSet;
}

//Output the set of isograms to a .txt file, the first line of the file contains
//the number of words of different lengths and total number of words
void OutputFile(const std::set<FString, StringComp>& IsogramSet, 
				const TMap<int32, int32> &NumIsograms)
{
	FString OutputFileName = "isograms.txt";
	std::ofstream OutputFile;
	OutputFile.open(OutputFileName);
	if (OutputFile.is_open())
	{
		for (auto it = NumIsograms.cbegin(); it != NumIsograms.cend(); ++it)
		{
			//Output an integer and the number of isograms of that integer length.
			OutputFile << "{" << it->first << " " << it->second << "},";
		}
		OutputFile << std::endl;
		for (auto Word : IsogramSet)
		{
			OutputFile << Word << std::endl;
		}
		OutputFile.close();
	}
	else throw std::runtime_error("Oh dear, could not write to output file");
}

//Calls the functions to process the input file and produce the output file.
int main() 
{
	TMap<int32, int32> NumIsograms;
	std::set<FString, StringComp> IsogramSet = ProcessInputFile(NumIsograms);
	OutputFile(IsogramSet, NumIsograms);
	return 0;
}