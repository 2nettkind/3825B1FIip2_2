#include <iostream>

class Temperature {
private:
	float temperature;
public:
	Temperature(float temp) {
		temperature = temp;
	}
	float GetTemperature() const {
		return temperature;
	}
	void SetTemperature(float temp) {
		temperature = temp;
	}
	float ConversionFahrenheit() const {
		return temperature * 1.8f + 32;
	}
	float ConversionKelvin() const {
		return temperature + 273.15f;
	}
	friend std::ostream& operator<<(std::ostream& os, const Temperature& t) {
		os << "Temperature:" << t.temperature << "C,"<< t.ConversionFahrenheit() << "F,"<< t.ConversionKelvin() << "K";
		return os;
	}
};

int main() {
	int choice = 0;
	float temp = 0;
	std::cout << "Enter the degrees (in Celsius)\n";
	std::cin >> temp;
	if (temp < -273.15f) {
		std::cout << "Error.The temperature cannot be below absolute zero (-273.15C)\n";
		return 1;
	}
	Temperature a(temp);
	std::cout << "Enter mode\n" "1.Convert to Fahrenheit\n" "2.Convert to Kelvin\n" "3.Console output\n";
	std::cin >> choice;
	if (choice < 1 || choice > 3) {
		std::cout << "Error\n";
		return 1;
	}
	switch (choice) {
	case 1:
		std::cout << "Result:" << a.ConversionFahrenheit() << "F\n";
		break;
	case 2:
		std::cout << "Result:" << a.ConversionKelvin() << "K\n";
		break;
	case 3:
		std::cout << a << std::endl;
	}
	return 0;
}
