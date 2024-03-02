#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <csignal>
using namespace std;

bool ctrlCPressed = false;

void signalHandler(int signum) {
    if (signum == SIGINT) {
        ctrlCPressed = true;
    }
}

class Book {
    int BID;
    string title, author, genre;
    bool availability;

public:
    Book() {
        BID = 0;
        availability = true;
    }

    void setBID(int bid) {
        BID = bid;
    }

    int getBID() {
        return BID;
    }

    void setTitle(string titl) {
        title = titl;
    }

    string getTitle() {
        return title;
    }

    void setAuthor(string Author) {
        author = Author;
    }

    string getAuthor() {
        return author;
    }

    void setGenre(string Genre) {
        genre = Genre;
    }

    string getGenre() {
        return genre;
    }

    void setAvailability(bool available) {
        availability = available;
    }

    bool getAvailability() {
        return availability;
    }

    void bookInformation() {
        cout << "Book ID: " << BID << endl;
        cout << "Book Title: " << title << endl;
        cout << "Book Author: " << author << endl;
        cout << "Book Genre: " << genre << endl;
        cout << "The book is " << (availability ? "Available" : "Not Available") << endl;
    }
};

class User {
    int UID;
    vector<Book> booksBorrowed;
    string name;

public:
    void setUID(int uid) {
        UID = uid;
    }

    int getUID() {
        return UID;
    }

    void setName(string Name) {
        name = Name;
    }

    string getName() {
        return name;
    }

    vector<Book> getBooksBorrowed() {
        return booksBorrowed;
    }

    void userInformation() {
        cout << "User Name: " << name << endl;
        cout << "User ID Number: " << UID << endl;
        cout << "Books that are borrowed:" << endl;
        for (Book singleBook : booksBorrowed) {
            singleBook.bookInformation();
        }
    }
};

class Library {
    vector<User> users;
    vector<Book> books;
    int lastUserID;
    int lastBookID;

public:
    Library() {
        lastUserID = 0;
        lastBookID = 0;
    }

    void addBook(Book newBook) {
        newBook.setBID(++lastBookID);
        books.push_back(newBook);
    }

    void removeBook(int rmBookID) {
        books.erase(remove_if(books.begin(), books.end(), [rmBookID](Book boo) { return boo.getBID() == rmBookID; }), books.end());
    }

    void displayBooks() {
        for (Book book : books) {
            book.bookInformation();
        }
    }

    void addUser(User newUser) {
        newUser.setUID(++lastUserID);
        users.push_back(newUser);
    }

    void removeUser(int rmUserID) {
        users.erase(remove_if(users.begin(), users.end(), [rmUserID](User use) { return use.getUID() == rmUserID; }), users.end());
        cout << "User removed successfully!"<< endl;
    }

    void displayUsers() {
        for (User user : users) {
            user.userInformation();
        }
    }

    void borrowBook(User &user, int bookID) {
        for (Book &book : books) {
            if (book.getBID() == bookID && book.getAvailability()) {
                book.setAvailability(false);
                user.getBooksBorrowed().push_back(book);
                cout << "Your book has been borrowed." << endl;
                return;
            }
        }
        cout << "Not available." << endl;
    }

    void returnBook(User &user, int bookID) {
        for (int i = 0; i < user.getBooksBorrowed().size(); i++) {
            if (user.getBooksBorrowed()[i].getBID() == bookID) {
                user.getBooksBorrowed().erase(user.getBooksBorrowed().begin() + i);
                for (Book &book : books) {
                    if (book.getBID() == bookID) {
                        book.setAvailability(true);
                        break;
                    }
                }
                cout << "Returned." << endl;
                return;
            }
        }
        cout << "Not found in user list." << endl;
    }

    void saveToFilebooks() {
        ofstream file("data_books.txt", ios::app);
        if (file.is_open()) {
            for (Book book : books) {
                file << "Book ID: " << book.getBID() << "\nBook title: " << book.getTitle() << "\nBook Author: " << book.getAuthor() << "\nBook Genre: " << book.getGenre() << "\nAvailability: " << book.getAvailability() << endl;
            }
            file.close();
            cout << "Library data saved to file: data_books.txt" << endl;
        } else {
            cout << "Unable to open file for saving." << endl;
        }
    }

    void saveToFileusers() {
        ofstream file("data_users.txt", ios::app);
        if (file.is_open()) {
            for (User user : users) {
                file << "User ID:" << user.getUID() << "\nUser Name: " << user.getName() << endl;
            }
            file.close();
            cout << "Library data saved to file: data_users.txt" << endl;
        } else {
            cout << "Unable to open file for saving." << endl;
        }
    }

    void loadFromFilebooks() {
        ifstream file("data_books.txt");
        if (file.is_open()) {
            int bid, availability;
            string title, author, genre;
            while (getline(file, title) && getline(file, author) && getline(file, genre) && file >> bid >> availability) {
                Book newBook;
                newBook.setBID(bid);
                newBook.setTitle(title);
                newBook.setAuthor(author);
                newBook.setGenre(genre);
                newBook.setAvailability(availability);
                books.push_back(newBook);

                lastBookID = max(lastBookID, bid);
            }
            file.close();
            cout << "Library data loaded from file: data_books.txt" << endl;
        } else {
            cout << "" << endl;
        }
    }

    void loadFromFileusers() {
        ifstream file("data_users.txt");
        if (file.is_open()) {
            int uid;
            string name;
            while (file >> uid) {
                file.ignore();
                getline(file, name);
                User newUser;
                newUser.setUID(uid);
                newUser.setName(name);
                users.push_back(newUser);

                lastUserID = max(lastUserID, uid);
            }
            file.close();
            cout << "Library data loaded from file: data_users.txt" << endl;
        } else {
            cout << "" << endl;
        }
    }

    vector<User> getUsers() {
        return users;
    }

    void saveLastIDs() {
        ofstream file("last_ids.txt");
        if (file.is_open()) {
            file << lastUserID << endl;
            file << lastBookID << endl;
            file.close();
            cout << "Last IDs saved to file: last_ids.txt" << endl;
        } else {
            cout << "" << endl;
        }
    }

    void loadLastIDs() {
        ifstream file("last_ids.txt");
        if (file.is_open()) {
            file >> lastUserID;
            file >> lastBookID;
            file.close();
        } else {
            cout << "" << endl;
        }
    }
};

int main() {
    int num;
    Library objectLibrary;
    signal(SIGINT, signalHandler);
    objectLibrary.loadFromFileusers();
    objectLibrary.loadFromFilebooks();
    objectLibrary.loadLastIDs();

    bool repeatOperation = true;
    try{
    while (repeatOperation && !ctrlCPressed) {
        cout << "1. Add new Book" << endl;
        cout << "2. Remove Book" << endl;
        cout << "3. Display Books" << endl;
        cout << "4. Add User" << endl;
        cout << "5. Remove User" << endl;
        cout << "6. Display Users" << endl;
        cout << "7. Borrow Book" << endl;
        cout << "8. Return Book" << endl;
        cout << "9. Exit" << endl;
        cout << "Choose one of the numbers here: ";
        cin >> num;

        switch (num) {
            case 1: {
                Book newBook;
                string title, author, genre;

                cin.ignore();
                cout << "Type the Title of the book: ";
                getline(cin, title);
                newBook.setTitle(title);

                cout << "Type the name of the Author: ";
                getline(cin, author);
                newBook.setAuthor(author);

                cout << "Type the Genre: ";
                getline(cin, genre);
                newBook.setGenre(genre);

                objectLibrary.addBook(newBook);
                cout << "Book entered successfully." << endl;
                break;
            }
            case 2: {
                int removeBookID;
                cout << "Type the ID of the Book: ";
                cin >> removeBookID;

                objectLibrary.removeBook(removeBookID);
                break;
            }
            case 3:
                objectLibrary.displayBooks();
                break;

            case 4: {
                User newUser;
                string name;

                cin.ignore();
                cout << "Enter your Name: ";
                getline(cin, name);
                newUser.setName(name);

                objectLibrary.addUser(newUser);
                cout << "User entered successfully!" << endl;
                break;
            }

            case 5: {
                int removeUserID;
                cout << "Type the User ID you want to remove: ";
                cin >> removeUserID;

                objectLibrary.removeUser(removeUserID);
                break;
            }
            case 6:
                objectLibrary.displayUsers();
                break;

            case 7: {
                int borrowUserID, borrowBookID;

                cout << "Type User ID: ";
                cin >> borrowUserID;

                User borrowUser;

                for (User u : objectLibrary.getUsers()) {
                    if (u.getUID() == borrowUserID) {
                        borrowUser = u;
                        break;
                    }
                }

                cout << "Type the ID of the Book you want to borrow: ";
                cin >> borrowBookID;

                objectLibrary.borrowBook(borrowUser, borrowBookID);

                break;
            }
            case 8: {
                int returnUserID, returnBookID;

                cout << "Type User ID: ";
                cin >> returnUserID;

                User returnUser;

                for (User u : objectLibrary.getUsers()) {
                    if (u.getUID() == returnUserID) {
                        returnUser = u;
                        break;
                    }
                }

                cout << "Type a Book ID to return: ";
                cin >> returnBookID;

                objectLibrary.returnBook(returnUser, returnBookID);
                break;
            }
            case 9:
                repeatOperation = false;
                objectLibrary.saveToFileusers();
                objectLibrary.saveToFilebooks();
                objectLibrary.saveLastIDs();
                break;

            default:
                cout << "ERROR, Please try again." << endl;
                break;
        }

        }
    }
    catch (...) {
        cout << "An exception occurred. Data will be saved before exiting." << endl;
    }
        objectLibrary.saveToFileusers();
        objectLibrary.saveToFilebooks();
        objectLibrary.saveLastIDs();
    
    return 0;
}