#include <iostream>
#include <cstdlib>
using std::cout;
using std::cin;
class Time {
private:
unsigned int hours, minutes, seconds;
bool Err(unsigned int h, unsigned int m, unsigned int s) const {
	return (h >= 0 && h < 24) && (m >= 0 && m < 60) && (s >= 0 && s < 60);
}
void norm() {
	int allsec = hours * 3600 + minutes * 60 + seconds;
	allsec %= 86400;
	if (allsec < 0) {
		allsec += 86400;
	}
	hours = allsec / 3600;
	minutes = (allsec % 3600) / 60;
	seconds = allsec % 60;
}
public:
	Time(unsigned int h = 0, unsigned int m = 0, unsigned int s = 0) :hours(h), minutes(m), seconds(s) {
		while (!Err(h, m, s)) {
			cout << "Error: incorrect time\n";
			cout << "Repeat input\n";
			cout << "Hours: ";   
			cin >> h;
			cout << "Minutes: "; 
			cin >> m;
			cout << "Seconds: "; 
			cin >> s;
		}
		hours = h;
		minutes = m;
		seconds = s;
	};
	void SetTime(unsigned int h, unsigned int m, unsigned int s) {
		while (!Err(h, m, s)) {
			cout << "Error:incorrect time\n";
			cout << "Repeat input\n";
			cout << "Hours: ";
			cin >> h;
			cout << "Minutes: ";
			cin >> m;
			cout << "seconds: ";
			cin >> s;
		}
		hours = h;
		minutes = m;
		seconds = s;
	}
	
	void print() const {
		if (hours < 10) {
			cout << '0';
		}
		cout << hours << ':';
		if (minutes < 10) {
			cout << '0';
		}
		cout << minutes << ':';
		if (seconds < 10) {
			cout << '0';
		}
		cout << seconds << '\n';
	}
	void Difference(const Time& other) const {
		int Sec = hours * 3600 + minutes * 60 + seconds;
		int Oth_Sec = other.hours * 3600 + other.minutes * 60 + other.seconds;
		int diffsec = Oth_Sec - Sec;
		char sign = (diffsec >= 0) ? '+' : '-';
		int Diff = diffsec >= 0 ? (diffsec) : (-diffsec);
		int h = Diff / 3600;
		int m = (Diff % 3600) / 60;
		int s = Diff % 60;
		cout << sign;
		if (h < 10) {
			cout << '0';
		}
		cout << h << ':';
		if (m < 10) {
			cout << '0';
		}
		cout << m << ':';
		if (s < 10) {
			cout << '0';
		}
		cout << s << '\n';
	}
	void add(unsigned int h, unsigned int m, unsigned int s) {
		hours += h;
		minutes += m;
		seconds += s;
		norm();
	}
	void sub(unsigned int h, unsigned int m, unsigned int s) {
		hours -= h;
		minutes -= m;
		seconds -= s;
		norm();
	}
};
int main() {
	int ch;
	unsigned int h, m, s;
	cout << "=== Working with time ===\n";
	cout << "Hours: ";
	cin >> h;
	cout << "Minutes: ";
	cin >> m;
	cout << "Seconds: ";
	cin >> s;
	Time t1(h, m, s);
	do {
		cout << "Menu:\n";
		cout << "1.Show current time\n";
		cout << "2.Set a new time\n";
		cout << "3.Calculate the time difference\n";
		cout << "4.Add shift\n";
		cout << "5.Subtract shift\n" ;
		cout << "0.exit\n" ;
		cout << "Select an action: ";
		cin >> ch;
		switch (ch) {
		case 1:
			cout << "Current time: ";
			t1.print();
			cout << '\n';
			break;

		case 2: {
			cout << "Enter a new time:\n";
			cout << "Hours: ";
			cin >> h;
			cout << "Minutes: ";
			cin >> m;
			cout << "Seconds: ";
			cin >> s;
			t1.SetTime(h, m, s);
			cout << "The time is set: ";
			t1.print();
			cout << '\n';
			break;
		}

		case 3: {
			cout << "Enter time to compare:\n";
			cout << "Hours: ";
			cin >> h;
			cout << "Minutes: ";
			cin >> m;
			cout << "Seconds: ";
			cin >> s;
			Time t2(h, m, s);
			cout << "Difference (t2 - t1) = ";
			t1.Difference(t2);
			cout << '\n';
			break;
		}

		case 4: {
			cout << "Enter shift:\n";
			cout << "Hours: ";
			cin >> h;
			cout << "Minutes: ";
			cin >> m;
			cout << "Seconds: ";
			cin >> s;
			t1.add(h, m, s);
			cout << "New time: ";
			t1.print();
			cout << '\n';
			break;
		}

		case 5: {
			cout << "Enter shift:\n";
			cout << "Hours: ";
			cin >> h;
			cout << "Minutes: ";
			cin >> m;
			cout << "Seconds: ";
			cin >> s;
			t1.sub(h, m, s);
			cout << "New time: ";
			t1.print();
			cout << '\n';
			break;
		}
		case 0:
			cout << "End\n";
			break;

		default:
			cout << "Incorrect selection. Try again\n";
		}
	} while (ch != 0);
}
