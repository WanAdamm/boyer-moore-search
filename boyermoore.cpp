#include <iostream>
#include <string>
#define NO_OF_CHARS 130

// this program is using boyer moore algorithm to match find a pattern within a given string.

bool boyerMoore(const std::string &text, const std::string &pattern);

int main()
{
    std::string text = "ABAAABCD";
    std::string pattern = "BCD";

    bool result = boyerMoore(text, pattern);
    std::cout << result << std::endl;
}

void badCharHeuristic(const std::string &text, int *badchar) // takes in the text and an array of all the  character to create a lookup table of the last occurence of the character
{
    int i;

    for (i = 0; i < NO_OF_CHARS; i++)
    {
        badchar[i] = -1; // initialize all as -1 to indicate that theres no occurence of the character
    }

    for (i = 0; i < text.length(); i++) // process the text to find the location of all the last occurrece of the character
    {
        badchar[(int)text[i]] = i;
    }
}

bool boyerMoore(const std::string &text, const std::string &pattern)
{
    int textLength = text.size();
    int patternLength = pattern.size();

    int badchar[NO_OF_CHARS]; // initializing the lookup table for bad character rule

    badCharHeuristic(text, badchar); // filling in the lookup table for bad character rule

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

            // shift the pattern so that the next character in text aligns with the last occurence of it in pattern
            // The condition that the shift + pattern length must be less than text length is necessary for the case when pattern  occurs at end of text
            shift += (shift + patternLength < textLength) ? patternLength - badchar[text[shift + patternLength]] : 1;
            return true;
        }
        else
        {
            // shift the pattern so that the next character in text aligns with the last occurence of it in pattern
            // The max function is used to make sure that we get a positive shift
            shift += std::max(1, currentPatternIndex - badchar[text[shift + currentPatternIndex]]);
        }
    }

    return false;
}