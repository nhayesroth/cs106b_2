/*
 * File: WordLadder.cpp
 * --------------------
 * Name: Nathan Hayes-Roth
 * Section: Linford
 * This file prompts users for two english words, which it uses to produce the shortest possible word-ladder.
 */

#include <iostream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "simpio.h"
#include "vector.h"
using namespace std;

/*constants*/
const string alphabet="abcdefghijklmnopqrstuvwxyz";
const Lexicon english ("EnglishWords.dat");

/*function prototypes*/
string startPrompt();
string endPrompt(string start);
Vector<string> wordLadder(string & start, string & end);
void printVectors(Vector<string> ladder);

/*main program*/
int main() {
	string start = startPrompt();	//get start word
	string end = endPrompt(start);	//get end word
	Vector<string> ladder = wordLadder(start, end);	//get ladder
	printVectors(ladder);	//print ladder
	return 0;
}

/*startPrompt()
 *prompts for and returns the start word
 */
string startPrompt(){
	string start = getLine("Please enter start word: ");		//prompt for start word
	for (int i=0; i<start.length(); i++){
		start[i]=tolower(start[i]);								//sets all characters to lower case
	}
	if (!english.contains(start)){
		cout << "     Sorry, but that's not a word." << endl;	//only accept valid words
		startPrompt();
	}
	return start;
}

/*endPrompt(string start)
 *prompts for and returns the end word
 */
string endPrompt(string start){
	string end = getLine("Please enter end word: ");			//prompt for end word
	for (int i=0; i<end.length(); i++){
		end[i]=tolower(end[i]);									//sets all characters to lower case
	}
	if (!english.contains(end) || start.length()!=end.length()){ //only accept valid words
		cout << "     Sorry, but word is invalid." << endl;	
		cout << "     Make sure your end word is English and of the right length." << endl;
		endPrompt(start);
	}
	return end;
}


/*wordLadder(string & start, string & end)
 *finds the shortest word ladder between the start and end words
 */
Vector<string> wordLadder(string & start, string & end){		//call by reference
	Lexicon usedWords;											//track used words
	Vector<string> ladder (1, start);							//word ladder
	Queue< Vector<string> > queue;								//empty queue of word ladders (ordered by number of changes)
	queue.enqueue(ladder);										//add the ladder of length 0 steps (start word)
	usedWords.add(start);										//add the start word to the list of used words
	while (!queue.isEmpty()){									//while the queue is not empty
		Vector<string> tempLadder = queue.dequeue();			
		string tempWord = tempLadder[tempLadder.size()-1];		//call the last word in a ladder tempWord
		if (tempWord == end){
			return tempLadder;									//return ladder if it ends on the correct word
		}
		for (int i=0; i<tempWord.length(); i++){				//step through each character of the ladder's final word 
			string tempWord = tempLadder[tempLadder.size()-1];	//reset the spelling to its correct state (after each round of for loop)
			for (int j=0; j<alphabet.length(); j++){			//step through each letter of alphabet
				tempWord[i]=alphabet[j];						//create all possible changes
				if (english.contains(tempWord) && !usedWords.contains(tempWord)){ //check to see if new word is valid and not already used
					Vector<string> tempLadderCopy = tempLadder;	//create a copy of the current ladder
					tempLadderCopy.add(tempWord);				//add the new word to the end of the copy
					usedWords.add(tempWord);					//add the new word to the list of used words
					if (tempWord == end) return tempLadderCopy;	//immediately return the current ladder if it ends on the correct word
					queue.enqueue(tempLadderCopy);				//or add the ladder to the queue and continue looping
				}
			}
		}
	}
	cout << "Sorry, no ladder exists.";
	ladder.clear();
	return ladder;
}

/*printVectors(Vector<string> ladder)
 *prints all elements of a vector, separated by commas and ended with a period
 */
void printVectors(Vector<string> ladder){
	if (ladder.size()<1) return;			//don't print anything if the ladder doesn't exist
	cout << endl << "Ladder[";
	for (int i=0; i<ladder.size(); i++){	//step through the ladder
		if (i>0) cout << ", ";				//put commas in the right spots
		cout << ladder[i];					//print each element
	}
	cout <<"]";
}