# CleanPaws
A simple Program that takes in a list of words and checks for their contents against a file or string input.

Usage: CleanPaws <TextFile> <WordList>
      TextFile: The path to the plain text document to be parsed for matches.
      WordList: The file containing one word per line to be checked against the main document.


Todo:
      Accept std::cin for easier piping or comparison against a string such as " echo "this is dummy text" | ./CleanPaws <WordList>.
      Implement Exact Matches on Single words as opposed to string.find()