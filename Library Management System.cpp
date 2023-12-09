#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <vector>

using namespace std;

// Define a simple User class
class User {
public:
    int userId;
    string username;
    string password;

    User(int id, const string& uname, const string& pwd)
        : userId(id), username(uname), password(pwd) {}
};

// Define a Book class
class Book {
public:
    char title[100];
    char author[100];
    int year;
    char genre[50];
    int userId;  
    bool isIssued;

    // Default constructor
    Book() {}

    // Parameterized constructor
    Book(const char* t, const char* a, int y, const char* g)
        : year(y), userId(-1), isIssued(false) {
        strncpy(title, t, sizeof(title) - 1);
        strncpy(author, a, sizeof(author) - 1);
        strncpy(genre, g, sizeof(genre) - 1);
    }
};

// Define a Library class
class Library {
private:
    vector<Book> books;
    vector<User> users;

    const string adminUsername = "abcd";
    const string adminPassword = "123";

    bool authenticateAdmin();
    bool authenticateUser(int& userId);
    void displayBooks(bool isAdminDisplay);
    void addBookAdmin();
    void addBookUser(int userId);
    void clearInputBuffer();

public:
    Library(); // Constructor to initialize users

    void runLibrarySystem();
};

Library::Library() {
    // Initialize users
    users.push_back(User(1, "user1", "password1"));
    users.push_back(User(2, "user2", "password2"));
    users.push_back(User(3, "user3", "password3"));
    users.push_back(User(4, "user4", "password4"));
    users.push_back(User(5, "user5", "password5"));
}

bool Library::authenticateAdmin() {
    string username, password;

    cout << "Enter Admin Username: ";
    getline(cin, username);

    cout << "Enter Admin Password: ";
    getline(cin, password);

    if (username == adminUsername && password == adminPassword) {
        cout << "Admin login successful.\n";

        time_t startTime = time(NULL);
        while (time(NULL) - startTime < 3) {}

        return true;
    } else {
        cout << "Admin login failed. Access denied.\n";

        // Introduce a delay after entering username and password
        time_t startTime = time(NULL);
        while (time(NULL) - startTime < 3) {}

        return false;
    }
}

bool Library::authenticateUser(int& userId) {
    string username, password;

    cout << "Enter User Username: ";
    getline(cin, username);

    cout << "Enter User Password: ";
    getline(cin, password);

    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].username == username && users[i].password == password) {
            cout << "User login successful.\n";
            userId = users[i].userId;
            return true;
        }
    }

    cout << "User login failed. Access denied.\n";
    return false;
}

void Library::displayBooks(bool isAdminDisplay) {
    ifstream file("library.dat", ios::binary);

    if (!file) {
        cout << "No books available.\n";
        return;
    }

    Book book;

    cout << "=== Existing Books ===\n";

    while (file.read((char*)&book, sizeof(Book))) {
        book.title[sizeof(book.title) - 1] = '\0';
        book.author[sizeof(book.author) - 1] = '\0';
        book.genre[sizeof(book.genre) - 1] = '\0';

        cout << "Title: " << book.title << "\n";
        cout << "Author: " << book.author << "\n";
        cout << "Year: " << book.year << "\n";
        cout << "Genre: " << book.genre << "\n";

        if (isAdminDisplay && book.isIssued) {
            cout << "Issued to User ID: " << book.userId << "\n";
        }

        cout << "\n";
    }

    file.close();
}

void Library::addBookAdmin() {
    Book newBook;

    cout << "Enter book title: ";
    cin.ignore();
    cin.getline(newBook.title, sizeof(newBook.title));

    cout << "Enter author: ";
    cin.getline(newBook.author, sizeof(newBook.author));

    cout << "Enter publication year: ";
    cin >> newBook.year;

    clearInputBuffer();

    cout << "Available genres:\n";
    cout << "1. Fiction\n";
    cout << "2. Non-Fiction\n";
    cout << "3. Mystery\n";
    cout << "4. Sci-Fi\n";
    cout << "5. Romance\n";

    int genreChoice;
    cout << "Enter genre number: ";
    cin >> genreChoice;

    clearInputBuffer();

    switch (genreChoice) {
        case 1:
            strncpy(newBook.genre, "Fiction", sizeof(newBook.genre) - 1);
            break;
        case 2:
            strncpy(newBook.genre, "Non-Fiction", sizeof(newBook.genre) - 1);
            break;
        case 3:
            strncpy(newBook.genre, "Mystery", sizeof(newBook.genre) - 1);
            break;
        case 4:
            strncpy(newBook.genre, "Sci-Fi", sizeof(newBook.genre) - 1);
            break;
        case 5:
            strncpy(newBook.genre, "Romance", sizeof(newBook.genre) - 1);
            break;
        default:
            cout << "Invalid genre choice. Defaulting to Fiction.\n";
            strncpy(newBook.genre, "Fiction", sizeof(newBook.genre) - 1);
    }

    newBook.title[sizeof(newBook.title) - 1] = '\0';
    newBook.author[sizeof(newBook.author) - 1] = '\0';
    newBook.genre[sizeof(newBook.genre) - 1] = '\0';

    ifstream file("library.dat", ios::binary);
    Book existingBook;
    bool bookExists = false;

    while (file.read((char*)&existingBook, sizeof(Book))) {
        if (strcmp(existingBook.title, newBook.title) == 0) {
            cout << "Error: Book with the same title already exists.\n";
            bookExists = true;
            break;
        }
    }

    file.close();

    if (!bookExists) {
        ofstream outFile("library.dat", ios::app | ios::binary);

        if (!outFile) {
            cout << "Error opening file!";
            return;
        }

        outFile.write((char*)&newBook, sizeof(Book));

        outFile.close();
        cout << "Book added successfully.\n";
    }
}

void Library::addBookUser(int userId) {
    Book existingBook;
    cout << "Enter book title: ";
    char title[100];
    cin.ignore();  // Ignore the newline character left in the buffer
    cin.getline(title, sizeof(title));

    fstream file("library.dat", ios::in | ios::out | ios::binary);

    bool bookFound = false;
    int position = 0;

    while (file.read((char*)&existingBook, sizeof(Book))) {
        if (strcmp(existingBook.title, title) == 0) {
            bookFound = true;
            break;
        }
        position += sizeof(Book);
    }

    if (!bookFound) {
        cout << "Error: Book with the given title does not exist.\n";
        file.close();
        return;
    }

    if (existingBook.isIssued) {
        cout << "Error: Book is already issued.\n";
        file.close();
        return;
    }

    existingBook.userId = userId;
    existingBook.isIssued = true;

    file.seekp(position, ios::beg);
    file.write((char*)&existingBook, sizeof(Book));

    file.close();
    cout << "Book issued successfully.\n";
}


void Library::clearInputBuffer() {
    cin.clear();
    while (cin.get() != '\n');
}

void Library::runLibrarySystem() {
    char choice;

    do {
        system("cls");

        cout << "=== Library Management System ===\n";
        cout << "1. Display Existing Books\n";
        cout << "2. User Login\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        clearInputBuffer();  // Clear the input buffer

        switch (choice) {
            case '1':
                cout << "1. Display Existing Books\n";
                displayBooks(false);
                break;
            case '2':
                {
                    int userId;
                    if (authenticateUser(userId)) {
                        char userChoice;
                        do {
                            system("cls");

                            cout << "=== User Menu ===\n";
                            cout << "1. Display Existing Books\n";
                            cout << "2. Issue a Book\n";
                            cout << "3. Logout\n";
                            cout << "Enter your choice: ";
                            cin >> userChoice;

                            clearInputBuffer();  // Clear the input buffer

                            switch (userChoice) {
                                case '1':
                                    cout << "1. Display Existing Books\n";
                                    displayBooks(false);
                                    break;
                                case '2':
                                    addBookUser(userId);
                                    break;
                                case '3':
                                    cout << "Logging out...\n";
                                    break;
                                default:
                                    cout << "Invalid choice. Please try again.";
                            }

                            if (userChoice != '3') {
                                cout << "\nPress Enter to continue...";
                                clearInputBuffer();  // Clear the input buffer
                            }

                        } while (userChoice != '3');
                    }
                }
                break;
            case '3':
                if (authenticateAdmin()) {
                    char adminChoice;
                    do {
                        system("cls");

                        cout << "=== Admin Menu ===\n";
                        cout << "1. Add a Book\n";
                        cout << "2. Display Existing Books\n";
                        cout << "3. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> adminChoice;

                        clearInputBuffer();  // Clear the input buffer

                        switch (adminChoice) {
                            case '1':
                                addBookAdmin();
                                break;
                            case '2':
                                cout << "2. Display Existing Books\n";
                                displayBooks(true);
                                break;
                            case '3':
                                cout << "Logging out...\n";
                                break;
                            default:
                                cout << "Invalid choice. Please try again.";
                        }

                        if (adminChoice != '3') {
                            cout << "\nPress Enter to continue...";
                            clearInputBuffer();  // Clear the input buffer
                        }

                    } while (adminChoice != '3');
                }
                break;
            case '4':
                cout << "Exiting the program. Goodbye!";
                break;
            default:
                cout << "Invalid choice. Please try again.";
        }

        if (choice != '4') {
            cout << "\nPress Enter to continue...";
            clearInputBuffer();  // Clear the input buffer
        }

    } while (choice != '4');
}

int main() {
    Library library;
    library.runLibrarySystem();

    return 0;
}
