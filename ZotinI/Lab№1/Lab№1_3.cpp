#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>

using namespace std;

class clocks {
private:
	unsigned sec, min, hours;

public:

	clocks() : hours(12), min(0), sec(0) {}

	void install_time() {
		bool c = false;
		cout << "Enter seconds (0-59): " << '\n';
		do {
			if (c) {
				cout << "Input error! Try again\n";
			}
			c = true;
			cin >> sec;
		} while (sec < 0 || sec >= 60);

		c = false;
		cout << "Enter minutes (0-59): " << '\n';
		do {
			if (c) {
				cout << "Input error! Try again\n";
			}
			c = true;
			cin >> min;
		} while (min < 0 || min >= 60);

		c = false;
		cout << "Enter hours (0-23): " << '\n';
		do {
			if (c) {
				cout << "Input error! Try again\n";
			}
			c = true;
			cin >> hours;
		} while (hours < 0 || hours >= 24);
		cout << "The time was set successfully" << '\n';
	}

	void print_time() const {
		cout << "Time now: " << hours << " hours " << min << " minutes " << sec << " seconds\n";
	}

	void difference(unsigned hours0, unsigned min0, unsigned sec0) const {
		int ins = hours * 3600 + min * 60 + sec;
		int zd = hours0 * 3600 + min0 * 60 + sec0;
		int dif = abs(ins - zd);

		int dhours = dif / 3600;
		int dmin = (dif % 3600) / 60;
		int dsec = dif % 60;

		cout << "The difference is " << dhours << " hours " << dmin << " minutes " << dsec << " seconds\n";
	}

	void difference() const {
		unsigned hours0, min0, sec0;
		bool c = false;
		cout << "Enter seconds (0-59): " << '\n';
		do {
			if (c) {
				cout << "Input error! Try again\n";
			}
			c = true;
			cin >> sec0;
		} while (sec0 < 0 || sec0 >= 60);

		c = false;
		cout << "Enter minutes (0-59): " << '\n';
		do {
			if (c) {
				cout << "Input error! Try again\n";
			}
			c = true;
			cin >> min0;
		} while (min0 < 0 || min0 >= 60);

		c = false;
		cout << "Enter hours (0-23): " << '\n';
		do {
			if (c) {
				cout << "Input error! Try again\n";
			}
			c = true;
			cin >> hours0;
		} while (hours0 < 0 || hours0 >= 24);

		difference(hours0, min0, sec0);
	}

	void sdv() {
		char s;
		bool c = false;
		cout << "Choose which way the time shift will be: '+' or '-'\n";
		do {
			if (c) {
				cout << "Input error! Try again\n";
			}
			cin >> s;
			c = true;
		} while (s != '+' && s != '-');

		c = false;
		int h, m, sc;

		cout << "How much hours to set the offset:\n";
		do {
			if (c) {
				cout << "Input error! Try again\n";
			}
			cin >> h;
			c = true;
		} while (h < 0);

		c = false;
		cout << "How much minutes to set the offset:\n";
		do {
			if (c) {
				cout << "Input error! Try again\n";
			}
			cin >> m;
			c = true;
		} while (m < 0);

		c = false;
		cout << "How much seconds to set the offset:\n";
		do {
			if (c) {
				cout << "Input error! Try again\n";
			}
			cin >> sc;
			c = true;
		} while (sc < 0);

		int totalsec = hours * 3600 + min * 60 + sec;
		int sdvsec = h * 3600 + m * 60 + sc;

		if (s == '+') {
			totalsec += sdvsec;
		}
		else {
			totalsec -= sdvsec;
		}

		totalsec %= 24 * 3600;
		if (totalsec < 0) {
			totalsec += 24 * 3600;
		}

		hours = totalsec / 3600;
		min = (totalsec % 3600) / 60;
		sec = totalsec % 60;

		cout << "Time shifted successfully\n";
	}
};


int main() {
	srand(time(nullptr));

	int s;
	clocks tim;
	bool c = false;
	do {
		cout << "Select an action: \n" << "1. Set the time\n" << "2. Find out the time\n" << "3. Calculate the difference between the set time and the set time\n"
			<< "4. Shift the time by a preset offset\n" << "0. Exit\n" << '\n';
		do {
			if (c) {
				cout << "Input error! Try again\n";
			}
			cin >> s;
			c = true;
		} while (!(s >= 0 && s <= 4));

		c = false;

		switch (s) {
		case 1:
			tim.install_time();
			break;
		case 2:
			tim.print_time();
			break;
		case 3:
			tim.difference();
			break;
		case 4:
			tim.sdv();
			break;
		default: break;
		}
		cout << '\n';
	} while (s != 0);
}