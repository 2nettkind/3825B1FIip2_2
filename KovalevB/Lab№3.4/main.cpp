#include <cmath>
#include <stdexcept>
#include <iostream>
#include <string>

using std::cout, std::cin, std::string;
using funcptr = double(*)(double);

class UserMenu {
    private:

    int amount;
    std::string* titles;
    funcptr*    functions;
    int lastSelected;

    void freeMemory() {
        delete[] titles;
        delete[] functions;
        titles = nullptr;
        functions = nullptr;
    }

    void allocMemory(int n) {
        titles = new string[n];
        functions = new funcptr[n]();
    }

    public:

    UserMenu(): 
    amount(0), 
    titles(nullptr), 
    functions(nullptr), 
    lastSelected(-1) 
    {}

    // не валидный конструктор от кол-ва команд,
    // т.к по условию нужен отдельный метод: "задать число команд" 
    UserMenu(int amount_): 
    amount(0), 
    titles(nullptr),
    functions(nullptr), 
    lastSelected(-1)
    {}

    UserMenu(const UserMenu& o): amount(o.amount), lastSelected(o.lastSelected) {
        allocMemory(o.amount);
        for (int i = 0; i < o.amount; ++i) {
            titles[i] = o.titles[i];
            functions[i] = o.functions[i]; 
        }
    }

    UserMenu& operator=(const UserMenu& o) {
        if (this != &o) {
            freeMemory();
            amount = o.amount;
            lastSelected = o.lastSelected;
            allocMemory(amount);
            for (int i = 0; i < amount; ++i) {
                titles[i] = o.titles[i];
                functions[i] = o.functions[i];
            }
        }
        return *this;
    }

    ~UserMenu() {
        freeMemory();
    }

    void SetCmdAmount(int n) {
        if (n <= 0) {
            std::string error("Amout of comands must be positive!");
            throw error;
        }   
        freeMemory();
        amount = n;
        allocMemory(n);
        lastSelected = -1;
    }

    int GetCmdAmount() const {
        return amount;
    }

    void SetTitle(int index, const string& title, funcptr func) {
        if (index < 1 || index > amount) {
            string error("Index out of range!");
            throw error;
        }
        titles[index - 1] = title;
        functions[index - 1] = func;
    }

    void ShowMenu() const {
        cout << ">>>>>>>>>> MENU <<<<<<<<<<"<< '\n';
        for (int i = 0; i < amount; ++i) {
            cout << (i + 1) << ") " << titles[i] << '\n';
        } 
        cout << "0) Exit\n";
    }

    int GetLatest() const {
        return lastSelected;
    }

    void Run() {
        while (true)
        {
            ShowMenu(); 

            int choice;

            cout << "Choose option: ";
            cin >> choice;

            if (choice == 0) {
                cout << "Exiting menu.\n";
                return;
            }

            if (choice < 1 || choice > amount) {
                string error("There is no function with this number.");
                throw error;
            }   
        
            if (functions[choice - 1] == nullptr) {
                string error("Function not setted yet by this number.");
                throw error;
            }

            lastSelected = choice;
        
            double arg;
            cout << "Choosed: " << titles[choice - 1] << '\n';
            cout << "Input function argument: ";
            cin >> arg;

            double res = functions[choice - 1](arg);
            cout << "Result: " << res << '\n';
        }
    }
};
    
int main() {
    UserMenu Menu;
    try {
        Menu.SetCmdAmount(5);
        Menu.SetTitle(1,"Sin(x)", sin);
        Menu.SetTitle(2, "Exp(x)", exp);
        Menu.SetTitle(3, "Log10", log10);   
        Menu.SetTitle(4, "Log e", log); 
        Menu.SetTitle(5, "Sinh(x)", sinh);
        Menu.Run();
    } catch (const string& e) {
        std::cerr << e << '\n';
    }
    cout << "Latest selected: " << Menu.GetLatest() << '\n';
}