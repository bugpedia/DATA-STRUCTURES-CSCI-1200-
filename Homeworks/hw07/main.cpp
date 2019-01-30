// -----------------------------------------------------------------
// HOMEWORK 7 WORD FREQUENCY MAPS
//
// You may use all of, some of, or none of the provided code below.
// You may edit it as you like (provided you follow the homework
// instructions).
// -----------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cassert>
#include <stdlib.h>
#include "MersenneTwister.h"

using namespace std;

// ASSIGNMENT: FILL IN YOUR OWN MAP STRUCTURE
typedef  map<string, map<string, int> > MY_MAP;
typedef  map<string, map<string, map<string, int> > > MY_MAP_W3;

// Custom helper function that reads the input stream looking for
// double quotes (a special case delimiter needed below), and white
// space.  Contiguous blocks of alphabetic characters are lowercased &
// packed into the word.
bool ReadNextWord(std::istream &istr, std::string &word) {
  char c;
  word.clear();
  while (istr) {
    // just "peek" at the next character in the stream
    c = istr.peek();
    if (isspace(c)) {
      // skip whitespace before a word starts
      istr.get(c);
      if (word != "") {
      	// break words at whitespace
      	return true;
      }
    } else if (c == '"') {
      // double quotes are a delimiter and a special "word"
      if (word == "") {
      	istr.get(c);
      	word.push_back(c);
      }
      return true;
    } else if (isalpha(c)) {
      // this a an alphabetic word character
      istr.get(c);
      word.push_back(tolower(c));
    } else {
      // ignore this character (probably punctuation)
      istr.get(c);
    }
  }
  return false;
}

// Custom helper function that reads the input stream looking a
// sequence of words inside a pair of double quotes.  The words are
// separated by white space, but the double quotes might not have
// space between them and the neighboring word.  Punctuation is
// ignored and words are lowercased.
std::vector<std::string> ReadQuotedWords(std::istream &istr) {
  // returns a vector of strings of the different words
  std::vector<std::string> answer;
  std::string word;
  bool open_quote = false;
  while (ReadNextWord(istr,word)) {
    if (word == "\"") {
      if (open_quote == false) { open_quote=true; }
      else { break; }
    } else {
      // add each word to the vector
      answer.push_back(word);
    }
  }
  return answer;
}

// Loads the sample text from the file, storing it in the map data
// structure Window specifies the width of the context (>= 2) of the
// sequencing stored in the map.  parse_method is a placeholder for
// optional extra credit extensions that use punctuation.
void LoadSampleText(MY_MAP &data, const std::string &filename, int window, 
  const std::string &parse_method, string &last_word) {
  // open the file stream
  std::ifstream istr(filename.c_str());
  if (!istr) { 
    std::cerr << "ERROR cannot open file: " << filename << std::endl; 
    exit(1);
  } 
  // verify the window parameter is appropriate
  if (window < 2) {
    std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
  }
  // verify that the parse method is appropriate
  bool ignore_punctuation = false;
  if (parse_method == "ignore_punctuation") {
    ignore_punctuation = true;
  } else {
    std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
    exit(1);
  }

  cout << "Loaded " << filename << " with window = " << window
       << " and parse method = " << parse_method << endl;
  cout << endl;

  string word;
  string prev = "@"; 

  while (ReadNextWord(istr,word)) {
    // skip the quotation marks (not used for this part)
    if (word == "\"") continue;
    
    // initializing the map that will be the value for MY_MAP
    map<string, int> occurrences;
    
    // If the pair does not exist, make a pair and insert it
    if (data.find(word) == data.end())
    {
      data.insert(make_pair(word, occurrences));
    }
    
    // If we are past the first word, and a previous word exists, then
    // either add a pair in the inside map or increment it
    if (prev != "@")
    {
      if ((data[prev]).find(word) == (data[prev]).end())
        (data[prev]).insert(make_pair(word, 1));
      else
        (data[prev])[word]++;
    }

    // store this word in prev, so that in the next iteration, we have 
    // the previous word
    prev = word;
  }

  // stores the last word in the file, for use in the "print" command
  last_word = prev;
}

// Loads sample text from file if given window = 3. Thus, the parameter
// passed through for data uses a different typedef as opposed to 
// LoadSampleText
void LoadSampleText_3(MY_MAP_W3 &data3, const string &filename, int window,
  const string &parse_method, string &last_word)
{
  // open the file stream
  std::ifstream istr(filename.c_str());
  if (!istr) { 
    std::cerr << "ERROR cannot open file: " << filename << std::endl; 
    exit(1);
  } 
  // verify the window parameter is appropriate
  if (window != 3) {
    std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
  }
  // verify that the parse method is appropriate
  bool ignore_punctuation = false;
  if (parse_method == "ignore_punctuation") {
    ignore_punctuation = true;
  } else {
    std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
    exit(1);
  }

  cout << "Loaded " << filename << " with window = " << window
       << " and parse method = " << parse_method << endl;
  cout << endl;

  string prev1 = "@";
  string prev2 = "@";
  string word;

  while (ReadNextWord(istr,word))
  {    
    // skip quotation marks
    if (word == "\"") continue;

    // initialize value of the first map
    map<string, map<string, int> > first_occurrences;

    // if the word isn't in the map, then make a pair with the word and 
    // a map<string, map<string,int> > value type
    if (data3.find(word) != data3.end())
    {
      data3.insert(make_pair(word, first_occurrences));
    }

    // assuming we now have a word in all three variables...
    if (prev1 != "@" && prev2 != "@")
    {
      // initialize a map<string, int> which will later be the value
      // for the second map in the structure
      map<string, int> second_occurrences;

      // if we can't find the second word, insert a pair using 
      // second_occurrences
      if (data3[prev2].find(prev1) == data3[prev2].end())
      {
        data3[prev2].insert(make_pair(prev1, second_occurrences));

        // insert the third word if it doesn't exist, else
        // increment the value of the word
        if (data3[prev2][prev1].find(word) == data3[prev2][prev1].end())
        {
          data3[prev2][prev1].insert(make_pair(word, 1));
        }
        else
        {
          data3[prev2][prev1][word]++;
        }
      }
      else
      {
        // same thing as the "if" above
        if (data3[prev2][prev1].find(word) == data3[prev2][prev1].end())
        {
          data3[prev2][prev1].insert(make_pair(word, 1));
        }
        else
        {
          data3[prev2][prev1][word]++;
        }
      }
    }

    // makes prev1, prev2, and word all go to the next work in the 
    // text file
    if (prev1 == "@")
    {
      prev1 = word;
    }
    else
    {
      prev2 = prev1;
      prev1 = word;
    }
  }

  // stores the last word, for the print function
  last_word = prev1;
}


// Helper function is called when the "generate" command is seen, 
// using a given word to construct a sentence of certain length by either
// selecting the most frequently occurring word, or choosing it randomly
// while taking into account the frequencies of the words
void NextWord(vector<string>& sentence, bool random_flag, int length, 
  MY_MAP& data, MY_MAP_W3& data3, MTRand& mtrand_1, int window)
{
  // used for window = 3
  bool driver = false;

  if (window == 2)
  {
    // User asked for "most common"
    if (!random_flag)
    {
      string word = sentence[0];
      cout << word;

      // generate as many words as the user requests by using a for loop
      for (int i = 0; i < length; i++)
      {
        MY_MAP::iterator itr = data.find(word);

        int max = 0;  

        map<string, int>::iterator itr2 = (itr->second).begin();
        
        // go through all of the values and find the max value
        while (itr2 != (itr->second).end())
        {
          if (itr2->second > max)
            max = itr2->second;

          itr2++;
        }

        // whichever word is the associated with the max value, set it equal
        // to "word" and then print it
        itr2 = (itr->second).begin();
        while (itr2 != (itr->second).end())
        {
          if (itr2->second == max)
          {
            word = itr2->first;
            break;
          }

          itr2++;
        }

        cout << " " << word;          
      }
    }
          
    // "Random" is specified
    else
    {
      string word = sentence[0];
      cout << word;

      for (int i = 0; i < length; i++)
      {
        MY_MAP::iterator itr = data.find(word);

        int c = (itr->second).size();
        int index = 0;

        vector<string> words;

        // push back all instances of the word into a vector of words, 
        // effectively taking care of all the probabilities
        map<string, int>::iterator itr2 = (itr->second).begin();
        while (itr2 != (itr->second).end())
        {
          words.push_back(itr2->first);
          itr2++;
        }

        // use mtrand to generate a random index, to index the vector of 
        // words
        index = mtrand_1.randInt(words.size()-1);
        word = words[index];

        cout << " " << word;
      }
    }

    cout << endl;
    cout << endl;
  }

  else if (window == 3)
  {
    // if only one word is inputted for the generate command
    if (sentence.size() == 1)
    {
      // if "most_common" was specified
      if (!random_flag)
      {
        string word = sentence[0];

        MY_MAP_W3::iterator itr = data3.find(word);

        int max = 0;  

        MY_MAP::iterator itr2 = (itr->second).begin();

        // finds the max, by going through to the third map and adding all the 
        // values seen
        while (itr2 != (itr->second).end())
        {
          map<string, int>::iterator itr3 = (itr2->second).begin();
          int d = 0;

          while (itr3 != (itr2->second).end())
          {
            d += itr3->second;
            itr3++;
          }

          if (d > max)
            max = d;

          itr2++;
        }

        // sets the word equal to whichever one is the max
        itr2 = (itr->second).begin();
        while (itr2 != (itr->second).end())
        {
          if ((itr2->second).size() == max)
          {
            word = itr2->first;
            break;
          }

          itr2++;
        }

        // push it back to sentence, so that we can then continue using these
        // two words as if we had a two word sentence command
        sentence.push_back(word);
        driver = true;          
      }
      else
      {
        string word = sentence[0];
        
        MY_MAP_W3::iterator itr = data3.find(word);
        vector<string> words;

        MY_MAP::iterator itr2 = (itr->second).begin();

        // find the max, and consecutively push back all occurrences of 
        // words into a vector, to account for probabilities
        while (itr2 != (itr->second).end())
        {
          map<string, int>::iterator itr3 = (itr2->second).begin();
          int d = 0;

          while (itr3 != (itr2->second).end())
          {
            d += itr3->second;
            itr3++;
          }
          for (int j = 0; j < d; j++)
          {
            words.push_back(itr2->first);
          }

          itr2++;
        }

        // generate a random index
        int index = mtrand_1.randInt(words.size()-1);
        sentence.push_back(words[index]);
        driver = true;       
      }
    }
    if (sentence.size() == 2)
    {
      if (!random_flag)
      {
        string first = sentence[0];
        string second = sentence[1];

        cout << first << " " << second;

        // used if the command was initially a one word generate and then
        // continued on to the two word generate command
        if (driver) length = length-1;

        // loop through the amount of words inputted
        for (int i = 0; i < length; i++)
        {
          MY_MAP_W3::iterator itr = data3.find(first);
          MY_MAP::iterator itr2 = (itr->second).find(second);
          map<string, int>::iterator itr3 = (itr2->second).begin();

          string word;
          int max = 0;

          // find the max, and set the word equal to the one whose value is
          // the max
          while (itr3 != (itr2->second).end())
          {
            if (itr3->second > max)
            {
              max = itr3->second;
              word = itr3->first;
            }

            itr3++;
          }

          cout << " " << word;
          first = second;
          second = word;  
        }
      }
      else
      {
        string first = sentence[0];
        string second = sentence[1];

        cout << first << " " << second;

        // used if the command was initially a one word generate and then
        // continued on to the two word generate command
        if (driver) length = length-1;
        for (int i = 0; i < length; i++)
        {
          // set iterators to the first and the second words
          MY_MAP_W3::iterator itr = data3.find(first);
          MY_MAP::iterator itr2 = (itr->second).find(second);
          map<string, int>::iterator itr3 = (itr2->second).begin();

          string word;
          vector<string> words;

          // push back all words to a vector, to account for probabilities
          while (itr3 != (itr2->second).end())
          {
            int d = 0;
            d += itr3->second;

            for (int j = 0; j < d; j++)
            {
              words.push_back(itr3->first);
            }
            
            itr3++;
          }

          // generate a random index
          int index = mtrand_1.randInt(words.size()-1);
          word = words[index];

          cout << " " << word;
          first = second;
          second = word;  
        }
      }
    }
  
    cout << endl;
    cout << endl;
  }
}


int main(){

  // ASSIGNMENT: THE MAIN DATA STRUCTURE
  MY_MAP data;
  MY_MAP_W3 data3;
  string last_word;
  MTRand mtrand_1;

  // Parse each command
  std::string command;    

  std::string filename;
  std::string parse_method;
  std::cin >> command >> filename >> parse_method;  

  if (window == 2)
  {
    LoadSampleText(data, filename, window, parse_method, last_word);

    while (std::cin >> command) {

      // print the portion of the map structure with the choices for the
      // next word given a particular sequence.
      if (command == "print") {
        std::vector<std::string> sentence = ReadQuotedWords(std::cin);

        MY_MAP::iterator itr = data.find(sentence[0]);
        int c;

        if (sentence[0] == last_word) c = 1;
        else c = 0;

        cout << itr->first;

        map<string, int>::iterator itr2 = (itr->second).begin();
        
        // find the value for c, the amount of times the word occurs
        while (itr2 != (itr->second).end())
        {
          c += itr2->second;
          itr2++;
        }

        cout << " (" << c << ")" << endl;

        itr2 = (itr->second).begin();

        // print out each sub occurrence, and the amount of times they show up
        while (itr2 != (itr->second).end())
        {
          cout << itr->first << " " << itr2->first << " (" << itr2->second << ")" 
               << endl;
          itr2++;
        }

        cout << endl;
      } 

      // generate the specified number of words 
      else if (command == "generate") {
        std::vector<std::string> sentence = ReadQuotedWords(std::cin);
        // how many additional words to generate
        int length;
        std::cin >> length;
        std::string selection_method;
        std::cin >> selection_method;
        bool random_flag;
        if (selection_method == "random") {
  	      random_flag = true;
        } else {
        	assert (selection_method == "most_common");
        	random_flag = false;
        }

        NextWord(sentence, random_flag, length, data, data3, mtrand_1, window);
    
      } else if (command == "quit") {
        break;
      } else {
        std::cout << "WARNING: Unknown command: " << command << std::endl;
      }
    }
  }

  else if (window == 3)
  {
    // load the sample text file
    LoadSampleText_3(data3, filename, window, parse_method, last_word);

    while (std::cin >> command) {
      // print the portion of the map structure with the choices for the
      // next word given a particular sequence.
      if (command == "print") {
        std::vector<std::string> sentence = ReadQuotedWords(std::cin);

        if (sentence.size() == 1)
        {  
          MY_MAP_W3::iterator itr = data3.find(sentence[0]);
          int c;

          if (sentence[0] == last_word) c = 1;
          else c = 0;

          cout << itr->first;

          MY_MAP::iterator itr2 = (itr->second).begin();

          while (itr2 != (itr->second).end())
          {
            int d = 0;
            map<string, int>::iterator itr3 = (itr2->second).begin();

            // find the sum of the values in the innermost map
            while (itr3 != (itr2->second).end())
            {
              d += itr3->second;
              itr3++;
            }

            // add them to c, for the command inputted
            c += d;
            itr2++;
          }

          cout << " (" << c << ")" << endl;

          itr2 = (itr->second).begin();

          // takes care of outputting the two word combos
          while (itr2 != (itr->second).end())
          {
            int d = 0;
            map<string, int>::iterator itr3 = (itr2->second).begin();

            while (itr3 != (itr2->second).end())
            {
              d += itr3->second;
              itr3++;
            }

            cout << itr->first << " " << itr2->first << " (" << d << ")" 
                 << endl;
            itr2++;
          }

          cout << endl;
        }

        // if the generate command have two words
        else if (sentence.size() == 2)
        {
          MY_MAP_W3::iterator itr = data3.find(sentence[0]);
          int c = 0; 

          cout << itr->first << " ";

          MY_MAP::iterator itr2 = (itr->second).find(sentence[1]);

          cout << itr2->first;

          map<string, int>::iterator itr3 = (itr2->second).begin();

          // find c, for the two word combo
          while (itr3 != (itr2->second).end())
          {
            c += itr3->second;
            itr3++;
          }

          cout << " (" << c << ")" << endl;

          itr3 = (itr2->second).begin();

          // output the three word combos
          while (itr3 != (itr2->second).end())
          {
            cout << itr->first << " " << itr2->first << " " << itr3->first 
                 << " (" << itr3->second << ")"   << endl;
            itr3++;
          }

          cout << endl;
        }
      }

      // generate the specified number of words 
      else if (command == "generate") {
        std::vector<std::string> sentence = ReadQuotedWords(std::cin);
        // how many additional words to generate
        int length;
        std::cin >> length;
        std::string selection_method;
        std::cin >> selection_method;
        bool random_flag;
        if (selection_method == "random") {
          random_flag = true;
        } else {
          assert (selection_method == "most_common");
          random_flag = false;
        }

        NextWord(sentence, random_flag, length, data, data3, mtrand_1, window);

      } else if (command == "quit") {
        break;
      } else {
        std::cout << "WARNING: Unknown command: " << command << std::endl;
      }
    }
  }
}