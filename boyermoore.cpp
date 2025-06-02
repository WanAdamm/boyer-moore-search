#include <iostream>
#include <string>
#include <vector>
#define NO_OF_CHARS 130

// this program is using boyer moore algorithm to match find a pattern within a given string.

void boyerMoore(const std::string &text, const std::string &pattern);

int main()
{
    std::string text = "string matching is an essential problem in computer science. Many modern applications rely on efficient string matching techniques to search, analyze, and process text data. For instance, string matching is used in DNA sequence analysis, in search engines, and in detecting duplicate content. The string matching process compares a pattern with parts of a larger text to find where, if anywhere, the pattern occurs. Different string matching algorithms approach the problem in different ways, aiming to reduce the time and effort needed to find a match within a string of text.";
    std::string pattern = "string";

    boyerMoore(text, pattern);
}

void badCharHeuristic(const std::string &pattern, int *badchar) // takes in the text and an array of all the  character to create a lookup table of the last occurence of the character
{
    int i;

    for (i = 0; i < NO_OF_CHARS; i++)
    {
        badchar[i] = -1; // initialize all as -1 to indicate that theres no occurence of the character
    }

    for (i = 0; i < pattern.length(); i++) // process the text to find the location of all the last occurrece of the character
    {
        badchar[(int)pattern[i]] = i;
    }
}

void goodSuffixHeuristic(const std::string &pattern, int patternLength, std::vector<int> &goodSuffixShiftTable)
{
    // Temporary array to hold the rightmost border positions of pattern suffixes
    std::vector<int> borderPosition(patternLength + 1);

    // Initialize the indices:
    // currentPatternIndex: moving leftward in the pattern
    // rightmostBorderPosition: tracks suffix border matches (i.e., where prefix = suffix)
    int currentPatternIndex = patternLength, rightmostBorderPosition = patternLength + 1;

    // Set the border of the full pattern as the initial border
    borderPosition[currentPatternIndex] = rightmostBorderPosition;

    // First pass: compute border positions for suffixes
    while (currentPatternIndex > 0)
    {
        // Look for a border match between pattern[0..i-1] and a suffix pattern[j..m-1]
        while (rightmostBorderPosition <= patternLength &&
               pattern[currentPatternIndex - 1] != pattern[rightmostBorderPosition - 1])
        {
            // If no match and no value already set in the shift table, compute the shift
            if (goodSuffixShiftTable[rightmostBorderPosition] == 0)
            {
                goodSuffixShiftTable[rightmostBorderPosition] = rightmostBorderPosition - currentPatternIndex;
            }

            // Move j (border pointer) to the next border candidate
            rightmostBorderPosition = borderPosition[rightmostBorderPosition];
        }

        // Move both pointers one step left
        currentPatternIndex--;
        rightmostBorderPosition--;

        // Save the current valid border
        borderPosition[currentPatternIndex] = rightmostBorderPosition;
    }

    // Second pass: finalize the good suffix shift table
    rightmostBorderPosition = borderPosition[0];

    for (int i = 0; i <= patternLength; i++)
    {
        // Fill in any entries that were not updated in the first loop
        if (goodSuffixShiftTable[i] == 0)
        {
            goodSuffixShiftTable[i] = rightmostBorderPosition;
        }

        // Correction: `if (rightmostBorderPosition == currentPatternIndex)` (not assignment `=`)
        if (i == rightmostBorderPosition)
        {
            rightmostBorderPosition = borderPosition[rightmostBorderPosition];
        }
    }
}

void boyerMoore(const std::string &text, const std::string &pattern)
{
    int textLength = text.size();
    int patternLength = pattern.size();
    int occurence = 0; // keep track of the occurence of the pattern

    std::vector<int> goodSuffixShiftTable(patternLength + 1);

    goodSuffixHeuristic(pattern, patternLength, goodSuffixShiftTable);

    int badchar[NO_OF_CHARS]; // initializing the lookup table for bad character rule

    badCharHeuristic(pattern, badchar); // filling in the lookup table for bad character rule

    int shift = 0; // used to shift n steps after using the bad character and good suffix rule.

    while (shift <= (textLength - patternLength)) // no need to go on if shift is too long
    {
        int currentPatternIndex = patternLength - 1; // start matching from the last character in the pattern

        while (currentPatternIndex >= 0 && pattern[currentPatternIndex] == text[shift + currentPatternIndex])
        {
            currentPatternIndex--; // working our way back until the first character in the pattern
        }

        if (currentPatternIndex < 0)
        {
            std::cout << "pattern occur at shift = " << shift << std::endl;
            occurence++; // increase the occurence counter

            // shifting after a full match
            shift += goodSuffixShiftTable[0];
        }
        else
        {
            // shift the pattern so that the next character in text aligns with the last occurence of it in pattern
            // The max function is used to make sure that we get a positive shift
            std::cout << "good suffix: " << goodSuffixShiftTable[currentPatternIndex + 1] << "   " << "bad character: " << currentPatternIndex - badchar[text[shift + currentPatternIndex]] << std::endl;
            shift += std::max(1, std::max(goodSuffixShiftTable[currentPatternIndex + 1], currentPatternIndex - badchar[text[shift + currentPatternIndex]]));
        }
    }

    std::cout << std::endl << pattern << ":" << occurence << std::endl; 
}