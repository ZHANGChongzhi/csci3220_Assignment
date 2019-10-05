#include<iostream>
#include<vector>
/*
CSCI3220 2018-19 First Term Assignment 5
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

using namespace std;
class Heap {
private:
	vector<int> list;//keeps the number.

private:
	void reOrderTheList() {
		for (int i = list.size() - 1; i >= 0; i--) {
			makeParent(i);
		}
	}// make the list in a heap order.

	void swap(int indexX, int indexY) {
		int tmp = list[indexX];
		list[indexX] = list[indexY];
		list[indexY] = tmp;
	}//swap the value of list[indexX] and list[indexY]

	void makeParent(int index) {
		int child1 = 2 * index + 1;// index of first child(if had)
		int child2 = 2 * index + 2;
		int heapSize = list.size();

		if (child1 > heapSize - 1) {// this is a leaf node.
			return;
		}
		else if (child1 == heapSize - 1) {// only has one child
			if (list[index] > list[child1]) {
				swap(index, child1);
			}
			return;
		}
		else {// has two children
			int minChild = child1;
			if (list[child2] < list[child1]) {
				minChild = child2;
			}// make minchild is the smaller one index of two children.
			if (list[index] > list[minChild]) {
				swap(index, minChild);
			}
			makeParent(minChild);//Recursion check the list[minChild].
			return;
		}
	}//check if list[index] is less than his two children. if not, make it right.

public:
	void inputList() {
		int inputSize, inputNum;
		cin >> inputSize;
		for (int i = 0; i < inputSize; i++) {
			cin >> inputNum;
			list.push_back(inputNum);
		}
		reOrderTheList();// Reorder the list.
	}//input several integers.

	int getSize() {
		return list.size();
	}

	int getMin() {
		return list[0];
	}//return the Min value.

	void deleteMin() {
		swap(0, list.size() - 1);//swap the last and first.
		list.pop_back();//delete the last
		makeParent(0);//Reorder.
	}//delete the Min value.

	void printHeap() {//print the heap.
		for (int i = 0; i < list.size(); i++) {
			if (i == 0)cout << list[i];
			else cout << "," << list[i];
		}
		cout << endl;
	}
};

int main(int* argc, char * args[]) {
	Heap h;
	h.inputList();
	h.printHeap();
	int size = h.getSize();
	for (int i = 0; i < size; i++) {
		h.deleteMin();
		h.printHeap();
	}
	return 0;
}