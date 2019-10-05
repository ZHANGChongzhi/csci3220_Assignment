#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<fstream>
/*
CSCI32202018 - 19First TermAssignment 2I declare that the assignment here submitted is originalexcept for
source material explicitly acknowledged, and that the same or closely related material has not been previously
submitted for another course.I also acknowledge that I am aware of University policy and regulations on 
honesty in academic work, and of the disciplinary guidelines and procedures applicable to breaches of 
such policy and regulations, as contained in the following websites.

University Guideline on Academic Honesty : http://www.cuhk.edu.hk/policy/academichonesty/Student

Name: ZHANG Chongzhi
Student ID  : 1155077072
*/
using namespace std;

class Node
{
public:
	string name;
	int inNum;//number of edges point in this node
	int outNum;//number of edges point out from this node
	bool visited;//using in DFS to determing whether the node is visited.
	vector<int> outEdges;//keeps index of outgoing edges.
	vector<int> inOutEdges;//keeps both in and out edges. Uings for DFS

public:
	Node(string s) :inNum(0), outNum(0), visited(false) {
		name = s;
	}
};

class Graph
{
public:
	vector<Node> nodeList;//all the nodes keeps here.
	stack<int> current;
	stack<int> complete;
	int startNode;
	int endNode;//All the int is the index in Nodelist

public:
	Graph() :startNode(-1), endNode(-1) {}

	void bulid() {//bulid the graph form input.
		string shortRead;//for instance CAC
		string N1name;//CA
		string N2name;//AC
		int N1p = -1, N2p = -1;//this is the index of the node in nodeList.

		bool existN1;
		bool existN2;//check if the node have already in the nodeList.


		while (getline(cin, shortRead)) {
			existN1 = false;
			existN2 = false;
			if (shortRead.size() == 0)break;
			N1name = shortRead;
			N2name = shortRead;
			N1name.erase(shortRead.size() - 1, 1);
			N2name.erase(0, 1);//keep s[1,k-1] and s[2,k] to two name.

			for (int i = 0; i < nodeList.size(); i++) {
				if (nodeList[i].name == N1name) {
					existN1 = true;
					N1p = i;
				}
			}//check if N1 have already exist
			if (!existN1) {
				Node N1(N1name);
				nodeList.push_back(N1);
				N1p = nodeList.size() - 1;
			}
			for (int i = 0; i < nodeList.size(); i++) {
				if (nodeList[i].name == N2name) {
					existN2 = true;
					N2p = i;
				}
			}//check if N2 have already exist
			if (!existN2) {
				Node N2(N2name);
				nodeList.push_back(N2);
				N2p = nodeList.size() - 1;

			}
			nodeList[N1p].outNum++;
			nodeList[N2p].inNum++;
			nodeList[N1p].outEdges.push_back(N2p);

			nodeList[N1p].inOutEdges.push_back(N2p);
			nodeList[N2p].inOutEdges.push_back(N1p);
		}
	}

	void bulidByFile() {//testing. Using a input.txt to input rather than cin. Dont care.
		string shortRead;
		string N1name;
		string N2name;
		int N1p = -1, N2p = -1;

		bool existN1;
		bool existN2;

		ifstream inputFile("input.txt");

		while (getline(inputFile, shortRead)) {
			existN1 = false;
			existN2 = false;
			if (shortRead.size() == 0)break;
			N1name = shortRead;
			N2name = shortRead;
			N1name.erase(shortRead.size() - 1, 1);
			N2name.erase(0, 1);

			for (int i = 0; i < nodeList.size(); i++) {
				if (nodeList[i].name == N1name) {
					existN1 = true;
					N1p = i;
				}
			}
			if (!existN1) {
				Node N1(N1name);
				nodeList.push_back(N1);
				N1p = nodeList.size() - 1;
			}
			for (int i = 0; i < nodeList.size(); i++) {
				if (nodeList[i].name == N2name) {
					existN2 = true;
					N2p = i;
				}
			}
			if (!existN2) {
				Node N2(N2name);
				nodeList.push_back(N2);
				N2p = nodeList.size() - 1;

			}
			nodeList[N1p].outNum++;
			nodeList[N2p].inNum++;
			nodeList[N1p].outEdges.push_back(N2p);

			nodeList[N1p].inOutEdges.push_back(N2p);
			nodeList[N2p].inOutEdges.push_back(N1p);
		}

		inputFile.close();
	}
	
	bool isConnected() {
		//using DFS to check is the graph is a connected graph
		if (nodeList.size() == 0)
			return false;
		DFS(nodeList[0]);
		for (int i = 0; i < nodeList.size(); i++) {
			if (nodeList[i].visited == false) {
				cout << nodeList[i].name << " is not visited" << endl;
				return false;
			}
		}//connected only when all the nodes is visited
		return true;
	}

	void DFS(Node &P) {//DFS.
		P.visited = true;
		//cout << "visit: " << P.name << endl;
		for (int i = 0; i < P.inOutEdges.size(); i++) {
			if (nodeList[P.inOutEdges[i]].visited == false) {
				DFS(nodeList[P.inOutEdges[i]]);
			}
		}
	}

	bool hasEP() {
		//check if the graph have EP.
		// also helps find the start and end position(if have)
		int normalNum = 0, startNum = 0, endNum = 0;
		for (int i = 0; i < nodeList.size(); i++) {
			if (nodeList[i].inNum == nodeList[i].outNum) {
				normalNum++;
			}
			else if (nodeList[i].inNum - nodeList[i].outNum == 1) {
				endNum++;
				endNode = i;
			}
			else if (nodeList[i].inNum - nodeList[i].outNum == -1) {
				startNum++;
				startNode = i;
			}
			else {
				return false;
			}
		}
		//cout << "normalNum = " << normalNum << "  startNum = " << startNum << "  endNum = " << endNum << endl;

		if ((startNum == 1 && endNum == 1) || (startNum == 0 && endNum == 0) ) {
			//cout << "has EP" << endl;
			return true;
		}
		return false;
	}

	void findEP() {
		//this will build the stack.
		if (startNode == -1)
			startNode = 0;
		int currentNodeIndex = startNode;
		int nextNodeIndex = -1;
		//cout << "Start form " << nodeList[startNode].name << endl;

		while (1) {
			current.push(currentNodeIndex);
			//cout << "Push " << nodeList[currentNodeIndex].name << " into current." << endl;
			if (nodeList[currentNodeIndex].outNum > 0) {
				nodeList[currentNodeIndex].outNum--;
				nextNodeIndex = nodeList[currentNodeIndex].outEdges[0];
				nodeList[currentNodeIndex].outEdges.erase(nodeList[currentNodeIndex].outEdges.begin());
				currentNodeIndex = nextNodeIndex;
			}
			else {
				complete.push(current.top());
				current.pop();
				if (current.size()==0) {
					//cout << "Find EP complete." << endl;
					return;
				}

				currentNodeIndex = current.top();
				current.pop();
			}
		}

	}

	void outputPath() {
		//output the path by using Stack complete.
		int nameLengh = nodeList[complete.top()].name.length();
		cout << nodeList[complete.top()].name;
		complete.pop();
		int size = complete.size();
		for (int i = 0; i < size; i++) {
			cout << nodeList[complete.top()].name[nameLengh - 1];
			complete.pop();
		}
	}
	
	void showNode() {
		//testing function. gives the graph information of each nodes.
		for (int i = 0; i < nodeList.size(); i++) {
			cout << nodeList[i].name << nodeList[i].outNum << " - ";
			for (int j = 0; j < nodeList[i].outEdges.size(); j++) {
				cout << nodeList[nodeList[i].outEdges[j]].name << " ";
			}
			cout << endl;
		}
	}
};


int main(int argc, char*args[]) {
	Graph test;
	test.bulid();
	//test.showNode();
	if (test.isConnected()) {
		//cout << "DFS" << endl;
		if (test.hasEP()) {
			test.findEP();
			//cout << "findEP" << endl;
			test.outputPath();
			//cout << "output" << endl;
		}
	}

	system("pause");

	return 0;
}