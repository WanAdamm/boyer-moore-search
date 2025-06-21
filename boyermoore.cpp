#include <iostream>
#include <string>
#include <vector>
#define NO_OF_CHARS 256

// This program implements the Boyer-Moore string search algorithm,
// which efficiently finds a pattern within a given text.

void boyerMoore(const std::string &text, const std::string &pattern);

int main()
{
    // std::string text = "string matching is an essential problem in computer science. Many modern applications rely on efficient string matching techniques to search, analyze, and process text data. For instance, string matching is used in DNA sequence analysis, in search engines, and in detecting duplicate content. The string matching process compares a pattern with parts of a larger text to find where, if anywhere, the pattern occurs. Different string matching algorithms approach the problem in different ways, aiming to reduce the time and effort needed to find a match within a string of text.";
    // std::string pattern = "match within a string";

    std::string text;
    std::string pattern;

    std::cout << "Enter Text: " << std::endl;
    std::getline(std::cin, text);

    std::cout << "Enter Pattern: " << std::endl;
    std::getline(std::cin, pattern);

    boyerMoore(text, pattern);
}

void badCharHeuristic(const std::string &pattern, int *badchar) // Builds the bad character heuristic table based on the last occurrence of each character in the pattern
{
    int i;

    for (i = 0; i < NO_OF_CHARS; i++)
    {
        badchar[i] = -1; // Initialize the bad character table with -1, indicating no occurrence of the character
    }

    for (i = 0; i < pattern.length(); i++) // Fill the table with the last occurrence of each character in the pattern
    {
        badchar[(int)pattern[i]] = i;
    }
}

void goodSuffixHeuristic(const std::string &pattern, int patternLength, std::vector<int> &goodSuffixShiftTable)
{
    // Temporary array to hold the rightmost border positions of pattern suffixes
    std::vector<int> borderPosition(patternLength + 1);

    // Initialize indices to track the current position in the pattern and the rightmost border
    int currentPatternIndex = patternLength, rightmostBorderPosition = patternLength + 1;

    // Set the border of the full pattern as the initial border
    borderPosition[currentPatternIndex] = rightmostBorderPosition;

    // Compute border positions for suffixes in the pattern
    while (currentPatternIndex > 0)
    {
        // Attempt to find a border match between prefix and suffix in the pattern
        while (rightmostBorderPosition <= patternLength &&
               pattern[currentPatternIndex - 1] != pattern[rightmostBorderPosition - 1])
        {
            // If no match and no value already set in the shift table, compute the shift
            if (goodSuffixShiftTable[rightmostBorderPosition] == 0)
            {
                goodSuffixShiftTable[rightmostBorderPosition] = rightmostBorderPosition - currentPatternIndex;
            }

            // Move to the next possible border position
            rightmostBorderPosition = borderPosition[rightmostBorderPosition];
        }

        // Move to the previous character in the pattern
        currentPatternIndex--;
        rightmostBorderPosition--;

        // Save the current valid border
        borderPosition[currentPatternIndex] = rightmostBorderPosition;
    }

    // Finalize the good suffix shift table
    rightmostBorderPosition = borderPosition[0];

    for (int i = 0; i <= patternLength; i++)
    {
        // Fill in any missing shift values in the table
        if (goodSuffixShiftTable[i] == 0)
        {
            goodSuffixShiftTable[i] = rightmostBorderPosition;
        }

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
    int occurence = 0; // To count the number of pattern occurrences

    std::vector<int> goodSuffixShiftTable(patternLength + 1);

    goodSuffixHeuristic(pattern, patternLength, goodSuffixShiftTable);

    int badchar[NO_OF_CHARS]; // Initialize the bad character heuristic table

    badCharHeuristic(pattern, badchar); // Populate the bad character table

    int shift = 0; // Variable to track the shift based on bad character and good suffix heuristics

    // Search through the text for potential matches
    while (shift <= (textLength - patternLength))
    {
        int currentPatternIndex = patternLength - 1; // Start matching from the last character of the pattern

        while (currentPatternIndex >= 0 && pattern[currentPatternIndex] == text[shift + currentPatternIndex])
        {
            currentPatternIndex--; // Move backwards in the pattern for matching
        }

        if (currentPatternIndex < 0)
        {
            std::cout << "Pattern occurs at shift = " << shift << std::endl;
            occurence++; // Increment the occurrence count for the found match

            // After a full match, shift the pattern using the good suffix rule
            shift += goodSuffixShiftTable[0];
            std::cout << "Shift by: " << shift << std::endl;
        }
        else
        {
            // Shift the pattern using the maximum of the bad character rule and the good suffix rule
            shift += std::max(1, std::max(goodSuffixShiftTable[currentPatternIndex + 1], currentPatternIndex - badchar[text[shift + currentPatternIndex]]));
            std::cout << "Shift by: " << shift << std::endl;
        }
    }

    std::cout << std::endl
              << pattern << ": " << occurence << std::endl; // Output the total number of occurrences
}