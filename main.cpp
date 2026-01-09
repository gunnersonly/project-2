#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// COLORS
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string RESET = "\033[0m";

// GLOBALS
string username;
string fullname;

// STRUCT
struct userInfo {
    string name;
    string userName;
    int password;
};

// FUNCTION DECLARATIONS
void logo();
bool UserRegister(userInfo* u);
void CheckUser(string uname, int pass);

void Users();
void Admins();

bool CreateFile(string f);
bool UpdateFile(string f);
bool AddData(string f);
string ReadFile(string f);

// ADMIN FUNCTIONS
bool DeleteUser(string uname);
void ViewDatabase();
void ViewUserFiles(string uname);

// LOGO
void logo() {
    cout << GREEN << "===== DIGITAL FILE SYSTEM =====\n" << RESET;
}

// MAIN
int main() {
    int choice;
    userInfo user;

    logo();

    do {
        cout << "\n1. Login\n2. Register\n0. Exit\nChoice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int pass;
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> pass;
            CheckUser(username, pass);
            break;
        }
        case 2:
            UserRegister(&user);
            break;
        }
    } while (choice != 0);

    return 0;
}

// ================= USER REGISTER =================
bool UserRegister(userInfo* u) {
    ifstream fin("TypingforeverDb.txt");
    string uname, role, name;
    int pass;

    cout << "Full name: ";
    cin >> u->name;
    cout << "Username: ";
    cin >> u->userName;
    cout << "Password: ";
    cin >> u->password;

    while (fin >> uname >> role >> pass >> name) {
        if (uname == u->userName) {
            cout << RED << "Username already exists!\n" << RESET;
            fin.close();
            return false;
        }
    }
    fin.close();

    ofstream fout("TypingforeverDb.txt", ios::app);
    fout << u->userName << " User " << u->password << " " << u->name << endl;
    fout.close();

    system(("mkdir " + u->userName).c_str());

    cout << GREEN << "Registration successful!\n" << RESET;
    return true;
}

// ================= LOGIN CHECK =================
void CheckUser(string uname, int pass) {
    ifstream fin("TypingforeverDb.txt");
    string u, r;
    int p;
    bool found = false;

    while (fin >> u >> r >> p >> fullname) {
        if (u == uname && p == pass) {
            found = true;
            username = u;
            fin.close();
            if (r == "Admin")
                Admins();
            else
                Users();
            return;
        }
    }

    if (!found)
        cout << RED << "Invalid username or password!\n" << RESET;

    fin.close();
}

// ================= USER MENU =================
void Users() {
    int ch;
    do {
        cout << "\n--- USER MENU ---\n";
        cout << "1. Create File\n";
        cout << "2. Update File\n";
        cout << "3. Add Data to File\n";
        cout << "4. List Files\n";
        cout << "5. Read File\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> ch;

        string f;
        switch (ch) {
        case 1:
            cout << "File name: ";
            cin >> f;
            CreateFile(f);
            break;
        case 2:
            cout << "File name: ";
            cin >> f;
            UpdateFile(f);
            break;
        case 3:
            cout << "File name: ";
            cin >> f;
            AddData(f);
            break;
        case 4:
            system(("ls " + username).c_str());
            break;
        case 5:
            cout << "File name: ";
            cin >> f;
            cout << ReadFile(f) << endl;
            break;
        }
    } while (ch != 0);
}

// ================= FILE FUNCTIONS =================
bool CreateFile(string f) {
    ofstream fout(username + "/" + f + ".txt");
    cout << "Enter data: ";
    cin.ignore();
    string data;
    getline(cin, data);
    fout << data << endl;
    fout.close();
    return true;
}

bool UpdateFile(string f) {
    ofstream fout(username + "/" + f + ".txt");
    cout << "Enter new data: ";
    cin.ignore();
    string data;
    getline(cin, data);
    fout << data << endl;
    fout.close();
    return true;
}

bool AddData(string f) {
    ofstream fout(username + "/" + f + ".txt", ios::app);
    cout << "Add data: ";
    cin.ignore();
    string data;
    getline(cin, data);
    fout << data << endl;
    fout.close();
    return true;
}

string ReadFile(string f) {
    ifstream fin(username + "/" + f + ".txt");
    string line, content;
    while (getline(fin, line))
        content += line + "\n";
    fin.close();
    return content;
}

// ================= ADMIN MENU =================
void Admins() {
    int ch;
    string u;

    do {
        cout << "\n--- ADMIN PANEL ---\n";
        cout << "1. View Database\n";
        cout << "2. View User Files\n";
        cout << "3. Delete User\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> ch;

        switch (ch) {
        case 1:
            ViewDatabase();{
            system("type TypingforeverDb.tx");
            }
            break;
        case 2:
            cout << "Enter username: ";
            cin >> u;
            ViewUserFiles(u);
            break;
        case 3:
            cout << "Enter username to delete: ";
            cin >> u;
            if (DeleteUser(u))
                cout << GREEN << "User deleted.\n" << RESET;
            else
                cout << RED << "User not found.\n" << RESET;
            break;
        }
    } while (ch != 0);
}

// ================= ADMIN FUNCTIONS =================
void ViewDatabase() {
    system("cat TypingforeverDb.txt");
}

void ViewUserFiles(string uname) {
    system(("ls " + uname).c_str());
}

bool DeleteUser(string uname) {
    ifstream fin("TypingforeverDb.txt");
    ofstream temp("temp.txt");

    string u, r, n;
    int p;
    bool found = false;

    while (fin >> u >> r >> p >> n) {
        if (u != uname)
            temp << u << " " << r << " " << p << " " << n << endl;
        else
            found = true;
    }

    fin.close();
    temp.close();

    remove("TypingforeverDb.txt");
    rename("temp.txt", "TypingforeverDb.txt");

    if (found)
        system(("rm -r " + uname).c_str());

    return found;
}
