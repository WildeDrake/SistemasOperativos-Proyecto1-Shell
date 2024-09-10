#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Fav {

	vector<string> favs;
	bool ruta_creada;
	string ruta;

	public:
	void crear(string ruta) {
		ofstream file(ruta);
		file.close();
		ruta_creada = true;
		this->ruta = ruta;
	}

	void save_favs() {
		if (!ruta_creada) {
			perror("no hay ruta ingresada");
			return;
		}

		ofstream file(ruta);
		for (int i = 0; i < favs.size(); i++) {
			file << favs[i] << endl;
		}
		file.close();
	}

	void load_favs() {
		if (!ruta_creada) {
			perror("no hay ruta ingresada");
			return;
		}

		ifstream file(ruta);
		if (file.is_open()) {
			string name;
			while (getline(file, name)) {
				favs.push_back(name);
			}
			file.close();
		}
	}

	int search_favs(const string &name) {
		for (int i = 0; i < favs.size(); i++) {
			if (favs[i] == name) {
				return i;
			}
		}
		return -1;
	}

	void add_fav(const string &name) {
		if (search_favs(name) == -1) {
			favs.push_back(name);
		}
	}

	void remove_fav(int index) {
		if (index >= 0 && index < favs.size()) {
			favs.erase(favs.begin() + index);
		}
	}
	void remove_fav(int i1, int i2) {
		if (i1 >= 0 && i1 < favs.size() && i2 >= 0 && i2 < favs.size()) {
			favs.erase(favs.begin() + i1, favs.begin() + i2 + 1);
		}
	}
	void remove_fav(const string &name) {
		int index = search_favs(name);
		if (index != -1) {
			favs.erase(favs.begin() + index);
		}
	}

	void remove_favs() {
		favs.clear();
	}

	void scan_favs(char *name) {
		for (int i = 0; i < favs.size(); i++) {
			if (favs[i].find(name) != string::npos) {
				cout << i << ". " << favs[i] << endl;
			}
		}
	}

	void print_fav(int index) {
		if (index >= 0 && index < favs.size()) {
			cout << index << ". " << favs[index] << endl;
		}
	}

	void print_favs() {
		for (int i = 0; i < favs.size(); i++) {
			cout << i << ". " << favs[i] << endl;
		}
	}

	string get_fav(int index) {
		if (index >= 0 && index < favs.size()) {
			return favs[index];
		}
		return "";
	}
};