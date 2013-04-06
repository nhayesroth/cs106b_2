/*
 * File: RandomWriter.cpp
 * ----------------------
 * Name: Nathan Hayes-Roth
 * Section: Tom Linford
 * This file "randomly" writes something based on a Markov Model. 
 * First, it prompts the user for a source file...
 * After prompting and receiving a degree of complexity n...
 * It produces a Markov map, linking all seeds of degree n to the characters that follow them.
 * It then uses these associations to generate 2000 characters, using relative frequency relationships
 * from the source text.
 */

#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "map.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
using namespace std;

/*constants*/
const int N_Output = 2000;

/*function prototypes*/
void getFile(ifstream & infile);
Map<string, Vector<char>> genMap(ifstream & infile);
string genStart(Map<string, Vector<char>> & map);
void writeRest(Map<string, Vector<char>> & map, string & start);

/*main program*/
int main() {
	ifstream infile;
	getFile(infile);
	Map<string, Vector<char>> map = genMap(infile);
	string start = genStart(map);
	writeRest(map, start);
	infile.close();
	return 0;
}

/*void getFile(ifstream & infile)
 * 
 *Prompt users to choose a source file and keep prompting until a valid file name is given
 */
void getFile(ifstream & infile){
	while (true){			//loop until a real file name is given
		string filename = getLine("Please enter the filename for your source material: ");;
		infile.clear();		//fix it if invalid names broke it
		infile.open(filename.c_str());	//accept strings as names
		if (infile.fail()){		//only accept real names
			cout << "	Sorry, " << filename << " could not be read." << endl<<endl;

		}
		else break;		//exit infinite loop
	}
	cout << "File was successfully read!"<<endl<<endl;
}

/*void genMap(ifstream & infile)
 *
 * generates a markov map of degree 'n' chosen by the user
 */
Map<string, Vector<char>> genMap(ifstream & infile){
	int order;
	while (true){	//loop until an appropriate order is chosen
		order = getInteger("What order of Markov model would you like to employ? ");
		if (order>0 && order<11) break; //range of acceptable orders
		cout << "\tPlease choose a value between [1-10]." << endl << endl;
	}
	cout << "Thank you. Your semi-random text is currently being generated..." << endl;
	Map<string, Vector<char>> map;		//the markov map
	string seedPlusOne;					// seedPlusOne will signify a seed+the following character
	while (!infile.eof()){				//keep looping until all the file has been read
		Vector<char> charList;			//the vector of characters following a seed
		seedPlusOne += infile.get();	//add the next character to the seed+character
		string seed = seedPlusOne.substr(0,order);	//break off all but the last character to find the seed
		if ((int) seedPlusOne.length() < (int)(order+1)){	//don't count anything until the seed has reached the right length
			continue;
		}
		if (!map.containsKey(seed)){	// found a new seed	
			map.put(seed, charList);	// add it to the map
			map[seed].add(seedPlusOne[order]);	//add the following character to the seed's list
			seedPlusOne=seedPlusOne.substr(1);	//drop the first character
			continue;
		}
		if (map.containsKey(seedPlusOne.substr(0,order))){	//found a redundant seed
			map[seed].add(seedPlusOne[order]);	//still add the following character to the seed's list
			seedPlusOne=seedPlusOne.substr(1);	//drop the first character
			continue;
		}
	}
	return map;
}

/*string genStart(Map<string, Vector<char>> & map)
 *
 *choose one of the most common seeds to start the random writing
 *
 */
string genStart(Map<string, Vector<char>> & map){
	string start;
	int maxFreq=0;
	foreach(string seed in map){	//check each seed in the map
		if (map.get(seed).size() > maxFreq){	//are they more frequent than all previous seeds?
			maxFreq = map.get(seed).size();		
			start=seed;							//if so, replace the previous most frequent with the current seed
		}
	}
	return start;
}

/*void writeRest(Map<string, Vector<char>> & map, string start)
 *
 *print the random text! finally!
 */
void writeRest(Map<string, Vector<char>> & map, string & start){

	cout << "MOST COMMON SEED: " << start;

	string seed = start;		//first seed will be the starting text (most frequent seed)
	int number = N_Output - start.length();	//how many additional characters are needed to reach 2000
	Vector<char> tempVector;	
	int randIndex;
	char ch;

	for (int i=0; i < number; i++){	//repeat as many times as is necessary to reach the correct output number
		tempVector = map.get(seed);	//set tempVector to the series of letters associated with the current seed
		randIndex = randomInteger(0, tempVector.size()-1);	//randomly choose an index from tempVector
		ch = tempVector[randIndex];	//find the associated character
		start += ch;				//and add it to the resulting text
		seed = seed.substr(1) + ch;	//remove the first character of the seed, add the new one, and repeat
	}
	cout << endl << start;
}