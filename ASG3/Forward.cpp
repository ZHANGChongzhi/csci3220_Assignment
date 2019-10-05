#include<iostream>
#include<vector>
#include<fstream>
#include<string>

using namespace std;
/*
CSCI3220 2018-19 First Term Assignment 3
I declare that the assignment here submitted is original except for source
material explicitly acknowledged, and that the same or closely related material
has not been previously submitted for another course. I also acknowledge that I
am aware of University policy and regulations on honesty in academic work, and
of the disciplinary guidelines and procedures applicable to breaches of such
policy and regulations, as contained in the following websites.
University Guideline on Academic Honesty:
http://www.cuhk.edu.hk/policy/academichonesty/
Student Name: ZHANG Chongzhi
Student ID : 1155077072
*/

class HiddenMarkovModel {
public:
//need to be inputted
	int numOfState;
	vector<float> initialPrOfState;//length = num Of State
	vector<float> transitionPr;//length = num Of State^2
	int numOfEmission;// 4 if ACGT
	vector<char> emissionSymbol;//say A C G T
	vector<float> emissionPr;//length = num Of State * num Of Emission
	string outcome;//the likelyhood of which to be computed

//need to be calculated
	vector<int> outcomeIndex;//converter A = 0, C = 1 , G = 2, T = 3, like CAC = 101
	int lengthOfOutcome;
	vector<float> Alpha;//length = num Of State * length of outcome
	float dataLikelyhood;//the fianl answer.

public:
	void getInput() {
		float temp;//keep temp float
		char tempSymbol;//keep temp char

		cin >> numOfState;
		for (int i = 0; i < numOfState; i++) {
			cin >> temp;
			initialPrOfState.push_back(temp);
		}
		for (int i = 0; i < numOfState*numOfState; i++) {
			cin >> temp;
			transitionPr.push_back(temp);
		}
		cin >> numOfEmission;
		for (int i = 0; i < numOfEmission; i++) {
			cin >> tempSymbol;
			emissionSymbol.push_back(tempSymbol);
		}
		for (int i = 0; i < numOfState*numOfEmission; i++) {
			cin >> temp;
			emissionPr.push_back(temp);
		}
		while (cin.get() != '\n');//clear the buffer, in case getline will get \n;
		getline(cin, outcome);

		//transfer outcome string into outcomeindex. like CAC -> 101 in a vector.
		for (int i = 0; i < outcome.length(); i++) {
			tempSymbol = outcome[i];
			for (int j = 0; j < emissionSymbol.size(); j++) {
				if (emissionSymbol[j] == tempSymbol) {
					outcomeIndex.push_back(j);
				}
			}
		}
		
	}//get the input.

	void calc() {
		lengthOfOutcome = outcome.length();

		//iteratively calculate Alpha(t,i) -> vector Alpha[ t * num of state + i ]
		float Alphati; //keep the result in each step
		int emissionPrIndex;//emissionPr index = i* numOfEmisson + outcomeIndex[t] 
		for (int t = 0; t < lengthOfOutcome; t++) {
			for (int i = 0; i < numOfState; i++) {
				if (t == 0) {
					//first step
					emissionPrIndex = i * numOfEmission + outcomeIndex[0];//store the index first.
					Alphati = initialPrOfState[i] * emissionPr[emissionPrIndex];
					//cout << "result is " << Alphati << endl;
					Alpha.push_back(Alphati);//keep the alpha in a vector.	
				}
				else {
					Alphati = 0;//initialize
					for (int j = 0; j < numOfState; j++) {
						emissionPrIndex = i*numOfEmission + outcomeIndex[t];//this is the index of emissionPr
						Alphati += Alpha[(t-1)*numOfState + j] * emissionPr[emissionPrIndex] * transitionPr[j*numOfState + i];
					}
					Alpha.push_back(Alphati);//keep the alpha in a vector.
				}
			}
		}
		
		dataLikelyhood = 0;//initialize
		for (int j = 0; j < numOfState; j++) {
			dataLikelyhood += Alpha[(lengthOfOutcome - 1)*numOfState + j];//sigma alpha(m,j). 
		}
		
	}//calculate alpha.

};

int main(int argc, char*args[]) {
	HiddenMarkovModel test;
	test.getInput();
	test.calc();
	cout << test.dataLikelyhood<<endl;
	return 0;
}