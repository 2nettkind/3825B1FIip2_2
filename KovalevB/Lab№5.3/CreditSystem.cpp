#include <iostream>
#include <string>
#include <cmath>

using namespace std;

struct ActiveLoan {
    int MonthsLeft;
    double CurrentDebt;
    double PercentRate;
    double MonthlyPayment;
};

struct Client {
    string ID;
    string Password;
    string FullName;
    double Amount;

    ActiveLoan CreditInfo;
    bool LoanTaken = false;

    Client()
    : ID(""), Password(""), FullName(""), Amount(0.0), CreditInfo({}), LoanTaken(false) {};

    Client (string id, string pass, string fName, double amnt, ActiveLoan CI = {}, bool LT = false)
    : ID(id), Password(pass), FullName(fName), Amount(amnt), CreditInfo(CI), LoanTaken(LT) {}
};

class ProcessingCenter{
    private:

    Client* clients;
    int size;
    int cap;

    void resize() {
        cap *= 2;
        Client* newclients = new Client[cap];
        for (int i = 0; i < size; i++) {
            newclients[i] = clients[i];
        }
        delete[] clients;
        clients = newclients;
    }

    public: 

    bool ClientExists(const string& id) {
        for (int i = 0; i < size; i++) {
            if (clients[i].ID == id) return true;
        }
        return false;
    }

    ProcessingCenter(): size(0), cap(10) {
         clients = new Client[cap];
    }

    ProcessingCenter(const ProcessingCenter&) = delete;

    ProcessingCenter& operator=(const ProcessingCenter&) = delete;

    void AddClient(const Client& clnt) {
        if (size >= cap) {
            resize();
        }
        clients[size] = clnt;
        size++;
    }

    ~ProcessingCenter() {
        delete[] clients;
    }

    Client* FindClient(const string& id, const string& pass) {
        for (int i = 0; i < size; i++) {
            if (clients[i].ID == id && clients[i].Password == pass) {
                return &clients[i];
            }
        }
        return nullptr;
    }
};

class CreditSystem {
    private:
    
    ProcessingCenter& pc;
    Client* currClient = nullptr;

    double GetPercentRate(double amount, int years) const {
        if (amount <= 100'000) {
            return (years <= 3) ? 0.16 : 0.13;
        } 
        else if (amount <= 500'000) {
            return (years <= 3) ? 0.12 : 0.14;
        } 
        else if (amount <= 1'000'000) {
            return (years <= 3) ? 0.15 : 0.17;
        }
        else { 
            return 0.11;
        }
    }

    double CalculateMonthlyPayment(double amount, int years) const {
        if ((amount < 1 || amount > 3'000'000) || 
            (years != 1 && years != 2 && years != 3 && years != 5 && years != 15)) {
            return -1;
        }

        double creditRate = GetPercentRate(amount, years);

        double monthlyRate = creditRate / 12.0;
        int overallMonths = years * 12;

        double temp = std::pow(1 + monthlyRate, overallMonths);
        double payment = amount * (monthlyRate * temp) / (temp - 1); 

        return payment;
    }

    public:

    CreditSystem(ProcessingCenter& cen): pc(cen), currClient(nullptr) {}

    void ResetSession() {
        currClient = nullptr;
    }

    Client* GetCurrentClient() const { 
        return currClient; 
    }

    bool AuthClient(const string& id, const string& pass) {
        currClient = pc.FindClient(id, pass);
        return currClient != nullptr;
    }

    void ShowCurrentLoan() const {
        if (currClient == nullptr) {
            cout << "Error: No active session. Please authorize first" << '\n';
            return;
        } 

        if (!currClient->LoanTaken) {
            cout << "You do not have any active loans" << '\n';
            return;
        }
        
        cout << "=== CURRENT LOAN STATE ===" << '\n';
        cout << "Client Account: " << currClient->ID << '\n';
        cout << "Current Debt (Body): " << currClient->CreditInfo.CurrentDebt << " rub." << '\n';
        cout << "Annual Interest Rate: " << (currClient->CreditInfo.PercentRate * 100) << "%" << '\n';
        cout << "Fixed Monthly Payment: " << currClient->CreditInfo.MonthlyPayment << " rub." << '\n';
        cout << "Months Remaining: " << currClient->CreditInfo.MonthsLeft << '\n';
        cout << "==========================" << '\n';
    }

    bool RegisterNewClient(string id, string password, string fullName, double amount) {
        if (password.length() <= 3) {
            cout << "Error: password length must be greater than 3 " << '\n';
            return false;
        }
        if (id.length() != 4 || (id < "0001" || id > "9999")) {
            cout << "Error: id must be in [0001, 9999] range" << '\n';
            return false;
        }

        if (pc.ClientExists(id)) {
            cout << "Error: client with this ID already exists\n";
            return false;
        }

        Client c(id, password, fullName, amount);
        pc.AddClient(c);

        cout << "Client successfully registered !" << '\n';
        return true;
    }

    bool CheckActiveLoan() const {
        if (currClient == nullptr) return false; 
        return currClient->LoanTaken; 
    }

    bool CheckCreditAviable(double EstCreditAmount , int EstCreditYears) const {
        if (currClient == nullptr) return false;
        if (currClient->LoanTaken) return false;

        double monthlyPayment = CalculateMonthlyPayment(EstCreditAmount, EstCreditYears);

        if (monthlyPayment < 0) {
            return false;
        }
        return currClient->Amount >= 6 * monthlyPayment;
    }

    void ShowAvailableCredits() const {
        cout << "=== AVAILABLE CREDIT PROGRAMS ===" << '\n';
    cout << "1. Amount up to 100,000 rub:" << '\n';
    cout << "   - Term <= 3 years: 16% annual rate" << '\n';
    cout << "   - Term > 3 years:  13% annual rate" << '\n';
    cout << "2. Amount 100,001 - 500,000 rub:" << '\n';
    cout << "   - Term <= 3 years: 12% annual rate" << '\n';
    cout << "   - Term > 3 years:  14% annual rate" << '\n';
    cout << "3. Amount 500,001 - 1,000,000 rub:" << '\n';
    cout << "   - Term <= 3 years: 15% annual rate" << '\n';
    cout << "   - Term > 3 years:  17% annual rate" << '\n';
    cout << "4. Amount 1,000,001 - 3,000,000 rub:" << '\n';
    cout << "   - Any term: 11% annual rate" << '\n';
    cout << "=================================" << '\n';
    cout << "* Max credit amount: 3,000,000 rub." << '\n';
    cout << "* Available terms: 1, 2, 3, 5, 15 years." << '\n';
    }

    void TakeLoan(double amount, int years) {
        if (currClient == nullptr) {
            cout << "Error: there is no active client" << '\n';
            return;
        }

        if (!CheckCreditAviable(amount, years)) {
            cout << "Error: credit for this client are not aviable" << '\n';
            return;
        }

        currClient->CreditInfo.MonthlyPayment = CalculateMonthlyPayment(amount, years);
        currClient->CreditInfo.MonthsLeft = years * 12;
        currClient->CreditInfo.CurrentDebt = amount;
        currClient->CreditInfo.PercentRate = GetPercentRate(amount, years);
        currClient->LoanTaken = true;

        currClient->Amount += amount;

        cout << "Success: Loan for " << amount << " rubles for " << years 
             << " years successfully issued!" << '\n';
    }

    void EarlyPayment(double payAmount) {
        if (currClient == nullptr) return;
        if (!currClient->LoanTaken) return;

        if (payAmount <= 0.0 || payAmount > currClient->Amount) return;

        if (payAmount > currClient->CreditInfo.CurrentDebt) {
            payAmount = currClient->CreditInfo.CurrentDebt;
        }

        currClient->Amount -= payAmount;
        currClient->CreditInfo.CurrentDebt -= payAmount;

        if (currClient->CreditInfo.CurrentDebt <= 0) {
            currClient->LoanTaken = false;
            currClient->CreditInfo = {};
            return;
        }

        double monthlyRate = currClient->CreditInfo.PercentRate / 12;
        int monthsLeft = currClient->CreditInfo.MonthsLeft;

        double temp = std::pow(1 + monthlyRate, monthsLeft);

        currClient->CreditInfo.MonthlyPayment = currClient->CreditInfo.CurrentDebt * (monthlyRate * temp) / (temp - 1); 
    }

    void RegularPayment() {
        if (currClient == nullptr) return;
        if (!currClient->LoanTaken) return;

        if (currClient->Amount < currClient->CreditInfo.MonthlyPayment) {
            cout << "Error: not enough money on account to cover monthly payment!" << '\n';
            return;
        }

        currClient->Amount -= currClient->CreditInfo.MonthlyPayment;

        // *остаток* тела долга = тело долга - остаток от ежемесячного платежа за вычетом погашения процентов 
        currClient->CreditInfo.CurrentDebt -= 
        (currClient->CreditInfo.MonthlyPayment - 
        (currClient->CreditInfo.CurrentDebt * (currClient->CreditInfo.PercentRate / 12.0)));
        
        --currClient->CreditInfo.MonthsLeft;
        cout << "Success: Regular payment processed!" << '\n';
        
        if (currClient->CreditInfo.MonthsLeft <= 0) {
            currClient->LoanTaken = false; 
            currClient->CreditInfo = {};
            std::cout << "Success: The loan has been fully repaid!" << '\n';
        }
    }
};  

void SetupDemoDatabase(ProcessingCenter& pc) {
    pc.AddClient(Client("0001", "1234", "Ivanov Ivan", 50000.0));
    pc.AddClient(Client("0002", "qwerty", "Petrov Petr", 250000.0));
}

int main() {
    ProcessingCenter pc;
    SetupDemoDatabase(pc);
    CreditSystem bank(pc);

    auto isValidYears = [](int y) {
        return y == 1 || y == 2 || y == 3 || y == 5 || y == 15;
    };
    auto isValidAmount = [](double a) {
        return a >= 1 && a <= 3'000'000;
    };

    while (true) {
        if (bank.GetCurrentClient() == nullptr) {
            cout << "\n=== BANKING SYSTEM ===\n"
                 << "1. Authorize (Login)\n"
                 << "2. Register New Client\n"
                 << "3. Show Tariff Screen\n"
                 << "0. Exit Program\n"
                 << "Select action: ";
            int choice;
            if (!(cin >> choice)) break;

            switch (choice) {
                case 1: {
                    string id, pass;
                    cout << "Enter Account ID: "; cin >> id;
                    if (id.length() != 4 || id < "0001" || id > "9999") {
                        cout << "Error: ID must be in [0001, 9999] range\n";
                        break;
                    }
                    cout << "Enter Password: "; cin >> pass;
                    if (pass.length() <= 3) {
                        cout << "Error: password length must be greater than 3\n";
                        break;
                    }
                    if (bank.AuthClient(id, pass)) cout << "Authorization successful!\n";
                    else cout << "Authorization failed!\n";
                    break;
                }
                case 2: {
                    string id, pass, name;
                    double amount;

                    cout << "Create ID: "; cin >> id;
                    if (id.length() != 4 || id < "0001" || id > "9999") {
                        cout << "Error: ID must be in [0001, 9999] range\n";
                        break;
                    }
                    cout << "Create Password: "; cin >> pass;
                    if (pass.length() <= 3) {
                        cout << "Error: password length must be greater than 3\n";
                        break;
                    }
                    cout << "Enter Full Name: "; cin.ignore(); getline(cin, name);
                    cout << "Enter Initial Balance: "; cin >> amount;
                    if (amount < 0) {
                        cout << "Error: initial balance cannot be negative\n";
                        break;
                    }
                    bank.RegisterNewClient(id, pass, name, amount);
                    break;
                }
                case 3:
                    bank.ShowAvailableCredits();
                    break;
                case 0:
                    return 0;
                default:
                    cout << "Invalid choice!\n";
                    break;
            }
        } else {
            Client* c = bank.GetCurrentClient();
            cout << "\n=== ACCOUNT CABINET [" << c->FullName << "] ===\n"
                 << "Current Account Balance: " << c->Amount << " rub.\n";
            if (bank.CheckActiveLoan()) {
                cout << "Loan Status: ACTIVE (Monthly payment: "
                     << c->CreditInfo.MonthlyPayment << " rub.)\n";
            } else {
                cout << "Loan Status: NONE\n";
            }
            cout << "--------------------------------------\n"
                 << "1. Check Credit Availability\n"
                 << "2. Take Out a New Loan\n"
                 << "3. View Current Loan Details\n"
                 << "4. Make Scheduled Regular Payment\n"
                 << "5. Process Early Repayment\n"
                 << "6. Logout\n"
                 << "Select action: ";
            int choice;
            if (!(cin >> choice)) break;

            switch (choice) {
                case 1: {
                    double amnt; int yrs;
                    cout << "Enter amount to check: "; cin >> amnt;
                    if (!isValidAmount(amnt)) {
                        cout << "Error: amount must be in [1, 3,000,000] range\n";
                        break;
                    }
                    cout << "Enter term in years (1/2/3/5/15): "; cin >> yrs;
                    if (!isValidYears(yrs)) {
                        cout << "Error: term must be one of: 1, 2, 3, 5, 15\n";
                        break;
                    }
                    if (bank.CheckCreditAviable(amnt, yrs)) cout << "Status: APPROVED\n";
                    else cout << "Status: REJECTED\n";
                    break;
                }
                case 2: {
                    double amnt; int yrs;
                    cout << "Enter amount to borrow: "; cin >> amnt;
                    if (!isValidAmount(amnt)) {
                        cout << "Error: amount must be in [1, 3,000,000] range\n";
                        break;
                    }
                    cout << "Enter term in years (1/2/3/5/15): "; cin >> yrs;
                    if (!isValidYears(yrs)) {
                        cout << "Error: term must be one of: 1, 2, 3, 5, 15\n";
                        break;
                    }
                    bank.TakeLoan(amnt, yrs);
                    break;
                }
                case 3:
                    bank.ShowCurrentLoan();
                    break;
                case 4:
                    bank.RegularPayment();
                    break;
                case 5: {
                    double pay;
                    cout << "Enter amount for early payment: "; cin >> pay;
                    if (pay <= 0) {
                        cout << "Error: payment amount must be greater than 0\n";
                        break;
                    }
                    if (!bank.CheckActiveLoan()) {
                        cout << "Error: no active loan\n";
                        break;
                    }
                    bank.EarlyPayment(pay);
                    break;
                }
                case 6:
                    bank.ResetSession();
                    cout << "Logged out.\n";
                    break;
                default:
                    cout << "Invalid choice!\n";
                    break;
            }
        }
    }
}
