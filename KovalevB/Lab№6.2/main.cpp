#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

class Number {
    private:
    string secret_number;

    public:
    Number(int n) {
        if (n < 1) {
            throw std::invalid_argument("err: length of number must be natural value.");
        }
        string digits = "0123456789";
        for (int i = 0; i < n; ++i) {
             
            int j = i + rand() % (10 - i);

             char tmp = digits[i];
             digits[i] = digits[j];
             digits[j] = tmp;
        }
        secret_number = digits.substr(0,n);
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
                cin >> attempt;

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
            cout << "Input length: "; cin >> n; 
            Game G(n);   
            G.Start();
            break;
        } catch (const std::invalid_argument& e) {
            cout << "\nOooopsssss....... Error detected: " << e.what() << "\n\n";
            cout << "Wanna start new game ? 1/0: ";
            int ch; cin >> ch;
            if (!ch) break;
        }
    }  
}