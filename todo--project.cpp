#include <iostream>
#include <string>
using namespace std;

// ==========================================
// 1. Date Class
// ==========================================
class Date {
private:
    int year, month, day;
public:
    Date(int y = 0, int m = 0, int d = 0) : year(y), month(m), day(d) {}
    int getYear() { return year; }
    int getMonth() { return month; }
    int getDay() { return day; }
    void printDate() { cout << year << "/" << month << "/" << day; }
};

// ==========================================
// 2. Task Class (Base Class)
// ==========================================
class task {
private:
    string title;
    string description;
    bool isDone;
    Date creationDate;
    Date deadline;
    string priority;

public:
    task(string t, string d, Date cd, Date dl, string p);
    virtual ~task();
    virtual void display(Date today);
    void markAsDone();
    string getTitle();
    string getPriority();
    bool getStatus();
    bool isOverdue(Date today);
};

// Implementation of task
task::task(string t, string d, Date cd, Date dl, string p) {
    title = t;
    description = d;
    creationDate = cd;
    deadline = dl;
    priority = p;
    isDone = false;
}

task::~task() {}

void task::display(Date today) {
    cout << "-----------------------" << endl;
    cout << "Title: " << title << endl;
    cout << "Description: " << description << endl;
    cout << "Created: "; creationDate.printDate();
    cout << " | Deadline: "; deadline.printDate();
    if (isOverdue(today)) cout << " [OVERDUE!]";
    cout << "\nPriority: " << priority << endl;
    cout << "Status: " << (isDone ? "Done" : "Not Done") << endl;
}

void task::markAsDone() { isDone = true; }
string task::getTitle() { return title; }
string task::getPriority() { return priority; }
bool task::getStatus() { return isDone; }

bool task::isOverdue(Date today) {
    if (isDone) return false;
    if (deadline.getYear() < today.getYear()) return true;
    if (deadline.getYear() > today.getYear()) return false;
    if (deadline.getMonth() < today.getMonth()) return true;
    if (deadline.getMonth() > today.getMonth()) return false;
    return deadline.getDay() < today.getDay();
}

// ==========================================
// 3. Todo Class (Derived Class)
// ==========================================
class Todo : public task {
public:
    Todo(string t, string d, Date cd, Date dl, string p);
};

// Implementation of Todo
Todo::Todo(string t, string d, Date cd, Date dl, string p) : task(t, d, cd, dl, p) {}

// ==========================================
// 4. TodoList Class (Manager Class)
// ==========================================
class TodoList {
private:
    task* items[100];
    int count;

public:
    TodoList();
    ~TodoList();
    void addTask(task* newTask);
    void removeTask();
    void toggleTaskStatus();
    void displayAllTask(Date today);
    void DoneNodoneTask(Date today);
    void displayOverdueTasks(Date today);
    void sortTasksByPriority(Date today);
};

// Utility for sorting
int getPriorityValue(string p) {
    if (p == "High" || p == "high") return 3;
    if (p == "Medium" || p == "medium") return 2;
    return 1;
}

// Implementation of TodoList
TodoList::TodoList() { count = 0; }

TodoList::~TodoList() {
    for (int i = 0; i < count; i++) delete items[i];
}

void TodoList::addTask(task* newTask) {
    if (count < 100) {
        items[count++] = newTask;
        cout << "Task added." << endl;
    } else cout << "Full!" << endl;
}

void TodoList::removeTask() {
    if (count == 0) return;
    cout << "Enter title to remove: ";
    string t; getline(cin, t);
    for (int i = 0; i < count; i++) {
        if (items[i]->getTitle() == t) {
            delete items[i];
            for (int j = i; j < count - 1; j++) items[j] = items[j+1];
            count--;
            cout << "Removed." << endl;
            return;
        }
    }
}

void TodoList::toggleTaskStatus() {
    cout << "Enter title to change status: ";
    string t; getline(cin, t);
    for (int i = 0; i < count; i++) {
        if (items[i]->getTitle() == t) {
            items[i]->markAsDone();
            cout << "Updated." << endl;
            return;
        }
    }
}

void TodoList::displayAllTask(Date today) {
    if (count == 0) { cout << "No tasks." << endl; return; }
    for (int i = 0; i < count; i++) items[i]->display(today);
}

void TodoList::DoneNodoneTask(Date today) {
    cout << "\n--- Done Tasks ---" << endl;
    for (int i = 0; i < count; i++) if (items[i]->getStatus()) items[i]->display(today);
    cout << "\n--- Pending Tasks ---" << endl;
    for (int i = 0; i < count; i++) if (!items[i]->getStatus()) items[i]->display(today);
}

void TodoList::displayOverdueTasks(Date today) {
    cout << "\n--- Overdue Tasks ---" << endl;
    for (int i = 0; i < count; i++) if (items[i]->isOverdue(today)) items[i]->display(today);
}

void TodoList::sortTasksByPriority(Date today) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (getPriorityValue(items[j]->getPriority()) < getPriorityValue(items[j+1]->getPriority())) {
                task* temp = items[j];
                items[j] = items[j+1];
                items[j+1] = temp;
            }
        }
    }
    displayAllTask(today);
}

// ==========================================
// 5. Main Function
// ==========================================
int main() {
    TodoList todoList;
    Date today(1404, 5, 1); // تاریخ فرضی امروز
    int choice;

    while (true) {
        cout << "\n====== Todo List Menu ======" << endl;
        cout << "1. Add Task\n2. Show All Tasks\n3. Show Done/Pending\n4. Show Overdue\n5. Change Status\n6. Remove Task\n7. Exit" << endl;
        cout << "Your choice: ";
        cin >> choice;
        cin.ignore();
        if (choice == 1) {
            string title, desc, priority;
            int cy, cm, cd, dy, dm, dd;

            cout << "Title: "; getline(cin, title);
            cout << "Description: "; getline(cin, desc);
            cout << "Creation Date (Y M D): "; cin >> cy >> cm >> cd;
            cout << "Deadline Date (Y M D): "; cin >> dy >> dm >> dd;
            cin.ignore();

            cout << "Priority (Low/Medium/High): "; getline(cin, priority);

            todoList.addTask(new Todo(title, desc, Date(cy,cm,cd), Date(dy,dm,dd), priority));
        }
        else if (choice == 2) {
            todoList.displayAllTask(today);
        }
        else if (choice == 3) {
            todoList.DoneNodoneTask(today);
        }
        else if (choice == 4) {
            todoList.displayOverdueTasks(today);
        }
        else if (choice == 5) {
            todoList.toggleTaskStatus();
        }
        else if (choice == 6) {
            todoList.removeTask();
        }
        else if (choice == 7) {
            cout << "Goodbye!" << endl;
            break;
        }
        else {
            cout << "Invalid choice!" << endl;
        }
    }
    return 0;
}

