#include<string>
#include<iostream>
#include<vector>
using namespace std;

class ScoreMatrixElement {
public:
	int rPosition, sPosition;
	int score;
	bool hasLeftArrow;
	bool hasDiagnolArrow;
	bool hasUpArrow;
public:
	ScoreMatrixElement() :hasLeftArrow(false), hasDiagnolArrow(false), hasUpArrow(false) {}
	int numberOfArrow() {
		int number = 0;
		if (hasLeftArrow)number++;
		if (hasDiagnolArrow)number++;
		if (hasUpArrow)number++;
		return number;
	}
};
int max(int a, int b, int c) {
	int m = a;
	if (b > m)m = b;
	if (c > m)m = c;
	return m;
}
class AlignmentMachine {
public:
	int matchScore;
	int mismatchScore;
	int indelScore;
	string r, s;
	int rLen, sLen;//Length of string
	ScoreMatrixElement **ScoreMatrix;
public:
	void getThreeScores() {
		cin >> matchScore;
		cin >> mismatchScore;
		cin >> indelScore;
	}//Set the match/mismatch/indel Score
	void getTwoStrings() {
		cin >> r;
		cin >> s;
		rLen = r.length();
		sLen = s.length();
	}
	int calculateTheScore() {
		ScoreMatrix = new ScoreMatrixElement*[rLen + 1];
		for (int i = 0; i < rLen + 1; i++) {
			ScoreMatrix[i] = new ScoreMatrixElement[sLen + 1];
		}
		ScoreMatrix[rLen][sLen].score = 0;
		for (int i = rLen - 1; i >= 0; i--) {
			ScoreMatrix[i][sLen].score = ScoreMatrix[i + 1][sLen].score + indelScore;
			ScoreMatrix[i][sLen].hasUpArrow = true;
		}
		for (int i = sLen - 1; i >= 0; i--) {
			ScoreMatrix[rLen][i].score = ScoreMatrix[rLen][i + 1].score + indelScore;
			ScoreMatrix[rLen][i].hasLeftArrow = true;
		}
		for (int i = rLen - 1; i >= 0; i--) {
			for (int j = sLen - 1; j >= 0; j--) {
				int scoreLeft = ScoreMatrix[i][j + 1].score + indelScore;
				int scoreUp = ScoreMatrix[i + 1][j].score + indelScore;
				int scoreDig;
				if (r[i] == s[j])
					scoreDig = ScoreMatrix[i + 1][j + 1].score + matchScore;
				else
					scoreDig = ScoreMatrix[i + 1][j + 1].score + mismatchScore;
				int finalScore = max(scoreLeft, scoreUp, scoreDig);
				ScoreMatrix[i][j].score = finalScore;
				if (scoreLeft == finalScore)
					ScoreMatrix[i][j].hasLeftArrow = true;
				if (scoreUp == finalScore)
					ScoreMatrix[i][j].hasUpArrow = true;
				if (scoreDig == finalScore)
					ScoreMatrix[i][j].hasDiagnolArrow = true;
			}
		}
		return 0;
	}//Port the Matrix
	int findTheRoute() {
		int numberOfRoute = 1;
		bool hasMultipleRoute;
		int i, j;//iterator
		int rBuffer, sBuffer;//Mark the last visited position which has multiple route.
		vector<ScoreMatrixElement> DeletedElement;
		int deleteCase = -1;//1 = only left was deleted, 2 = only dig was deleted, 3 = both left and dig were deleted
		string rRoute, sRoute;//store the alignment
		do {
			i = 0;
			j = 0;
			hasMultipleRoute = false;
			rRoute = "";
			sRoute = "";
			while (i != rLen | j != sLen) {
				if (ScoreMatrix[i][j].numberOfArrow() == 0) {
					cout << "error! There is no arrow in point(" << i << "," << j << ")!!" << endl;
					return 0;
				}
				else if (ScoreMatrix[i][j].numberOfArrow() == 1) {
					if (ScoreMatrix[i][j].hasDiagnolArrow) {
						rRoute += r[i];
						sRoute += s[j];
						i++;
						j++;
					}
					else if (ScoreMatrix[i][j].hasLeftArrow) {
						rRoute += '_';
						sRoute += s[j];
						j++;
					}
					else if (ScoreMatrix[i][j].hasUpArrow) {
						rRoute += r[i];
						sRoute += '_';
						i++;
					}
					else {
						cout << "error!";
						return 0;
					}
				}
				else if (ScoreMatrix[i][j].numberOfArrow() == 2) {//follows to the left most arrow.
					hasMultipleRoute = true;
					rBuffer = i;
					sBuffer = j;//mark the location.
					if (ScoreMatrix[i][j].hasLeftArrow) {
						rRoute += '_';
						sRoute += s[j];
						j++;
					}
					else {
						rRoute += r[i];
						sRoute += s[j];
						i++;
						j++;
					}
				}
				else {//Number of arrow ==3
					hasMultipleRoute = true;
					rBuffer = i;
					sBuffer = j;
					rRoute += '_';
					sRoute += s[j];
					j++;
				}
			}
			if (hasMultipleRoute)
			{
				numberOfRoute++;
				for (int c = 0; c < DeletedElement.size(); c++) {
					int checkR = DeletedElement[c].rPosition;
					int checkS = DeletedElement[c].sPosition;
					if (rBuffer<checkR&&sBuffer <= checkS | rBuffer == checkR&&sBuffer<checkS) {
						ScoreMatrix[checkR][checkS] = DeletedElement[c];
						DeletedElement.erase(DeletedElement.begin() + c);
						c--;
					}
				}

				ScoreMatrixElement DeleteOne;
				DeleteOne.rPosition = rBuffer;
				DeleteOne.sPosition = sBuffer;
				DeleteOne.score = ScoreMatrix[rBuffer][sBuffer].score;
				DeleteOne.hasDiagnolArrow = ScoreMatrix[rBuffer][sBuffer].hasDiagnolArrow;
				DeleteOne.hasLeftArrow = ScoreMatrix[rBuffer][sBuffer].hasLeftArrow;
				DeleteOne.hasUpArrow = ScoreMatrix[rBuffer][sBuffer].hasUpArrow;
				DeletedElement.push_back(DeleteOne);

				if (ScoreMatrix[rBuffer][sBuffer].hasLeftArrow) {//delete the left most(left - dia - up order) arrow of the last visited point which has multipule arrows. 
					ScoreMatrix[rBuffer][sBuffer].hasLeftArrow = false;
					deleteCase = 1;
				}
				else {
					ScoreMatrix[rBuffer][sBuffer].hasDiagnolArrow = false;
				}
			}
			//print the alignment.
			cout << endl;
			cout << rRoute << endl;
			cout << sRoute << endl;
		} while (hasMultipleRoute);
		return numberOfRoute;
	}//return the number of route. Print all the alignment. Warning: this function will casue the change of matrix.
	void run() {
		getThreeScores();
		getTwoStrings();
		calculateTheScore();
		cout << ScoreMatrix[0][0].score << endl;
		findTheRoute();
	}//run the alignment
};

int main(int argc, char* args[]) {
	AlignmentMachine test;
	test.run();
	return 0;
}