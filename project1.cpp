#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>
#include <map>
#include <set>
#include <chrono>  // system_clock
#include <iomanip> // has std::put_time
using namespace std;
////////////////////////// Utilities //////////////////////////////
int ReadInt(int low, int high)
{
    cout << "\nEnter number in range " << low << " - " << high << ": ";
    int value;
    cin >> value;
    if (low <= value && value <= high)
        return value;
    cout << "ERROR: invalid number...Try again\n";
    return ReadInt(low, high);
}
int ShowReadMenu(const vector<string> &choices)
{
    cout << "\nMenu:\n";
    for (int ch = 0; ch < (int)choices.size(); ++ch)
    {
        cout << "\t" << ch + 1 << ": " << choices[ch] << "\n";
    }
    return ReadInt(1, choices.size());
}
string GetCurrentTimeDate()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}
class Book
{
private:
    string isbn;
    string title;
    string author;
    vector<string> pages;

public:
    void Read()
    {
        string str;

        cout << "Enter ISBN: ";
        cin >> str;
        SetIsbn(str);

        cout << "Enter Title: ";
        cin >> str;
        SetTitle(str);

        cout << "Enter Author Name: ";
        cin >> str;
        SetAuthor(str);

        cout << "Enter How many pages: ";
        int pages_count;
        cin >> pages_count;

        for (int page = 0; page < pages_count; ++page)
        {
            cout << "Enter page # " << page + 1 << ": ";
            cin >> str;
            pages.push_back(str);
        }
    }

    const string &GetAuthor() const
    {
        return author;
    }

    void SetAuthor(const string &author)
    {
        this->author = author;
    }

    const string &GetIsbn() const
    {
        return isbn;
    }

    void SetIsbn(const string &isbn)
    {
        this->isbn = isbn;
    }

    const vector<string> &GetPages() const
    {
        return pages;
    }

    void SetPages(const vector<string> &pages)
    {
        this->pages = pages;
    }

    const string &GetTitle() const
    {
        return title;
    }

    void SetTitle(const string &title)
    {
        this->title = title;
    }

    string ToString()
    {
        return title;
    }
};
class BookReadingSession
{
private:
    // Using a pointer is a bad design chioce here: what if the original book removed now by admin? We access garbage!
    // A more proper way: use book isbn, and later use book manager to find the book
    Book *book; // Note, we did not create this pointer. Someone else should destory
    int current_page;
    string last_access_date;

public:
    BookReadingSession() : BookReadingSession(nullptr)
    {
    }

    BookReadingSession(Book *book) : book(book), current_page(0), last_access_date(GetCurrentTimeDate())
    {
    }
    string ToString()
    {
        ostringstream oss;
        oss << GetBook()->GetTitle() << " Page: " << PageIdxStr() << " - " << GetLastAccessDate();
        return oss.str();
    }

    string GetCurrentPageContent()
    {
        return book->GetPages()[current_page];
    }

    const Book *GetBook() const
    {
        return book; // No set
    }

    const string &GetLastAccessDate() const
    {
        return last_access_date;
    }

    void ResetLastAccessDate()
    { // More convenient
        last_access_date = GetCurrentTimeDate();
    }

    int GetCurrentPage() const
    {
        return current_page;
    }

    string PageIdxStr()
    {
        ostringstream oss;
        oss << GetCurrentPage() + 1 << "/" << book->GetPages().size();
        return oss.str();
    }

    void NextPage()
    {
        if (current_page < (int)book->GetPages().size() - 1)
            current_page++;
    }
    void PreviousPage()
    {
        if (current_page > 0)
            current_page--;
    }
};
class User
{
private:
    string user_name;
    string password;
    string name;
    string email;
    bool is_library_admin;

    vector<BookReadingSession *> reading_sessions;

public:
    // If you have pointers internally: start with canceling copy constructor, so that you discover any bugs due to misuse
    // Provide it based on logic & needs
    User(const User &) = delete;
    void operator=(const User &) = delete;

    User()
    {
        is_library_admin = false;
    }

    ~User()
    {
        cout << "Destructor User\n";
        for (auto &session : reading_sessions)
            delete session;

        reading_sessions.clear();
    }

    string ToString() const
    {
        ostringstream oss;
        oss << "Name: " << user_name;
        if (IsLibraryAdmin())
            oss << " | Admin";
        oss << "\n";
        oss << "Email: " << email << "\n";
        oss << "User name: " << user_name << "\n";
        return oss.str();
    }

    void Read(const string &user_name)
    {
        SetUserName(user_name);

        string str;

        cout << "Enter password: ";
        cin >> str;
        SetPassword(str);

        cout << "Enter name: ";
        cin >> str;
        SetName(str);

        cout << "Enter email: ";
        cin >> str;
        SetEmail(str);
    }

    const string &GetEmail() const
    {
        return email;
    }

    void SetEmail(const string &email)
    {
        this->email = email;
    }

    const string &GetName() const
    {
        return name;
    }

    void SetName(const string &name)
    {
        this->name = name;
    }

    const string &GetPassword() const
    {
        return password;
    }

    void SetPassword(const string &password)
    {
        this->password = password;
    }

    const string &GetUserName() const
    {
        return user_name;
    }

    void SetUserName(const string &userName)
    {
        user_name = userName;
    }

    bool IsLibraryAdmin() const
    {
        return is_library_admin;
    }

    void SetIsLibraryAdmin(bool isLibraryAdmin)
    {
        is_library_admin = isLibraryAdmin;
    }

    const vector<BookReadingSession *> &GetReadingSessions() const
    {
        // Note: Although the return is const vector, but the pointer is not const, so someone can cause problems using setters on pointer
        return reading_sessions; // No set
    }

    BookReadingSession *AddReadingSession(Book *book)
    {
        // We create pointer & later delete it
        BookReadingSession *session = new BookReadingSession(book);
        reading_sessions.push_back(session);
        return session;
    }
};

class UsersManager
{
private:
    User *current_user = nullptr;
    map<string, User *> userame_userobject_map;

    void FreeLoadedData()
    {
        for (auto pair : userame_userobject_map)
        {
            delete pair.second;
        }
        userame_userobject_map.clear();
        current_user = nullptr;
    }

public:
    UsersManager()
    {
    }

    ~UsersManager()
    {
        cout << "Destuctor: UsersManager\n";
        FreeLoadedData();
    }
    UsersManager(const UsersManager &) = delete;
    void operator=(const UsersManager &) = delete;

    void LoadDatabase()
    {
        cout << "UsersManager: LoadDatabase\n";

        FreeLoadedData();
        // Some "Dummy Data" for simplicity
        User *user1 = new User();
        user1->SetUserName("ayushi");
        user1->SetPassword("82");
        user1->SetEmail("2021pgcaca082@nitjsr.ac.in");
        user1->SetIsLibraryAdmin(true);
        user1->SetName("ayushi gupta");
        userame_userobject_map[user1->GetUserName()] = user1;

        User *user2 = new User();
        user2->SetUserName("anmol");
        user2->SetPassword("79");
        user2->SetEmail("2021pgcaca079@nitjsr.ac.in");
        user2->SetIsLibraryAdmin(false);
        user2->SetName("anmol");
        userame_userobject_map[user2->GetUserName()] = user2;
    }

    void AccessSystem()
    {
        int choice = ShowReadMenu({"Login", "Sign Up"});
        if (choice == 1)
            DoLogin();
        else
            DoSignUp();
    }

    void DoLogin()
    {
        LoadDatabase();

        while (true)
        {
            string user_name, pass;
            cout << "Enter user user_name & password: ";
            cin >> user_name >> pass;

            if (!userame_userobject_map.count(user_name))
            {
                cout << "\nInvalid user user_name or password. Try again\n\n";
                continue;
            }
            User *user_exist = userame_userobject_map.find(user_name)->second;

            if (pass != user_exist->GetPassword())
            {
                cout << "\nInvalid user user_name or password. Try again\n\n";
                continue;
            }
            current_user = user_exist;
            break;
        }
    }


    void DoSignUp()
    {
        current_user = new User(); 
        string user_name;
        while (true)
        {
            cout << "Enter user name (No spaces): ";
            cin >> user_name;

            if (userame_userobject_map.count(user_name))
                cout << "Already used. Try again\n";
            else
                break;
        }

        current_user->Read(user_name);
        current_user->SetIsLibraryAdmin(false);

        userame_userobject_map[current_user->GetUserName()] = current_user;
    }

    BookReadingSession *AddReadingSession(Book *book)
    {
        return current_user->AddReadingSession(book);
    }

    const User *GetCurrentUser() const
    {
        return current_user;
    }
};

class BooksManager
{
private:
    map<string, Book *> isbn_to_book_map;

    void FreeLoadedData()
    {
        for (auto pair : isbn_to_book_map)
        {
            delete pair.second;
        }
        isbn_to_book_map.clear();
    }

public:
    BooksManager()
    {
    }

    void LoadDatabase()
    {
        cout << "BooksManager: LoadDatabase\n";
        FreeLoadedData();

        Book *book1 = new Book();
        book1->SetIsbn("00001");
        book1->SetAuthor("E. Balagurusamy");
        book1->SetTitle("CPP By E. Balagurusamy");
        vector<string> pages1 = {"C++ Inheritance :: In C++, inheritance is a process in which one object acquires all the properties and behaviors of its parent object automatically. In such way, you can reuse, extend or modify the attributes and behaviors which are defined in other class. In C++, the class which inherits the members of another class is called derived class and the class whose members are inherited is called base class. The derived class is the specialized class for the base class.  C++ PolymorphismThe term Polymorphism is the combination of poly + morphs which means many forms. It is a greek word. In object-oriented programming, we use 3 main concepts: inheritance, encapsulation, and polymorphism.", "Abstract Classes::   Abstract classes are the way to achieve abstraction in C++. Abstraction in C++ is the process to hide the internal details and showing functionality only. Abstraction can be achieved by two ways:   Abstract class  Interface  Abstract class and interface both can have abstract methods which are necessary for abstraction.", "C++ Namespaces Namespaces in C++ are used to organize too many classes so that it can be easy to handle the application. For accessing the class of a namespace, we need to use namespacename::classname. We can use using keyword so that we don't have to use complete name all the time.", "C++ StringsIn C++, string is an object of std::string class that represents sequence of characters. We can perform many operations on strings such as concatenation, comparison, conversion etc."};
        book1->SetPages(pages1);
        AddBook(book1);

        Book *book2 = new Book();
        book2->SetIsbn("00002");
        book2->SetAuthor("Thomas H. Cormen");
        book2->SetTitle("Intro to Algorithm by Thomas H. Cormen");
        vector<string> pages2 = {"Linked list  In this article, we will see the introduction of linked list. Linked list is a linear data structure that includes a series of connected nodes. Linked list can be defined as the nodes that are randomly stored in the memory. A node in the linked list contains two parts, i.e., first is the data part and second is the address part. The last node of the list contains a pointer to the null. After array, linked list is the second most used data structure. In a linked list, every link contains a connection to another link.", "What is a Stack? A Stack is a linear data structure that follows the LIFO (Last-In-First-Out) principle. Stack has one end, whereas the Queue has two ends (front and rear). It contains only one pointer top pointer pointing to the topmost element of the stack. Whenever an element is added in the stack, it is added on the top of the stack, and the element can be deleted only from the stack. In other words, a stack can be defined as a container in which insertion and deletion can be done from the one end known as the top of the stack.", "Queue 1. A queue can be defined as an ordered list which enables insert operations to be performed at one end called REAR and delete operations to be performed at another end called FRONT.  2. Queue is referred to be as First In First Out list.  3. For example, people waiting in line for a rail ticket form a queue.", "Tree Data Structure We read the linear data structures like an array, linked list, stack and queue in which all the elements are arranged in a sequential manner. The different data structures are used for different kinds of data. Some factors are considered for choosing the data structure:", "A graph can be defined as group of vertices and edges that are used to connect these vertices. A graph can be seen as a cyclic tree, where the vertices (Nodes) maintain any complex relationship among them instead of having parent child relationship "};
        book2->SetPages(pages2);
        AddBook(book2);

        Book *book3 = new Book();
        book3->SetIsbn("00003");
        book3->SetAuthor("Narasimha Karumanchi");
        book3->SetTitle("Tree in DSA");
        vector<string> pages3 = {"Tree Data Structure::  We read the linear data structures like an array, linked list, stack and queue in which all the elements are arranged in a sequential manner. The different data structures are used for different kinds of data.", "Binary Tree:: The Binary tree means that the node can have maximum two children. Here, binary name itself suggests that 'two'; therefore, each node can have either 0, 1 or 2 children.", "AVL Tree::   AVL Tree is invented by GM Adelson - Velsky and EM Landis in 1962. The tree is named AVL in honour of its inventors. AVL Tree can be defined as height balanced binary search tree in which each node is associated with a balance factor which is calculated by subtracting the height of its right sub-tree from that of its left sub-tree. Tree is said to be balanced if balance factor of each node is in between -1 to 1, otherwise, the tree will be unbalanced and need to be balanced.", "B Tree::  B Tree is a specialized m-way tree that can be widely used for disk access. A B-Tree of order m can have at most m-1 keys and m children. One of the main reason of using B tree is its capability to store large number of keys in a single node and large key values by keeping the height of the tree relatively small.", "B+ Tree::  B+ Tree is an extension of B Tree which allows efficient insertion, deletion and search operations. In B Tree, Keys and records both can be stored in the internal as well as leaf nodes. Whereas, in B+ tree, records (data) can only be stored on the leaf nodes while internal nodes can only store the key values.  The leaf nodes of a B+ tree are linked together in the form of a singly linked lists to make the search queries more efficient."};
        book3->SetPages(pages3);
        AddBook(book3);
    }

    BooksManager(const BooksManager &) = delete;
    void operator=(const BooksManager &) = delete;

    ~BooksManager()
    {
        cout << "Destuctor: BooksManager\n";
        FreeLoadedData();
    }

    // CRUD operations
    void AddBook(Book *book)
    {
        isbn_to_book_map[book->GetIsbn()] = book;
    }

    void UpdateBook(string isbn, Book *book)
    {
    }

    void DeleteBook(string isbn)
    {
    }

    Book *GetBook(string isbn)
    {
        return nullptr;
    }

    const map<string, Book *> &GetIsbnToBookMap() const
    {
        return isbn_to_book_map;
    }

    void SetIsbnToBookMap(const map<string, Book *> &isbnToBookMap)
    {
        isbn_to_book_map = isbnToBookMap;
    }
};

class UserView
{
private:
    UsersManager &users_manager;
    BooksManager &books_manager;

public:
    UserView(UsersManager &users_manager, BooksManager &books_manager) : users_manager(users_manager), books_manager(books_manager)
    {
    }

    void Display()
    {
        const User *user = users_manager.GetCurrentUser();
        cout << "\n\nHello " << user->GetName() << " | User View\n";

        vector<string> menu;
        menu.push_back("View Profile");
        menu.push_back("List & Select from My Reading History");
        menu.push_back("List & Select from Available Books");
        menu.push_back("Logout");

        while (true)
        {
            int choice = ShowReadMenu(menu);
            if (choice == 1)
                ViewProfile();
            else if (choice == 2)
                ListReadingHistory();
            else if (choice == 3)
                ListAvailableBooks();
            else
                break;
        }
    }

    void ViewProfile()
    {
        const User *user = users_manager.GetCurrentUser();

        cout << "\n"
             << user->ToString() << "\n";
    }

    void DisplaySession(BookReadingSession *session)
    {
        vector<string> menu;
        menu.push_back("Next Page");
        menu.push_back("Previous Page");
        menu.push_back("Stop Reading");

        while (true)
        {
            cout << "Current Page: " << session->PageIdxStr() << "\n";
            cout << session->GetCurrentPageContent() << "\n";

            int choice = ShowReadMenu(menu);
            if (choice == 1)
                session->NextPage();
            else if (choice == 2)
                session->PreviousPage();
            else
                break;
        }
        session->ResetLastAccessDate();
    }

    void ListReadingHistory()
    {
        int idx = 0;
        const auto &sessions = users_manager.GetCurrentUser()->GetReadingSessions();
        for (auto &session : sessions)
        {
            cout << ++idx << ": " << session->ToString() << "\n";
        }

        if (idx == 0)
            cout << "\nNo history. List books and start having fun\n";
        else
        {
            cout << "\nWhich session to open?: ";
            int choice = ReadInt(1, idx);
            DisplaySession(sessions[choice - 1]);
        }
    }

    void ListAvailableBooks()
    {
        const map<string, Book *> &mp = books_manager.GetIsbnToBookMap();

        cout << "\nOur current book collection:\n";
        int idx = 0;
        for (const auto &pair : mp)
        {
            // As we call ToString: we don't need to worry about book data members future changes
            cout << "\t" << ++idx << " " << pair.second->ToString() << "\n";
        }

        cout << "\nWhich book to read?: ";
        int choice = ReadInt(1, idx); // For simplicity, assume a new book is selected

        idx = 0;
        for (const auto &pair : mp)
        {
            if (++idx == choice)
            {
                BookReadingSession *session = users_manager.AddReadingSession(pair.second);
                DisplaySession(session);
                break;
            }
        }
    }
};

class AdminView
{
private:
    UsersManager &users_manager;
    BooksManager &books_manager;

public:
    AdminView(UsersManager &users_manager, BooksManager &books_manager) : users_manager(users_manager), books_manager(books_manager)
    {
    }

    void Display()
    {
        const User *user = users_manager.GetCurrentUser();

        cout << "\n\nHello " << user->GetName() << " | Admin View\n";
        vector<string> menu;
        menu.push_back("View Profile");
        menu.push_back("Add Book");
        menu.push_back("Logout");

        while (true)
        {
            int choice = ShowReadMenu(menu);
            if (choice == 1)
                ViewProfile();
            else if (choice == 2)
                AddBook();
            else
                break;
        }
    }

    void ViewProfile()
    {
        const User *user = users_manager.GetCurrentUser();

        cout << "\n"
             << user->ToString() << "\n";
    }

    void AddBook()
    {
        Book *book = new Book();
        book->Read();
        books_manager.AddBook(book);
    }
};

class OnlineReaderSystem
{
private:
    BooksManager *books_manager;
    UsersManager *users_manager;

    void LoadDatabase()
    {
        users_manager->LoadDatabase();
        books_manager->LoadDatabase();
    }

public:
    OnlineReaderSystem() : books_manager(new BooksManager()), users_manager(new UsersManager())
    {
    }
    ~OnlineReaderSystem()
    {
        cout << "Destuctor: OnlineReaderSystem\n";

        if (books_manager != nullptr)
        {
            delete books_manager;
            books_manager = nullptr;
        }
        if (users_manager != nullptr)
        {
            delete users_manager;
            users_manager = nullptr;
        }
    }

    void Run()
    { // only public one
        LoadDatabase();

        while (true)
        {
            users_manager->AccessSystem(); // login/signup

            // Let's make for every user, its own viewer class: Remember: Single responsibility principle
            if (users_manager->GetCurrentUser()->IsLibraryAdmin())
            {
                AdminView view(*users_manager, *books_manager);
                view.Display();
            }
            else
            {
                UserView view(*users_manager, *books_manager);
                view.Display();
            }
        }
    }
};

int main()
{

    OnlineReaderSystem site;

    site.Run();

    return 0;
}