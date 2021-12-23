/* " AI project "
Hitori is played with a grid of squares or cells, with each cell initially containing a number. 
The game is played by eliminating squares/numbers and this is done by blacking them out. The objective is to transform the grid to a state wherein all three following rules are true:

1- no row or column can have more than one occurrence of any given number
2- black cells cannot be adjacent, although they can be diagonal to one another.
3- the remaining numbered cells must be all connected to each other, horizontally or vertically.


2019 - December

*/
#include <windows.h>
#include <stack>
#include<iostream>
#include<vector>
#include<fstream>
#include<utility>
#include <queue>
#include <time.h>
using namespace std;
int number_of_size = 0;
int expandingA = 0;
int expandingHC = 0;
int expandingRHC = 0;
int expandingSA = 0;
int expandingGreedy = 0;

class State {
private:
	int Size;
	int** board;
	int** colorboard; // 0- colorless      1-white      2-black 
	int h;
	int h1;
	friend class Hitori;
public:
	void Read(string);
	void print();
	void Successor(Hitori*, State*, int);
	bool Is_goal();
	int Check(int, int, State*);
	bool Surrounding(State*, int, int);
	void Gredy(Hitori *);
	void Hill_climbing(Hitori *, State*);
	void Random_Hill_climbing(Hitori *temp, State*current_state);
	void SA(Hitori *);
	void Astar(Hitori *);
	int Find_numer_of_same();

	//----------Constructor----------
	State(const State& other) {
		if (this != &other)
			*this = other;
	}
	State() {
		h = 0;
		h1 = -1;
		Size = 0;
	}



	//--------OPERATOR----------
	State& operator = (const State& other) {

		this->Size = other.Size;
		this->h = other.h;
		this->h1 = other.h1;
		this->board = new int*[other.Size];
		this->colorboard = new int*[other.Size];
		for (int i = 0; i < other.Size; i++) {
			this->board[i] = new int[other.Size];
			this->colorboard[i] = new int[other.Size];
		}

		for (int i = 0; i<other.Size; i++)
			for (int j = 0; j < other.Size; j++) {
				this->board[i][j] = other.board[i][j];
				this->colorboard[i][j] = other.colorboard[i][j];
			}
		return *this;
	}
	bool operator == (const State& other) {
		for (int i = 0; i< Size; i++)
			for (int j = 0; j <Size; j++)
				if (this->board[i][j] != other.board[i][j] || this->colorboard[i][j] != other.colorboard[i][j])
					return false;
		return true;
	}
	bool operator < (const State& other) {
		if (this->h < other.h)
			return true;
		return false;
	}
	bool operator > (const State& other) {
		if (this->h > other.h)
			return true;
		return false;
	}
	bool operator >= (const State& other) {
		if (this->h >= other.h)
			return true;
		return false;
	}
	bool operator <= (const State& other) {
		if (this->h <= other.h)
			return true;
		return false;
	}
};

struct Hitori {
	friend class State;
private:
	vector <State> vec_list;
	struct Compare {
		bool operator() (State left, State right) { return  left.h < right.h; }
	};
	priority_queue < State, vector<State>, Compare> _queue;
	friend class State;
public:
	int Random();
	void add(State* temp, int num) // num==2  hill ciliming & ...           num==1 for gredy & A*
	{
		if (num == 2) {
			this->vec_list.push_back(*temp);
		}
		if (num == 1)
			this->_queue.push(*temp);
	}
};

bool State::Surrounding(State* current, int x, int y)
{



	int number = 0;
	int up = 0;
	int down = 0;
	int right = 0;
	int left = 0;
	pair <int, int> *temp;
	temp = new pair <int, int>[8];
	for (int i = 0; i < 8; i++) {
		temp[i].first = -1;
		temp[i].second = -1;
	}
	if (x + 1 < current->Size)
		if (current->colorboard[x + 1][y] == 2)
			return false;
	if (x - 1 >= 0)
		if (current->colorboard[x - 1][y] == 2)
			return false;
	if (y - 1 >= 0)
		if (current->colorboard[x][y - 1] == 2)
			return false;
	if (y + 1 < current->Size)
		if (current->colorboard[x][y + 1] == 2)
			return false;
	//element existance checking
	if (x - 1 >= 0 && x - 1 < this->Size && y + 1 >= 0 && y + 1 < this->Size) {// 0
		temp[number].first = x - 1;
		temp[number].second = y + 1;
		down++;
		left++;
	}
	number++;
	if (x - 1 >= 0 && x - 1 < this->Size && y - 1 >= 0 && y - 1 < this->Size) { //1
		temp[number].first = x - 1;
		temp[number].second = y - 1;
		up++;
		left++;
	}
	number++;
	if (x + 1 >= 0 && x + 1 < this->Size && y + 1 >= 0 && y + 1 < this->Size) {//2
		temp[number].first = x + 1;
		temp[number].second = y + 1;
		right++;
		down++;

	}
	number++;
	if (x + 1 >= 0 && x + 1 < this->Size && y - 1 >= 0 && y - 1 < this->Size) {//3
		temp[number].first = x + 1;
		temp[number].second = y - 1;
		right++;
		up++;

	}
	number++;
	if (x - 2 >= 0 && x - 2 < this->Size && y >= 0 && y < this->Size) {//4
		temp[number].first = x - 2;
		temp[number].second = y;
		left++;
	}
	number++;
	if (x + 2 >= 0 && x + 2 < this->Size && y >= 0 && y < this->Size) {//5
		temp[number].first = x + 2;
		temp[number].second = y;
		right++;

	}
	number++;
	if (x >= 0 && x < this->Size && y + 2 >= 0 && y + 2 < this->Size) {//6
		temp[number].first = x;
		temp[number].second = y + 2;
		down++;
	}
	number++;
	if (x >= 0 && x < this->Size && y - 2 >= 0 && y - 2 < this->Size) {//7
		temp[number].first = x;
		temp[number].second = y - 2;
		up++;
	}
	number++;

	//elements in upside of current
	if (up > 2) { //when we have 3 elements 
		if (current->colorboard[temp[1].first][temp[1].second] == 2 &&
			current->colorboard[temp[7].first][temp[7].second] == 2 &&
			current->colorboard[temp[3].first][temp[3].second] == 2)
			return false;
	}

	if (up == 2) { //when we have 2 elements
		if (temp[3].first == -1)
			if (current->colorboard[temp[1].first][temp[1].second] == 2 &&
				current->colorboard[temp[7].first][temp[7].second] == 2)
				return false;
		if (temp[7].first == -1)
			if (current->colorboard[temp[1].first][temp[1].second] == 2 &&
				current->colorboard[temp[3].first][temp[3].second] == 2)
				return false;
		if (temp[1].first == -1)
			if (current->colorboard[temp[3].first][temp[3].second] == 2 &&
				current->colorboard[temp[7].first][temp[7].second] == 2)
				return false;
	}
	if (up == 1) { // when we have an element
		if (temp[3].first == -1 && temp[7].first == -1)
			if (current->colorboard[temp[1].first][temp[1].second] == 2)
				return false;
		if (temp[1].first == -1 && temp[7].first == -1)
			if (current->colorboard[temp[3].first][temp[3].second] == 2)
				return false;
	}

	//elements in leftside of current
	if (left > 2) { //when we have 3 elements 
		if (current->colorboard[temp[0].first][temp[0].second] == 2 &&
			current->colorboard[temp[4].first][temp[4].second] == 2 &&
			current->colorboard[temp[1].first][temp[1].second] == 2)
			return false;
	}
	if (left == 2) {//when we have 2 elements
		if (temp[1].first == -1)
			if (current->colorboard[temp[4].first][temp[4].second] == 2 &&
				current->colorboard[temp[0].first][temp[0].second] == 2)
				return false;
		if (temp[0].first == -1)
			if (current->colorboard[temp[4].first][temp[4].second] == 2 &&
				current->colorboard[temp[1].first][temp[1].second] == 2)
				return false;
		if (temp[4].first == -1)
			if (current->colorboard[temp[0].first][temp[0].second] == 2 &&
				current->colorboard[temp[1].first][temp[1].second] == 2)
				return false;
	}
	if (left == 1) {// when we have an element
		if (temp[1].first == -1 && temp[4].first == -1)
			if (current->colorboard[temp[0].first][temp[1].second] == 2)
				return false;
		if (temp[0].first == -1 && temp[4].first == -1)
			if (current->colorboard[temp[1].first][temp[3].second] == 2)
				return false;
	}

	//elements in rightside of current
	if (right > 2) {//when we have 3 elements 
		if (current->colorboard[temp[3].first][temp[3].second] == 2 &&
			current->colorboard[temp[5].first][temp[5].second] == 2 &&
			current->colorboard[temp[2].first][temp[2].second] == 2)
			return false;
	}
	if (right == 2) {//when we have 2 elements
		if (temp[5].first == -1)
			if (current->colorboard[temp[3].first][temp[3].second] == 2 &&
				current->colorboard[temp[2].first][temp[2].second] == 2)
				return false;
		if (temp[3].first == -1)
			if (current->colorboard[temp[5].first][temp[5].second] == 2 &&
				current->colorboard[temp[2].first][temp[2].second] == 2)
				return false;
		if (temp[2].first == -1)
			if (current->colorboard[temp[3].first][temp[3].second] == 2 &&
				current->colorboard[temp[5].first][temp[5].second] == 2)
				return false;
	}
	if (right == 1) {// when we have an element
		if (temp[3].first == -1 && temp[5].first == -1)
			if (current->colorboard[temp[2].first][temp[2].second] == 2)
				return false;
		if (temp[2].first == -1 && temp[5].first == -1)
			if (current->colorboard[temp[3].first][temp[3].second] == 2)
				return false;
	}

	//elements in downside of current
	if (down > 2) {//when we have 3 elements 
		if (current->colorboard[temp[2].first][temp[2].second] == 2 &&
			current->colorboard[temp[6].first][temp[6].second] == 2 &&
			current->colorboard[temp[0].first][temp[0].second] == 2)
			return false;

	}
	if (down == 2) {//when we have 2 elements
		if (temp[2].first == -1)
			if (current->colorboard[temp[0].first][temp[0].second] == 2 &&
				current->colorboard[temp[6].first][temp[6].second] == 2)
				return false;
		if (temp[0].first == -1)
			if (current->colorboard[temp[6].first][temp[6].second] == 2 &&
				current->colorboard[temp[2].first][temp[2].second] == 2)
				return false;
		if (temp[6].first == -1) {

			if (current->colorboard[temp[2].first][temp[2].second] == 2 &&
				current->colorboard[temp[0].first][temp[0].second] == 2)
				return false;
		}
	}
	if (down == 1) {// when we have an element
					//cout << "x :" << x << "  Y: " << y << "\t" << current->board[x][y] << endl;
		if (temp[0].first == -1 && temp[6].first == -1)
			if (current->colorboard[temp[2].first][temp[2].second] == 2)
				return false;
		if (temp[2].first == -1 && temp[6].first == -1)
			if (current->colorboard[temp[0].first][temp[0].second] == 2)
				return false;
	}

	return true;
}

bool State::Is_goal()
{
	for (int i = 0; i < Size; i++)
		for (int j = 0; j < Size; j++) {
			if (colorboard[i][j] != 2)
				for (int k = 0; k < Size; k++) {
					if (board[i][j] == board[i][k] && colorboard[i][k] != 2 && j != k)
						return false;
					if (board[i][j] == board[k][j] && colorboard[k][j] != 2 && i != k)
						return false;
				}

		}
	return true;
}
int State::Check(int x, int y, State*current) // dashtane tekrari
{
	int n = 0;
	int temp = board[x][y];
	for (int i = 0; i < Size; i++) {
		if (board[i][y] == temp && colorboard[i][y] != 2 && x != i) {
			n++;
		}
		if (board[x][i] == temp && colorboard[x][i] != 2 && y != i) {
			n++;
		}
	}
	return n;
}
void State::Hill_climbing(Hitori *temp, State*current_state)
{
	expandingHC++;
	Successor(temp, current_state, 2);
	if (temp->vec_list.size() > number_of_size)
		number_of_size = temp->vec_list.size();
	if (temp->vec_list.size() == 0) {
		current_state->print();
		cout << "win!!!\n";
		//cout << "size number : " << number_of_size<<"\n"<<"number of successor: " << expandingHC<<"\n";
		return;
	}
	State temp_s;
	if (Is_goal()) {
		current_state->print();
		//	cout << "size number : " << number_of_size << "\n" << "number of successor: " << expandingHC << "\n";
		cout << "win!!!\n";
		return;
	}
	if (temp->vec_list.size() != 0)
		temp_s = temp->vec_list[0];
	for (int i = 0; i<temp->vec_list.size(); i++) {// empty !!
		if (temp_s.h <= temp->vec_list[i].h) {
			temp_s = temp->vec_list[i];
			if (Is_goal()) {
				current_state->print();
				cout << "\nwin!!!\n";
				//cout << "size number : " << number_of_size << "\n" << "number of successor: " << expandingHC << "\n";

				return;
			}
		}
	}
	temp->vec_list.clear();
	Hill_climbing(temp, &temp_s);

}

void State::Random_Hill_climbing(Hitori *temp, State*current_state)
{

	if (Is_goal()) {

		cout << "win!!!\n";
		return;
	}
	State temp_s;
	expandingRHC++;
	Successor(temp, current_state, 2);
	if (temp->vec_list.size() > number_of_size)
		number_of_size = temp->vec_list.size();
	if (temp->vec_list.size() == 0) {
		current_state->print();
		cout << "win!!!\n";
		//cout << "size number : " << number_of_size << "\n" << "number of successor: " << expandingRHC << "\n";
		return;
	}
	if (temp->vec_list.size() != 0) {
		temp_s = temp->vec_list[temp->Random()];
		temp->vec_list.clear();
		Random_Hill_climbing(temp, &temp_s);
	}
	return;
}
void State::SA(Hitori *temp)
{
	static double T = 1;
	State * current;
	State * next;
	double r;
	current = this;
	double E = 0;
	expandingSA++;
	Successor(temp, current, 2);
	if (temp->vec_list.size() > number_of_size)
		number_of_size = temp->vec_list.size();
	static double Tmin = .0001;
	static double alpha = 0.9;// Decrease in temperature 
	while (T > Tmin) {
		//current->print();
		if (current->Is_goal()) {
			current->print();
			//	cout << "size number : " << number_of_size << "\n" << "number of successor: " << expandingSA << "\n";
			cout << "win!!!\n";
			return;
		}
		if (temp->vec_list.size() == 0)
			return;
		int RandIndex = rand() % temp->vec_list.size();

		next = &temp->vec_list[RandIndex];
		E = next->h1 - current->h;
		if (E > 0)
			current = next;
		else {
			double probability = exp(E / T);
			r = ((double)rand() / (RAND_MAX));
			if (probability > r)
				current = next;
		}

		expandingSA++;
		temp->vec_list.clear();
		Successor(temp, current, 2);
		//current->print();
		if (temp->vec_list.size() == 0) {
			current->print();
			//cout << "size number : " << number_of_size << "\n" << "number of successor: " << expandingSA << "\n";
			cout << "win!!!\n";
			return;
		}

		T *= alpha; // Decreases T
	}

}
void State::Astar(Hitori *temp)
{
	State  top;
	this->h++;
	expandingA++;
	Successor(temp, this, 1);
	if (temp->_queue.size() > number_of_size)
		number_of_size = temp->_queue.size();
	while (!temp->_queue.empty()) {
		top = temp->_queue.top();
		if (top.Is_goal()) {
			top.print();
			//cout << "size number : " << number_of_size << "\n" << "number of successor: " << expandingA << "\n";
			cout << "win!!!\n";
			return;
		}
		top.h++;
		expandingA++;
		//	cout << "size number : " << number_of_size << "\n" << "number of successor: " << expandingA << "\n";
		Successor(temp, &top, 1);
		if (temp->_queue.size() > number_of_size)
			number_of_size = temp->_queue.size();
		temp->_queue.pop();
	}
}
int State::Find_numer_of_same()
{
	int sum = 0;
	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size; j++) {
			int n = 0;
			int temp = board[i][j];
			for (int k = 0; k < Size; k++) {
				if (board[k][j] == temp && colorboard[k][j] != 2 && i != k) {
					n++;
				}
				if (board[i][k] == temp && colorboard[i][k] != 2 && j != k) {
					n++;
				}
			}
			sum += n;
		}
	}
	return sum;
}
int Hitori::Random()
{
	srand(time(NULL));
	vector<float> numbers;
	int n;
	int m;
	int min = 0;
	int min_index = 0;
	int sum = 0;
	int negetive_m = 0;
	for (int i = 0; i < this->vec_list.size(); i++) {
		sum += this->vec_list[i].h;
	}
	for (int i = 0; i < this->vec_list.size(); i++) {
		numbers.push_back(this->vec_list[i].h*sum);
	}
	n = rand() % (sum*sum);
	for (int i = 0; i < numbers.size(); i++) {
		m = numbers[i] - n;
		if (m >= 0)
		{
			if (min > m) {
				min = m;
				min_index = i;
			}
		}
		else {
			if (min > m) {
				min = m;
				min_index = i;
			}
		}
	}

	return min_index;
}
void State::Successor(Hitori*_Hitori, State*current, int num)
{
	State * S_temp = NULL;
	for (int i = 0; i < current->Size; i++) {
		for (int j = 0; j < current->Size; j++) {
			if (current->colorboard[i][j] == 0) {
				if (Check(i, j, current) > 0) { //      tekrari dashte bashe dar satr va soton

					int temp = current->board[i][j];
					if (Surrounding(current, i, j)) {// sharayet siah shodan dashte bashe

						if (i == 0 && j == 0) { //corner 1  (left_up)

							if (current->board[i + 1][j] == temp && current->board[i][j + 1] == temp) {
								S_temp = new State(*current);
								S_temp->colorboard[i][j] = 2;
								S_temp->h++;
								S_temp->h1 = S_temp->Find_numer_of_same();
								if (num == 2)
									_Hitori->add(S_temp, 2);
								else
									_Hitori->add(S_temp, 1);
							}
							else {
								S_temp = new State(*current);
								S_temp->colorboard[i][j] = 2;
								S_temp->h++;
								S_temp->h1 = S_temp->Find_numer_of_same();
								if (num == 2)
									_Hitori->add(S_temp, 2);
								else
									_Hitori->add(S_temp, 1);
							}
						}
						else if (i == current->Size - 1 && j == 0) { //corner 2 (right_up)
							if (current->board[i - 1][j] == temp && current->board[i][j + 1] == temp) {
								S_temp = new State(*current);
								S_temp->colorboard[i][j] = 2;
								S_temp->h++;
								S_temp->h1 = S_temp->Find_numer_of_same();
								if (num == 2)
									_Hitori->add(S_temp, 2);
								else
									_Hitori->add(S_temp, 1);

							}
							else {
								S_temp = new State(*current);
								S_temp->colorboard[i][j] = 2;
								S_temp->h++;
								S_temp->h1 = S_temp->Find_numer_of_same();
								if (num == 2)
									_Hitori->add(S_temp, 2);
								else
									_Hitori->add(S_temp, 1);
							}
						}
						if (i == current->Size - 1 && j == current->Size - 1) { //corner 3 (right_down)
							if (current->board[i - 1][j] == temp && current->board[i][j - 1] == temp) {
								S_temp = new State(*current);
								S_temp->colorboard[i][j] = 2;
								S_temp->h++;
								S_temp->h1 = S_temp->Find_numer_of_same();
								if (num == 2)
									_Hitori->add(S_temp, 2);
								else
									_Hitori->add(S_temp, 1);
							}
							else {
								S_temp = new State(*current);
								S_temp->colorboard[i][j] = 2;
								S_temp->h++;
								S_temp->h1 = S_temp->Find_numer_of_same();
								if (num == 2)
									_Hitori->add(S_temp, 2);
								else
									_Hitori->add(S_temp, 1);
							}
						}
						if (i == 0 && j == current->Size - 1) { //corner 4   (left_down)
							if (current->board[i + 1][j] == temp && current->board[i][j - 1] == temp) {
								S_temp = new State(*current);
								S_temp->colorboard[i][j] = 2;
								S_temp->h++;
								S_temp->h1 = S_temp->Find_numer_of_same();
								if (num == 2)
									_Hitori->add(S_temp, 2);
								else
									_Hitori->add(S_temp, 1);
							}
							else {
								S_temp = new State(*current);
								S_temp->colorboard[i][j] = 2;
								S_temp->h++;
								h1 = S_temp->Find_numer_of_same();
								if (num == 2)
									_Hitori->add(S_temp, 2);
								else
									_Hitori->add(S_temp, 1);
							}
						}
						if (i - 1 >= 0 && i + 1 < current->Size) {
							if ((current->board[i - 1][j] == current->board[i + 1][j])) { // A S A      s nabyad packshe
								S_temp = new State(*current);
								S_temp->colorboard[i][j] = 1;
								S_temp->h++;
								S_temp->h1 = S_temp->Find_numer_of_same();
								if (num == 2)
									_Hitori->add(S_temp, 2);
								else
									_Hitori->add(S_temp, 1);
							}
						}
						if (j - 1 >= 0 && j + 1 < current->Size) {
							if ((current->board[i][j - 1] == current->board[i][j + 1])) { // A S A      s nabyad packshe
								S_temp = new State(*current);
								S_temp->colorboard[i][j] = 1;
								S_temp->h++;
								S_temp->h1 = S_temp->Find_numer_of_same();
								if (num == 2)
									_Hitori->add(S_temp, 2);
								else
									_Hitori->add(S_temp, 1);
							}
						}
						if (((i - 1 >= 0 && i + 1 < current->Size) && (j - 1 >= 0 && j + 1 < current->Size))) {
							if ((current->board[i][j - 1] == current->board[i][j + 1]) || (current->board[i - 1][j] == current->board[i + 1][j])) {
								S_temp = new State(*current);
								S_temp->colorboard[i][j] = 1;
								S_temp->h++;
								S_temp->h1 = S_temp->Find_numer_of_same();
								if (num == 2)
									_Hitori->add(S_temp, 2);
								else
									_Hitori->add(S_temp, 1);
							}
						}
						else {

							S_temp = new State(*current);
							S_temp->colorboard[i][j] = 2;
							S_temp->h++;
							S_temp->h1 = S_temp->Find_numer_of_same();
							if (num == 2)
								_Hitori->add(S_temp, 2);
							else
								_Hitori->add(S_temp, 1);
						}

					}

				}
				else {
					S_temp = new State(*current);
					S_temp->colorboard[i][j] = 1;
					S_temp->h1 = S_temp->Find_numer_of_same();
					if (num == 2)
						_Hitori->add(S_temp, 2);
					else
						_Hitori->add(S_temp, 1);
				}
			}
		}
	}
}
void State::Read(string Address)
{
	fstream file;
	file.open(Address);
	if (!file) {
		cout << "file khondeh nashoedeh!!";
		return;
	}
	file >> Size;
	this->board = new int*[Size];
	this->colorboard = new int*[Size];
	for (int i = 0; i < Size; i++) {
		this->board[i] = new int[Size];
		this->colorboard[i] = new int[Size];
	}
	int temp;
	for (int i = 0; i <this->Size; i++) {
		for (int j = 0; j < this->Size; j++) {
			file >> temp;
			this->board[j][i] = temp;
			this->colorboard[j][i] = 0;
		}
	}

}
void State::print()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int i = 0; i < this->Size; i++) {
		for (int j = 0; j < this->Size; j++) {
			if (this->colorboard[j][i] == 2)
			{

				SetConsoleTextAttribute(hConsole, 7);
				cout << " " << this->board[j][i] << " ";
			}
			else {
				SetConsoleTextAttribute(hConsole, 240);
				cout << " " << this->board[j][i] << " ";

			}
		}
		SetConsoleTextAttribute(hConsole, 240);
		cout << "\n";
	}
	cout << endl;
	cout << "\n";
}

void State::Gredy(Hitori *temp)
{
	State  top;
	expandingGreedy++;
	Successor(temp, this, 1);
	if (temp->_queue.size() > number_of_size)
		number_of_size = temp->_queue.size();
	while (!temp->_queue.empty()) {

		top = temp->_queue.top();

		if (top.Is_goal()) {
			top.print();
			//cout << "size number : " << number_of_size << "\n" << "number of successor: " << expandingGreedy << "\n";
			cout << "win!!!\n";
			return;
		}
		expandingGreedy++;
		Successor(temp, &top, 1);
		if (temp->_queue.size() > number_of_size)
			number_of_size = temp->_queue.size();
		temp->_queue.pop();

	}
}
int main() {
	int n;

	Hitori _Hitori;
	State _State;
	_State.Read("input.txt");
	//clock_t time = clock();


	//_State.Gredy(&_Hitori);
	//_State.Hill_climbing(&_Hitori, &_State);
	//_State.Random_Hill_climbing(&_Hitori, &_State);
	//_State.Astar(&_Hitori);
	//_State.SA(&_Hitori);
	//time = clock() - time;
	//cout << endl << ("Time taken: ", (float)(time) / CLOCKS_PER_SEC) << endl;

	cin >> n;
	return 0;
}

