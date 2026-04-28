#include <iostream>
#include <fstream>
#include <string>

using std::cout, std::cin, std::string;

struct Date {
    int day;
    int month;
    int year;
};

struct StepRecord {
    Date date;
    int  startHour;
    int  startMin;
    int  endHour;
    int  endMin;
    int  steps;
};

class StepCounter {
    private:

    Date startDate;
    bool startDateSet;
    StepRecord* records;
    int size;
    int capacity;

    // Внутренняя структура для агрегации шагов по датам
    struct DayTotal {
        Date date;
        int  steps;
    };

    void freeMemory() {
        delete[] records;
        records = nullptr;
    }

    void allocMemory(int n) {
        records = new StepRecord[n];
    }

    void increase() {
        int newCap = (capacity == 0) ? 4 : capacity * 2;
        StepRecord* tmp = new StepRecord[newCap];
        for (int i = 0; i < size; ++i) {
            tmp[i] = records[i];
        }
        delete[] records;
        records  = tmp;
        capacity = newCap;
    }

    static bool isLeap(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    static int daysInMonth(int month, int year) {
        if (month == 2) return isLeap(year) ? 29 : 28;
        if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
        return 31;
    }

    static bool isValidDate(const Date& d) {
        if (d.year < 1 || d.month < 1 || d.month > 12 || d.day < 1) return false;
        return d.day <= daysInMonth(d.month, d.year);
    }

    static bool isValidTime(int h, int m) {
        return h >= 0 && h < 24 && m >= 0 && m < 60;
    }

    
    // день недели считаем - как число дней, прошедших от опорной даты 1.1.1
    static int weekday(const Date& d) {
        long days = 0;

        for (int y = 1; y < d.year; ++y) {
            days += isLeap(y) ? 366 : 365;
        }

        for (int m = 1; m < d.month; ++m) {
            days += daysInMonth(m, d.year);
        }

        days += d.day - 1;

        return (int)(days % 7) + 1;
    }

    int aggregateByDay(DayTotal* outBuf, int monthFilter, int yearFilter, int wdFilter) const {
        int n = 0;
        for (int i = 0; i < size; ++i) {
            const StepRecord& r = records[i];

            if (monthFilter != 0 &&
                (r.date.month != monthFilter || r.date.year != yearFilter)) continue;
            if (wdFilter != 0 && weekday(r.date) != wdFilter) continue;

            int found = -1;
            for (int j = 0; j < n; ++j) {
                if (outBuf[j].date.day   == r.date.day &&
                    outBuf[j].date.month == r.date.month &&
                    outBuf[j].date.year  == r.date.year) {
                    found = j;
                    break;
                }
            }

            if (found >= 0) {
                outBuf[found].steps += r.steps;
            } else {
                outBuf[n].date  = r.date;
                outBuf[n].steps = r.steps;
                ++n;
            }
        }
        return n;
    }

    public:

    StepCounter():
    startDate{0, 0, 0},
    startDateSet(false),
    records(nullptr),
    size(0),
    capacity(0)
    {}

    StepCounter(const StepCounter& o):
    startDate(o.startDate),
    startDateSet(o.startDateSet),
    records(nullptr),
    size(o.size),
    capacity(o.capacity)
    {
        if (capacity > 0) {
            allocMemory(capacity);
            for (int i = 0; i < size; ++i) {
                records[i] = o.records[i];
            }
        }
    }

    StepCounter& operator=(const StepCounter& o) {
        if (this != &o) {
            freeMemory();
            startDate = o.startDate;
            startDateSet = o.startDateSet;
            size = o.size;
            capacity = o.capacity;
            if (capacity > 0) {
                allocMemory(capacity);
                for (int i = 0; i < size; ++i) {
                    records[i] = o.records[i];
                }
            }
        }
        return *this;
    }

    ~StepCounter() {
        freeMemory();
    }

    void SetStartDate(int day, int month, int year) {
        Date d{day, month, year};
        if (!isValidDate(d)) {
            string error("invalid start date");
            throw error;
        }
        startDate = d;
        startDateSet = true;
    }

    Date GetStartDate() const {
        if (!startDateSet) {
            string error("start date is not set");
            throw error;
        }
        return startDate;
    }

    void AddRecord(
        int day,
        int month,
        int year,
        int startH,
        int startM,
        int endH,
        int endM,
        int steps) {
        Date d{day, month, year};
        if (!isValidDate(d)) {
            string error("invalid record date");
            throw error;
        }
        if (!isValidTime(startH, startM) || !isValidTime(endH, endM)) {
            string error("invalid time interval");
            throw error;
        }
        if (endH * 60 + endM < startH * 60 + startM) {
            string error("end of interval is earlier than start");
            throw error;
        }
        if (steps < 0) {
            string error("steps must be non negative");
            throw error;
        }
        if (size == capacity) increase();
        records[size++] = StepRecord{d, startH, startM, endH, endM, steps};
    }

    int GetRecordsCount() const { return size; }

    StepRecord GetRecord(int index) const {
        if (index < 1 || index > size) {
            string error("record index out of range");
            throw error;
        }
        return records[index - 1];
    }

    double AvgStepsMonth(int month, int year) const {
        if (month < 1 || month > 12) {
            string error("invalid month");
            throw error;
        }
        if (size == 0) {
            string error("no records for given month");
            throw error;
        }
        DayTotal* buf = new DayTotal[size];
        int n = aggregateByDay(buf, month, year, 0);
        if (n == 0) {
            delete[] buf;
            string error("no records for given month");
            throw error;
        }
        long long sum = 0;
        for (int i = 0; i < n; ++i) sum += buf[i].steps;
        double avg = (double)sum / n;
        delete[] buf;
        return avg;
    }

    double AvgStepsTotal() const {
        if (size == 0) {
            string error("no records to compute average");
            throw error;
        }
        DayTotal* buf = new DayTotal[size];
        int n = aggregateByDay(buf, 0, 0, 0);
        long long sum = 0;
        for (int i = 0; i < n; ++i) sum += buf[i].steps;
        double avg = (double)sum / n;
        delete[] buf;
        return avg;
    }

    double AvgStepsByWeekday(int wd) const {
        if (wd < 1 || wd > 7) {
            string error("weekday must be in 1-7");
            throw error;
        }
        if (size == 0) {
            string error("no records for given weekday");
            throw error;
        }
        DayTotal* buf = new DayTotal[size];
        int n = aggregateByDay(buf, 0, 0, wd);
        if (n == 0) {
            delete[] buf;
            string error("no records for given weekday");
            throw error;
        }
        long long sum = 0;
        for (int i = 0; i < n; ++i) sum += buf[i].steps;
        double avg = (double)sum / n;
        delete[] buf;
        return avg;
    }

    int MaxStepsInMonth(int month, int year, Date& outDate) const {
        if (month < 1 || month > 12) {
            string error("invalid month");
            throw error;
        }
        if (size == 0) {
            string error("no records for given month");
            throw error;
        }
        DayTotal* buf = new DayTotal[size];
        int n = aggregateByDay(buf, month, year, 0);
        if (n == 0) {
            delete[] buf;
            string error("no records for given month");
            throw error;
        }
        int idx = 0;
        for (int i = 1; i < n; ++i) {
            if (buf[i].steps > buf[idx].steps) idx = i;
        }
        outDate = buf[idx].date;
        int result = buf[idx].steps;
        delete[] buf;
        return result;
    }

    int MaxStepsTotal(Date& outDate) const {
        if (size == 0) {
            string error("no records");
            throw error;
        }
        DayTotal* buf = new DayTotal[size];
        int n = aggregateByDay(buf, 0, 0, 0);
        int idx = 0;
        for (int i = 1; i < n; ++i) {
            if (buf[i].steps > buf[idx].steps) idx = i;
        }
        outDate = buf[idx].date;
        int result = buf[idx].steps;
        delete[] buf;
        return result;
    }

    void SaveToFile(const string& filename) const {
        std::ofstream f(filename);
        if (!f.is_open()) {
            string error("cant open file for writing");
            throw error;
        }
        f << (startDateSet ? 1 : 0) << '\n';
        if (startDateSet) {
            f << startDate.day << ' ' << startDate.month << ' ' << startDate.year << '\n';
        }
        f << size << '\n';
        for (int i = 0; i < size; ++i) {
            const StepRecord& r = records[i];
            f << r.date.day << ' ' << r.date.month << ' ' << r.date.year << ' '
              << r.startHour << ' ' << r.startMin << ' '
              << r.endHour << ' ' << r.endMin << ' '
              << r.steps << '\n';
        }
    }

    void LoadFromFile(const string& filename) {
        std::ifstream f(filename);
        if (!f.is_open()) {
            string error("cant open file for reading");
            throw error;
        }
        int flag;
        if (!(f >> flag)) {
            string error("cant read info from file");
            throw error;
        }
        if (flag != 0) {
            Date sd;
            if (!(f >> sd.day >> sd.month >> sd.year)) {
                string error("cant read info from file");
                throw error;
            }
            if (!startDateSet) {
                if (!isValidDate(sd)) {
                    string error("invalid start date!");
                    throw error;
                }
                startDate = sd;
                startDateSet = true;
            }
        }
        int n;
        if (!(f >> n) || n < 0) {
            string error("cant read info from file");
            throw error;
        }

        for (int i = 0; i < n; ++i) {
            int day, month, year, sH, sM, eH, eM, steps;
            if (!(f >> day >> month >> year >> sH >> sM >> eH >> eM >> steps)) {
                string error("cant read info from file");
                throw error;
            }
            AddRecord(day, month, year, sH, sM, eH, eM, steps);
        }
    }
};

static void printDate(const Date& d) {
    cout << d.day << '.' << d.month << '.' << d.year;
}

int main() {
    StepCounter pedometer;
    try {
        pedometer.SetStartDate(1, 1, 2026);

        Date sd = pedometer.GetStartDate();
        cout << "Start date: ";
        printDate(sd);
        cout << '\n';

        pedometer.AddRecord( 1, 1, 2026,  8,  0,  9,  0, 4500);
        pedometer.AddRecord( 1, 1, 2026, 19,  0, 20,  0, 2000);
        pedometer.AddRecord( 2, 1, 2026,  7, 30,  8, 15, 3200);
        pedometer.AddRecord(15, 1, 2026, 18,  0, 19,  0, 6000);
        pedometer.AddRecord(20, 2, 2026, 10,  0, 11,  0, 5000);
        pedometer.AddRecord( 8, 3, 2026, 12,  0, 13, 30, 7500);

        cout << "Records count: " << pedometer.GetRecordsCount() << '\n';

        StepRecord r = pedometer.GetRecord(3);
        cout << "Record #3: ";
        printDate(r.date);
        cout << "  " << r.startHour << ':' << r.startMin
             << " - " << r.endHour << ':' << r.endMin
             << "  steps = " << r.steps << '\n';

        cout << "Average for 01.2026: " << pedometer.AvgStepsMonth(1, 2026) << '\n';
        cout << "Average total: " << pedometer.AvgStepsTotal() << '\n';
        cout << "Average on Wed: " << pedometer.AvgStepsByWeekday(3) << '\n';

        Date dMax;
        int maxAll = pedometer.MaxStepsTotal(dMax);
        cout << "Max total: " << maxAll << " on ";
        printDate(dMax);
        cout << '\n';

        Date dMaxJan;
        int maxJan = pedometer.MaxStepsInMonth(1, 2026, dMaxJan);
        cout << "Max in 01.2026: " << maxJan << " on ";
        printDate(dMaxJan);
        cout << '\n';

        pedometer.SaveToFile("history.txt");
        cout << "History saved.\n";

        StepCounter loaded;
        loaded.LoadFromFile("history.txt");
        cout << "Loaded records: " << loaded.GetRecordsCount() << '\n';
        cout << "Loaded average total: " << loaded.AvgStepsTotal() << '\n';

    } catch (const string& e) {
        std::cerr << e << '\n';
    }
}