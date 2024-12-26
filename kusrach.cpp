#include <iostream>
#include <fstream>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <string>
#include <filesystem>
#include <limits>
using namespace std;
using namespace filesystem;

int isfilegood(string path);

class Set {
private:
	int setsize;
	int fv;

public:
	char* data;
	Set() {};

	Set(int size, int firstvalue = 0) {
		setsize = size;
		fv = firstvalue;
		data = new char[size];
		for (int i = 0; i < size; i++) data[i] = firstvalue;
	};

	bool add(int newdata) {
		if (newdata <= setsize && newdata >= 0 && data[newdata] == false) {
			data[newdata] = 1;
			return true;
		}
		else return false;

	}

	bool all() {
		for (int i = 0; i < setsize; i++) if (data[i] == fv) return false;
	}

	int firstfalse() {
		for (int i = 0; i < setsize; i++) if (data[i] == fv) return i;
		return fv - 1;
	}

	int fisrsttrue() {
		for (int i = 0; i < setsize; i++) if (data[i] != fv) return i;
		return fv - 1;
	}

	void print() {
		for (int i = 0; i < setsize; i++) {
			cout << (int)data[i] << " " << ends;
		}
	}

	int setmin() {
		char out = 100;
		for (int i = 0; i < setsize; i++) {
			if (data[i] < out && data[i] > 0) out = data[i];
		}
		return out;
	}

	int setmax() {
		char out = -1;
		for (int i = 0; i < setsize; i++) {
			out = max(out, data[i]);
		}
		return out;
	}
};

class List {
public:
	int data;
	List* next;
	char color;

	List() {
		data = -1;
		next = nullptr;
		color = 0;
	};

	List(int value) : data(value), next(nullptr), color(0) {};

	~List() {
		delete next;
		next = nullptr;
//		cout << "Deleting node at: " << this << "(" << data << ")" << std::endl;
	};

	void operator=(List *other) {
		List* temp = other;
		while (temp) {
			add(temp->data);
			temp = temp->next;
		}
	}

	void add(int value) {
		if (data == -1) {
			data = value;
			return;
		}
		if (next == nullptr) {
			next = new List(value);
		}
		else next->add(value);
	}

	void addsort(int value) {
		if (data == -1) {
			data = value;
			return;
		}
		else if (value > data) {
			List* temp = next;
			next = new List(data);
			next->next = temp;
			data = value;
		}
		else if (next == nullptr) {
			next = new List(value);
		}
		else next->addsort(value);
	}

	void addset(int value) {
		if (data == value) return;
		if (data == -1) {
			data = value;
			return;
		}
		if (next == nullptr) {
			next = new List(value);
		}
		else next->addset(value);
	}

	void print() {
		if (data == -1) {
			cout << "-" << endl;
			return;
		}
		cout << data << " ";
		if (next != nullptr) next->print();
		else {
			cout << endl;
			return;
		}
	};

	void deletee(int elem) {
		if (data == elem) {
			if (next == nullptr) {
				data = -1;
				return;
			}
			data = next->data;
			List* temp = next->next;
			next->next = nullptr;
			delete next;
			next = temp;
			return;
		};
		List* find = this;
		while (find) {
			if (find->next->data == elem) {
				List* temp = find->next->next;
				find->next->next = nullptr;
				delete find->next;
				find->next = temp;
				return;
			}
			find = find->next;
		}
		return;
	}

	bool isin(int value) {
		if (data == value) return true;
		else if (next == nullptr) return false;
		else next->isin(value);
	}

	int size(int out = 0) {
		if (data == -1) return 0;
		if (next == nullptr) return (++out);
		else next->size(++out);
	}


};

class Graf {
private:

	List q;

	int nextcolor(int el, char* maxcolor) {
		List* temp = &list[el];
		Set color(*maxcolor);

		while (temp) {
			color.add(list[temp->data - 1].color - 1);
			temp = temp->next;
		}

		int out = color.firstfalse();
		if (out == -1) {
			return ++ * maxcolor;
		}
		else return ++out;

	}

	/*bool iscolored() {
		for (int i = 0; i < size; i++) {
			if (list[i].color == 0) return false;
		}
		return true;
	}*/

public:
	unsigned char size;
	bool isvzvesh;
	bool isorient;
	bool iscolored;
	bool isrendered;
	char** mass;
	List* list;
	char maxcolor;

	Graf() {
		isvzvesh = false;
		isorient = false;
		iscolored = false;
		isrendered = false;
		maxcolor = 0;
	};

	Graf(int yoursize) : size(yoursize) {
		mass = new char* [size];
		for (int i = 0; i < size; i++) {
			mass[i] = new char[size];
		}
		isvzvesh = false;
		isorient = false;
		iscolored = false;
		isrendered = false;
		maxcolor = 0;
	}

	~Graf(){
		for (int i = 0; i < size; i++) {
			delete[] mass[i];
		}
		delete mass ;
		delete[] list;
	}

	void init(bool fill = true) {
		mass = new char* [size];
		for (int i = 0; i < size; i++) {
			mass[i] = new char[size];
		}
		list = new List[size];
		if (fill){
			for (int i = 0; i < size; i++) {
				for (int j = i; j < size; j++) {
					if (i == j) mass[i][j] = 0;
					else mass[i][j] = mass[j][i] = rand() % 2;
					if (mass[i][j] != 0) {
						if (isvzvesh) mass[i][j] = mass[j][i] = (rand() % 10);
						if (isorient) {
							mass[i][j] *= (pow(-1, rand() % 2));
							mass[j][i] = mass[i][j] * -1;
						}
						list[i].add(j + 1);
						list[j].add(i + 1);
					}
				}
				mass[i][i] = list[i].size();
			}
		}

	}

	void print(bool ishide = true) {
		cout << endl;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (i == j && ishide) cout << "\033[90m#\033[0m ";
				else cout << (int)mass[i][j] << " ";
			}
			cout << endl;
		}
	}

	void colorit() {
		if (iscolored) {
			cout << "Этот граф уже был расскаршен!" << endl;
			return;
		}
		List sort;
		for (int i = 0; i < size; i++) {
			sort.addsort(mass[i][i]);
		}
		List* temp = &sort;
		while (temp) {
			for (int i = 0; i < size; i++) {
				if (list[i].size() == temp->data && !q.isin(i)) {
					q.add(i);
					break;
				}
			}
			temp = temp->next;
		}

		char r = 1;
		temp = &q;

		while (temp) {
			if (list[temp->data].size() > 0) list[temp->data].color = nextcolor(temp->data, &r);
			temp = temp->next;
		}

		cout << "\nУдалось покрасить граф с помощью " << (int)r << " цветов." << endl;
		maxcolor = r;
		iscolored = true;

	};

	void printlist() {
		cout << endl;
		for (int i = 0; i < size; i++) {
			if (list[i].color == 0) cout << i + 1 << ": "; else
				if (maxcolor <= 7) cout << "\033[3" << (int)list[i].color << "m" << i + 1 << "\033[0m: ";
				else cout << "\033[38;5;" << (int)list[i].color * 8 << "m" << i + 1 << "\033[0m: ";
			list[i].print();
		}
	};

	class Render {
	public:

		Graf* graf;

	private: 
		int sizex;
		int sizey;
		unsigned char** field;
		unsigned char curx, cury, nexx, nexy;

		void nextpos(unsigned char pos) {
			pos = pos % 6;
			switch (pos) {
			case 0: {
				nexx = curx - 1;
				nexy = cury - 1;
				return;
			}
			case 1: {
				nexx = curx + 1;
				nexy = cury - 1;
				return;
			}
			case 2: {
				nexx = curx + 2;
				nexy = cury;
				return;
			}
			case 3: {
				nexx = curx + 1;
				nexy = cury + 1;
				return;
			}
			case 4: {
				nexx = curx - 1;
				nexy = cury + 1;
				return;
			}
			case 5: {
				nexx = curx - 2;
				nexy = cury;
				return;
			}

			}
		}

		bool tryplace(int element) {
			if (field[cury][curx] != ' ') return false;
			for (int i = 0; i < 6; i++) {
				nextpos(i);
				if (field[nexy][nexx] != ' ') {
					if (isdigit(field[nexy][nexx]) && !graf->list[element].isin(field[nexy][nexx] - '0')) return false;
					if (isalpha(field[nexy][nexx]) && !graf->list[element].isin(field[nexy][nexx] - '@')) return false;
				}
			}
			return true;
		}

		bool canyoplace(int element) {
			if (element == 254) return false;
			int tempy = cury;
			int tempx = curx;
			for (int i = 0; i < 6; i++) {
				nextpos(i);
				cury = nexy;
				curx = nexx;
				if (tryplace(element)) {
					cury = tempy;
					curx = tempx;
					return true;
				}
				cury = tempy;
				curx = tempx;
			}
			return false;
		}

		void find(int elem, bool findalpha = false) {
			for (int i = 0; i < sizey; i++) {
				for (int j = 0; j < sizex; j++) {
					if ((!findalpha && field[i][j] == elem + '1') || (findalpha && field[i][j] == elem + 'A')) {
						nexx = j;
						nexy = i;
						return;
					}
				}
			}
		}

		void cleanadd(List* out, List* currentlist, Set* n) {
			for (int i = 0; i < 6; i++) {
				nextpos(i);
				if (field[nexy][nexx] != ' ') {
					if (isdigit(field[nexy][nexx]) && currentlist->isin(field[nexy][nexx] - '0'))
						currentlist->deletee(field[nexy][nexx] - '0');
					if (isalpha(field[nexy][nexx]) && currentlist->isin(field[nexy][nexx] - '@'))
						currentlist->deletee(field[nexy][nexx] - '@');
				}
			}
			if (currentlist->data == -1) return;
			List* temp = currentlist;
			while (temp) {
				if (n->data[temp->data - 1] == 0) out->add(temp->data);
				temp = temp->next;
			}
			temp = currentlist;
			while (temp) {
				out->addset(temp->data);
				temp = temp->next;
			}
			return;
		}

		bool wholelinehor(int line) {
			for (int i = 0; i < sizey; i++) {
				if (field[line][i] != ' ') return false;
			}
			return true;
		}

		bool wholelinever(int line) {
			for (int i = 0; i < sizex; i++) {
				if (field[i][line] != ' ') return false;
			}
			return true;
		}

		int movetocorner() {
			int out = 0;
			while (wholelinehor(1)) {
				delete[] field[0];
				for (int i = 0; i < sizey - 1; i++) {
					field[i] = field[i + 1];
					out++;
				}
			}
			while (wholelinever(1)) {
				for (int i = 0; i < sizey; i++) {
					for (int j = 0; j < sizex - 1; j++) {
						field[i][j] = field[i][j+1];
					}
				}
			}
			return out;
		}
		
		void init() {
			field = new unsigned char* [sizey];
			for (int i = 0; i < sizex; i++) {
				field[i] = new unsigned char[sizex];
			}

			for (int i = 0; i < sizey; i++) {
				for (int j = 0; j < sizex; j++) {
					field[i][j] = ' ';
				}
			}
		}

	public:

		void reinit() {
			for (int i = 0; i < sizey; i++) {
				delete[] field[i];
			}
			delete field;

			sizey = 50;
			sizex = 50;

			init();

			curx = nexx = sizex / 2;
			cury = nexy = sizey / 2;
		}

		Render() {
			sizey = 50;
			sizex = 50;

			init();

			curx = nexx = sizex / 2;
			cury = nexy = sizey / 2;
		};

		~Render(){
			for (int i = 0; i < sizey; i++) {
				delete[] field[i];
			}
			delete field;
		}

		void dorender(List* Q, bool isanim = false) {

			List* sort = Q;
			unsigned char elem = sort->data;
			if (field[cury][curx] == ' ') field[cury][curx] = elem + '1';
			Set n(graf->size);
			n.add(elem);

			unsigned char tempx, tempy;
			bool doagain = true;
			bool notalpha = false;
			while (sort) {
				List curlist;
				curlist = &graf->list[elem];
				List templist;
				List* tmpcur = &templist;
				cleanadd(tmpcur, &curlist, &n);
				unsigned char newelem = tmpcur->data - 1;
				tempy = cury;
				tempx = curx;
				char newi = rand() % 6;
				for (int i = newi; i < newi + 6; i++) {
					if (newelem == 254) break;
					nextpos(i);
					cury = nexy;
					curx = nexx;
					if (tryplace(newelem)) {
						if (n.data[newelem] == false) field[cury][curx] = newelem + '1';
						else if (!notalpha) field[cury][curx] = newelem + 'A';
						if (isanim) print(isanim);
						n.add(newelem);
						tmpcur = tmpcur->next;
						if (tmpcur == nullptr) break;
						newelem = tmpcur->data - 1;
						i = newi - 1;
						/*while (tmpcur) {
							List* temp = tmpcur;
							for (int j = 0; j < 6; j++) { <------- ДОДЕЛАТЬ

							}

						}*/
					}
					curx = tempx;
					cury = tempy;
				}
				if (n.data[elem] == 0) {
					if (doagain) {
						List* finde = &graf->list[elem];
						while (finde) {
							if (finde->data == -1) return;
							if (n.data[finde->data - 1] == 1) {
								find(finde->data - 1, true);
								cury = nexy;
								curx = nexx;
								if (canyoplace(elem)) {
									elem = finde->data - 1;
									break;
								}
							}
							finde = finde->next;
						}
						doagain = false;
						notalpha = true;
					}
					else {
						sort = sort->next;
						if (sort) elem = sort->data;
						notalpha = false;
					}
					continue;
				}
				if (elem == sort->data) {
					doagain = true;
					notalpha = false;
					sort = sort->next;
					if (sort) elem = sort->data;
					if (n.data[elem] == 1) {
						find(elem);
						cury = nexy;
						curx = nexx;
					}
					else {
						List* finde = &graf->list[elem];
						while (finde) {
							if (finde->data == -1) return;
							if (n.data[finde->data - 1] == 1) {
								find(finde->data - 1);
								cury = nexy;
								curx = nexx;
								if (canyoplace(elem)) {
									elem = finde->data - 1;
									break;
								}
							}
							finde = finde->next;
						}
					}

				}
				else elem = sort->data; 
			};
			/*if (!n.all()) {
				cout << "AAAAAAAAAAAAAAAAAAAAAAAAA! - " << n.firstfalse() + 1 << endl;
				List smallfind;
				for (int i = 0; i < graf->size; i++) {
					if (n.data[i] == 0) smallfind.add(i);
				}
				List* obhod = &smallfind;
				while (obhod) {
					List* finde = &graf->list[obhod->data];
					while (finde) {
						find(finde->data - 1);
						cury = nexy;
						curx = nexx;
						if (canyoplace(obhod->data)) {
							field[cury][curx] = obhod->data + '1';
							break;
						}
						finde = finde->next;
					}
					obhod = obhod->next;
				}
				print();
				cout << "AAAAAAAAAAAAAAAAAAAAAA" << endl;
			}*/

		}

		void circumcision() {
			int prevsize = sizey - movetocorner();
			int newsizey ;
			int newsizex;
			for (int i = 1; (!wholelinehor(i) || !wholelinehor(i - 1)); i++) {
				newsizey = i + 1;
			}
			for (int i = 1; (!wholelinever(i) || !wholelinever(i - 1)); i++) {
				newsizex = i + 1;
			}


			unsigned char** newfield = new unsigned char* [newsizey];
			for (int i = 0; i < newsizey; i++) {
				newfield[i] = new unsigned char[newsizex];
			}
			for (int i = 0; i < newsizey; i++) {
				for (int j = 0; j < newsizex; j++) {
					newfield[i][j] = field[i][j];
				}
			}
			for (int i = 0; i < prevsize; i++) {
				delete[] field[i];
			}
			delete field;
			field = newfield;
			sizey = newsizey;
			sizex = newsizex;
		}

		void print(bool isanim = false) {
			if (isanim) {
				Sleep(100);
				cout << "\033[50A";
			}
			for (int i = 0; i < sizey; i++) {
				for (int j = 0; j < sizex; j++) {
					if (isdigit(field[i][j]))
						if (graf->maxcolor <= 7)
							cout << "\033[3" << (int)graf->list[field[i][j] - '1'].color << "m" << field[i][j] << "\033[0m";
						else cout << "\033[38;5;" << (int)graf->list[field[i][j] - '1'].color * 8 << "m" << field[i][j] << "\033[0m";
					else
						if (field[i][j] != ' ') cout << field[i][j] - '@';
						else cout << field[i][j];
				}
				cout << "\n";
			}

		}

	};

	Render render;

	void dorender(bool isanim = false) {

		if (isrendered) return;

		render.graf = this;

		render.dorender(&q, isanim);

		render.circumcision();

		//render.print();

		isrendered = true;

	};

	void reinit() {
		delete[] list;
		list = new List[size];
		for (int i = 0; i < size; i++) {
			int sum = 0;
			mass[i][i] = 0;
			for (int j = 0; j < size; j++) {
				sum += mass[i][j];
			}
			mass[i][i] = sum;
		}
		for (int i = 0; i < size; i++) {
			for (int j = i + 1; j < size; j++) {
				if (mass[i][j] != 0) {
					list[i].add(j + 1);
					list[j].add(i + 1);
				}
			}
		}

		if (iscolored) {
			iscolored = false;
			delete q.next;
			q.next = nullptr;
			q.data = -1;
			colorit();
		}

		if (isrendered) {
			isrendered = false;
			render.reinit();
			dorender();
		}
	}

	void printselect(unsigned char x, unsigned char y) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (i == j) {
					cout << "\033[90m#\033[0m ";
					continue;
				}
				if (x == j && y == i) cout << "\033[41m"; 
				if (x == i && y == j) cout << "\033[31m";
					cout << (int)mass[i][j] << "\033[0m ";
			}
			cout << endl;
		}
	}

	void tozero() {
		for (int i = 0; i < size; i++) {
			for (int j = i; j < size; j++) {
				mass[i][j] = mass[j][i] = 0;
			}
		}
		reinit();
	}

	void change() {
		cout << "\nКрасные квадрат - курсор \nW A S D - двигать курсор, Q - выход, иначе - инвертировать значение.\n" << endl;
		bool end = false;
		unsigned char x, y;
		x = size < 4 ? size - 1 : size - 2;
		y = size < 4 ? 0 : 1;
		printselect(x, y);
		while (!end) {
			char move = _getch();
			unsigned char newx, newy;
			newx = x;
			newy = y;
			switch (tolower(move)) {
			case 'w': {
				newy -= (y - 1 == x) ? 2 : 1;
				break;
			}
			case 's': {
				newy += (y + 1 == x) ? 2 : 1;
				break;
			}
			case 'a': {
				newx -= (x - 1 == y) ? 2 : 1;
				break;
			}
			case 'd': {
				newx += (x + 1 == y) ? 2 : 1;
				break;
			}
			case 'q': {
				end = true;
				break;
			}
			default: {
				mass[x][y] = mass[y][x] = mass[x][y] == 1 ? 0 : 1;
				break;
			}
			}
			if (newy >= 0 && newy < size) y = newy;
			if (newx >= 0 && newx < size) x = newx;
			cout << "\033[" << (int)size << "A";
			printselect(x, y);
		}
		reinit();

	}

	void grafinit() {
		cout << "\nВведите размер графа: ";
		scanf_s("%hhi", &size);
		//cout << "Изменить доп. параметры графа? (Взешенность, ориентированность) (Y/n)";
		//char pick;
		//pick = _getch();
		//if (pick == 'y' || pick == 'Y') {
		//	cout << "Граф будет взвешанный? (Y/n)";
		//	char isit;
		//	isit = _getch();
		//	isvzvesh = (isit == 'Y' || isit == 'y') ? true : false;
		//	cout << "Граф будет ориинтированный? (Y/n)";
		//	isit = _getch();
		//	isorient = (isit == 'Y' || isit == 'y') ? true : false;
		//}
		init();
		cout << "Заполнить его случайными значениями? (Y/n)\n";
		char pick;
		pick = _getch();
		if (pick == 'N' || pick == 'n') {
			tozero();
			change();
		}

	}

	void changesize(unsigned char newsize) {
		unsigned char minsize = min(newsize, size);
		char** newmass = new char* [newsize];
		for (int i = 0; i < newsize; i++) {
			newmass[i] = new char[newsize];
		}
		for (int i = 0; i < minsize; i++) {
			for (int j = i; j < minsize; j++) {
				newmass[i][j] = newmass[j][i] = mass[i][j];
			}
		}
		if (newsize > minsize) {
			for (int i = 0; i < newsize; i++) {
				for (int j = minsize; j < newsize; j++) {
					newmass[i][j] = newmass[j][i] = 0;
				}
			}
		}
		for (int i = 0; i < size; i++) {
			delete[] mass[i];
		}
		delete mass;
		mass = newmass;
		size = newsize;
		reinit();
	}

	void rerand() {
		for (int i = 0; i < size; i++) {
			for (int j = i; j < size; j++) {
				if (i == j) mass[i][j] = 0;
				else mass[i][j] = mass[j][i] = rand() % 2;
			}
		}
		reinit();
	}

	void savetofile(string path) {
		ofstream file(path);
		file << "GRAF\n" << (int) size << endl;
		for (int i = 0; i < size - 1; i++) {
			for (int j = i + 1; j < size; j++) {
				file.put(mass[i][j]);
			}
			file << endl;
		}
		file << isvzvesh << " " << isorient << " " << iscolored << " " << isrendered << endl;
		file.close();
	}

	void readfromfile(string path) {
		if (!exists(path)) return;
		if (filesystem::is_empty(path)) return;
		
		ifstream file(path);
		string content;
		file >> content;
		if (content != "GRAF") return;
		int newsize;
		file >> newsize;
		file.get();
		if (size == 0) {
			size = newsize;
			init();
		}
		else {
			changesize(newsize);
		}
		tozero();
		for (int i = 0; i < size - 1; i++) {
			for (int j = i + 1; j < size; j++) {
				mass[i][j] = mass[j][i] = file.get();
				print();
			}
			file.get();
		}
		isvzvesh = file.get();
		isorient = file.get();
		iscolored = file.get();
		isrendered = file.get();
		file.close();
		reinit();
	}

};

int search(string *path) {
	bool end = false;
	string newpath;
	cout << "\nВход в режим поиска, вводите название папок для передвижения по ним, '.' для движения назад, '.q' для выхода.\n" << endl;
	while (!end) {
		int isgood = isfilegood(*path);
		switch (isgood) {
		case  0: {

			string command = "dir /b /o:e " + *path;
			system(command.c_str());

			cout << "\n" << *path;
			if (!path->ends_with('\\')) cout << "\\";
			getline(cin, newpath);

			if (newpath == ".q" || newpath == ".Q") {
				cout << "\nВыход из режима поиска." << endl;
				return -1;
			}
			if (newpath == ".") {
				path->erase(path->find_last_of('\\'));
				if (path->length() == 2) *path = "c:\\";
				newpath.clear();
				break;
			}
			if (!path->ends_with('\\')) *path += "\\";
			if (exists(*path + newpath) || (newpath.find(".txt") < newpath.length())) *path += newpath;
			break;
		}
		case 1: {
			return 1;
		}
		case 2: {
			return 2;
		}
		default: {
			path->erase(path->find_last_of('\\'));
			if (path->length() == 2) *path = "c:\\";
			newpath.clear();
			break;
		}
		};
	}
};

int isfilegood(string path) {
	if (is_directory(path)) return 0;
	if (path.find(".txt") < path.length()) {
		if (!exists(path)) return 1;
		if (filesystem::is_empty(path)) return 1;
		ifstream file(path);
		if (!file) return -1;
		string content;
		file >> content;
		file.close();
		if (content == "GRAF") return 2;
	}
	return -1;
}

int main() {
	system("chcp 1251 > nul");
	Graf graph;
	srand(time(NULL));
	cout << "Добро пожаловать в программу для курсовой работы на тему \"Раскраска графа\"!\n" << endl;

	/*graph.size = 6;
	graph.init();
	graph.colorit();
	graph.dorender();
	graph.savetofile("C:\\Users\\dima_\\Desktop\\Papka\\c++\\goida.txt");
	graph.readfromfile("C:\\Users\\dima_\\Desktop\\Papka\\c++\\goida.txt");
	graph.print();
	graph.printlist();
	graph.render.print();*/

	//graph.size = 6;
	//graph.init();
	//graph.colorit();
	//graph.dorender();

	
	cout << "Считать граф из файла? (Y/n)";
	char pick;
	pick = _getch();
	if (!(pick == 'N' || pick == 'n')) {
		cout << "\nВведите (полный путь\\название.txt ИЛИ название.txt ИЛИ папка для поиска по папкам) от куда нужно считать файл: ";
		string path;
		getline(cin, path);
		if (path == "") getline(cin, path);
		bool end = false;
		if (path == ".q") {
			end = true;
			graph.grafinit();
		}
		while (!end) {
			int isgood = isfilegood(path);
			switch (isgood) {
			case 2: {
				graph.readfromfile(path);
				end = true;
				break;
			}
			case 1:
			case -1: {
				cout << "\nНеверный файл\nВведите заново:";
				getline(cin, path);
				break;
			}
			case 0: {
				if (search(&path) == -1) {
					end = true;
					graph.grafinit();
				}
				break;
			}
			}
			if (path == ".q") {
				end = true;
				graph.grafinit();
			}
		}
	}
	else graph.grafinit();

	//graph.print();
	
	bool end = false;
	bool ask = true;
	bool needclear = true;
	char whatprint = 1;
	while (!end) {
		if (needclear) system("cls");
		if (needclear && graph.iscolored) cout << "Граф покрашен с помощью " << (int) graph.maxcolor << " цветов.\n\n";
		needclear = true;
		switch (whatprint) {
		case 1: {
			graph.print();
			break;
		}
		case 2: {
			graph.printlist();
			break;
		}
		case 3: {
			graph.render.print();
			break;
		}
		}
		cout << "\nОперации над графом: \nВыйти из программы - 1; Модифицировать граф - 2; Изменить вывод - 3; Сохранить граф в файл - 4; Считать граф из файла - 5;";
		pick = _getch();
		switch (pick) {
		case '1': {
			end = true;
			break;
		}
		case '2': {
			cout << "\nМодификация графа: \nРаскрасить граф - 1; Изменить несколько позиций - 2; Изменить размер - 3; \nЗаново заполнить граф случайными числами - 4; Очистить граф - 5\n";
			char pick2 = _getch();
			switch (pick2) {
			case '1': {
				system("cls");
				needclear = false;
				graph.colorit();
				break;
			}
			case '2': {
				graph.change();
				break;
			}
			case '3': {

				cout << "\nВведите новый размер (от 2 до 100): ";
				int newsize;
				cin >> newsize; 
				while (newsize < 2 || newsize > 100) {
					cout << "\nНеверный размер!\nВведите новый размер (от 0 до 100): ";
					cin >> newsize;
				}
				graph.changesize(newsize);
				if (newsize > 9) {
					if (whatprint == 3) whatprint = 1;
					graph.isrendered = false;
				}
				break;
			}
			case '4': {
				graph.rerand();
				break;
			}
			case '5': {
				graph.tozero();
				break;
			}
			}
			break;
		}
		case '3': {
			cout << "\nCпособы вывода:\n";
			if (whatprint == 1) cout << "\033[4mВыводить матрицу\033[0m - 1;"; else cout << "Выводить матрицу - 1;";
			if (whatprint == 2) cout << " \033[4mВыводить список\033[0m - 2;"; else cout << " Выводить список - 2;";
			if (!graph.iscolored) cout << "\033[30;1m";
			if (graph.size < 10) 
				if (whatprint == 3) cout << " \033[4mОтрисовать граф\033[24m - 3.\033[0m" << endl; else cout << " Отрисовать граф - 3.\033[0m" << endl;
			char pick2 = _getch();
			if (pick != whatprint) {
				switch (pick2) {
				case '1': {
					whatprint = 1;
					break;
				}
				case '2': {
					whatprint = 2;
					break;
				}
				case '3': {
					if (graph.size > 10) break;
					if (graph.size > 7 && ask) {
						cout << "\nПри отрисовке графа размером больше 7 могут возникать ошибки, продолжить? (Y/n)";
						ask = false;
						char pick2 = _getch();
						if (pick2 == 'N' || pick2 == 'n') break;
					}
					if (!graph.iscolored) {
						cout << "\nНельзя отрисовать нераскрашенный граф.\nРаскрасить его? (Y/n)";
						pick2 = _getch();
						if (!(pick2 == 'N' || pick2 == 'n')) graph.colorit(); else break;
					}
					graph.dorender(true);
					whatprint = 3;
					break;
				}
				}
			}
			break;
		}
		case '4': {
			cout << "\nВведите (полный путь\\название.txt ИЛИ название.txt ИЛИ папка для поиска по папкам), куда нужно будет сохранить граф: ";
			string path;
			getline(cin, path);
			if (path == "") getline(cin, path);
			bool end = false;
			if (path == ".q") end = true;
			while (!end) {
				int isgood = isfilegood(path);
				switch (isgood) {
				case 1: {
					graph.savetofile(path);
					end = true;
					break;
				}
				case 2: {
					cout << "\nВ этом файле находится другой граф, перезаписать его? (Y/n)" << endl;
					char choise = _getch();
					if (!(choise == 'N' || choise == 'n')) {
						graph.savetofile(path);
						end = true;
						break;
					}
					else path.erase(path.find_last_of('\\'));
					if (search(&path) == -1) end = true;
					break;
				}
				case -1: {
					cout << "\nНеверный файл\nВведите заново:";
					getline(cin, path);
					break;
				}
				case 0: {
					if (search(&path) == -1) end = true;
					break;
				}
				}
				if (path == ".q") end = true;
			}

			break;
		}
		case '5': {
			cout << "\nВведите (полный путь\\название.txt ИЛИ название.txt ИЛИ папка для поиска по папкам) от куда нужно считать файл: ";
			string path;
			getline(cin, path);
			if (path == "") getline(cin, path);
			bool end = false;
			if (path == ".q") end = true;
			while (!end) {
				int isgood = isfilegood(path);
				switch (isgood) {
				case 2: {
					graph.readfromfile(path);
					end = true;
					break;
				}
				case 1:
				case -1: {
					cout << "\nНеверный файл\nВведите заново:";
					getline(cin, path);
					break;
				}
				case 0: {
					if (search(&path) == -1) {
						end = true;
					}
					break;
				}
				}
				if (path == ".q") end = true;
			}
			
			break;
		}
		}


	}
}
/*

1: 2 5
2: 1 3
3: 2 4 6
4: 3 5
5: 1 4 6
6: 3 5

1---2---3
 \_ ___/|
  _X    |
 / |    |
4---5---6


  1   1
   2   5 6
	3 4
   6
  5

1: 2 3 6
2: 1
3: 1 5 6
4: 5 6
5: 3 4 6
6: 1 3 4 5

/-------\
1---2   3
 \__ __/|
	X__ |
	|  \|
4---5---6
\-------/


  2   E
   1 3
  C 6
   5 4


1: 2 3 6
2: 1 7
3: 1 4 5 6 7
4: 3 5 6
5: 3 4 6
6: 1 3 4 5 7
7: 2 3 6


  3
   7
  2 6
   1   6
	3 4
   6 5
  1


1: 3 4 5 6 7
2: 4 5 7
3: 1 4 6 7
4: 1 2 3 5 6 7
5: 1 2 4
6: 1 3 4
7: 1 2 3 4


	  6
   1  1  5
	7 3 4 2 7
   2 4 6
	  2 1




*/
