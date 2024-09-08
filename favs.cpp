
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


class fav {

	std::vector<std::string> favs;

    fav()

	public:
	void save_favs() {
		std::ofstream file("misfavoritos.txt");
		for (int i = 0; i < favs.size(); i++) {
			file << favs[i] << std::endl;
		}
		file.close();
	}

	void load_favs() {
		std::ifstream file("misfavoritos.txt");
		std::string name;
		while (std::getline(file, name)) {
			favs.push_back(name);
		}
		file.close();
	}

	int search_favs(const std::string &name) {
		for (int i = 0; i < favs.size(); i++) {
			if (favs[i] == name) {
				return i;
			}
		}
		return -1;
	}

	void add_fav(const std::string &name) {
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

	void print_favs() {
		for (int i = 0; i < favs.size(); i++) {
			std::cout << i << ". " << favs[i] << std::endl;
		}
	}
};