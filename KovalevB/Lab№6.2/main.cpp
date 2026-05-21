#include <iostream>
#include <string>
#include <cctype>
#include <limits>
#include <stdexcept>

using namespace std;

class Number {
    private:
    string secret_number;

    public:
    Number(int n) {
        if (n < 1 || n > 9) { 
            throw std::invalid_argument("length must be between 1 and 9.");
        }
        string digits = "0123456789";

        int j = 1 + rand() % 9;
        char tmp = digits[0]; digits[0] = digits[j]; digits[j] = tmp;

        for (int i = 1; i < n; ++i) {
            j = i + rand() % (10 - i);
            tmp = digits[i]; digits[i] = digits[j]; digits[j] = tmp;
        }
        secret_number = digits.substr(0, n);
    } 

    string GetNumber() const { return secret_number; }
};


class Guess {
    private:
    int bulls = 0;
    int cows = 0;

    public:

    Guess(const string& ptry, const Number& secnum): bulls(0), cows(0) {
        string secret = secnum.GetNumber();
        
        for (size_t i = 0; i < ptry.length(); i++) {
            if (ptry[i] == secret[i]) {
                ++bulls;
            } else if (secret.find(ptry[i]) != string::npos) {
                cows++;
            }
        }
    }   

    int GetBulls() { return bulls; }
    int GetCows() { return cows; }

    bool IsWin(int n) const {
        return bulls == n;
    }
};


class Game {
    private:
    int n;
    Number secret;

    bool ValidateGuess(const string& s, int n) {
        if (s.size() != n) return false;
        if (s[0] == '0') return false;
        bool seen[10] = {};

        for (char c : s) {
            if (!isdigit(c)) return false;

            int d = c - '0';
            if (seen[d]) return false;
            seen[d] = true;
        }
        return true;
    }

    public:

    Game(int n_): n(n_), secret(n) {}

    void Start() {
        cout << "Secret is " << n << " digits. Start guessing!\n";
        string attempt;
            while (true) {
                cout << "Your guess: ";
                if (!(cin >> attempt)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "err: invalid input.\n";
                    continue;
                }

                if (!ValidateGuess(attempt, n)) {
                    cout << "err: string don't fits task conditions.\n";
                    continue;
                }

                Guess g(attempt, secret);
                cout << "Bulls - " << g.GetBulls() << "\n" 
                << "Cows - " << g.GetCows() << '\n';

                if (g.IsWin(n)) {
                    cout << "Your Win!" << '\n';
                    cout << "Secret number was: " << secret.GetNumber();
                    break;
                }
            }
            
        }
};

int main() {
    srand(time(nullptr));

    while (true) {
        try {
            int n;
            cout << "Input length: ";
            if (!(cin >> n)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "err: expected a number.\n\n";
                continue;
            } 
            Game G(n);   
            G.Start();
            break;
        } catch (const std::invalid_argument& e) {
            cout << "\nOooopsssss....... Error detected: " << e.what() << "\n\n";
            cout << "Wanna start new game ? 1/0: ";
            int ch;
            if (!(cin >> ch)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                ch = 0;
            }
            if (!ch) break;
        }
    }  
}