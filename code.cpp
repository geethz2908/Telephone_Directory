#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

class Contact 
{
protected:
    string name;
    string phoneNumber;

public:
    Contact(const string& _name, const string& _phoneNumber)
        : name(_name), phoneNumber(_phoneNumber) {}

    virtual void display() const = 0; //cannot create objects from abstract class

    const string& getName() const {
        return name;
    }

    const string& getPhoneNumber() const {
        return phoneNumber;
    }
};

class Person : public Contact 
{
private:
    string address;

public:
    Person(const string& _name, const string& _phoneNumber, const string& _address)
        : Contact(_name, _phoneNumber), address(_address) {}

    void display() const override 
    {  //overides previous display function
        cout << setw(20) << left << name << "phone_number -> " << setw(15) << phoneNumber << "Address -> " << address << endl;
    }
};

class Business : public Contact 
{
private:
    string companyName;

public:
    Business(const string& _name, const string& _phoneNumber, const string& _companyName)
        : Contact(_name, _phoneNumber), companyName(_companyName) {}

    void display() const override {
        cout << setw(20) << left << name << "phone_number -> " << setw(15) << phoneNumber <<"Company-> " << companyName << endl;
    }
};

class TelephoneDirectory 
{
private:
    map<string, Contact*> contacts;

public:
    ~TelephoneDirectory() {
        for (auto& pair : contacts) {
            delete pair.second;
        }
        contacts.clear(); //clears contact map
    }

    void addContact(Contact* contact) 
    {
        contacts[contact->getName()] = contact;
        cout << "Contact added: ";
        contact->display();
    }

    Contact* searchContact(const string& name) {
        auto it = contacts.find(name);
        if (it != contacts.end()) {
            cout << "Contact found: ";
            it->second->display(); //second refers to value part
            return it->second;
        } else {
            cout << "Contact not found for: " << name << endl;
            return nullptr;
        }
    }

    void displayContacts() const {
        if (contacts.empty()) {
            cout << "Telephone Directory is empty." << endl;
        } else {
            cout << "Telephone Directory:" << endl;
            for (const auto& pair : contacts) {
                pair.second->display();
            }
        }
    }

    void deleteContact(const string& name) {
        auto it = contacts.find(name);
        if (it != contacts.end()) {  //Checks if the contact was found in the contacts map.
            cout << "Contact deleted: ";
            it->second->display();
            delete it->second;
            contacts.erase(it);
        } else {
            cout << "Contact not found for deletion: " << name << endl;
        }
    }

    void saveContactsToFile(const string& filename) const {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (const auto& pair : contacts) {
                outFile << pair.second->getName() << "," << pair.second->getPhoneNumber() << "\n";
            }
            cout << "Contacts saved to file: " << filename << endl;
        } else {
            cerr << "Error opening file for writing: " << filename << endl;
        }
    }

    void loadContactsFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                stringstream ss(line);
                string name, phoneNumber;
                getline(ss, name, ',');
                getline(ss, phoneNumber, ',');
                addContact(new Person(name, phoneNumber, "N/A"));
                //addContact(new Business(name,phoneNumber,"N/A"));
            }
            cout << "Contacts loaded from file: " << filename << endl;
        } else {
            cerr << "Error opening file for reading: " << filename << endl;
        }
    }
};

void displayMenu() 
{
    cout << "\n*************  Welcome to Telephone_Directory *************\n"
         << "\n1. Add Person Contact"
         << "\n2. Add Business Contact"
         << "\n3. Search Contact"
         << "\n4. Display Contacts"
         << "\n5. Delete Contact"
         << "\n6. Save Contacts to File"
         << "\n7. Load Contacts from File"
         << "\n8. Quit"
         << endl;
}

int main() 
{
    TelephoneDirectory directory; //declaration of telephonedirectory object

    int choice;
    string name, phoneNumber, address, companyName, filename;

    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) 
        {
            case 1:
                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter phone number: ";
                cin >> phoneNumber;
                cout << "Enter address: ";
                cin.ignore();
                getline(cin, address);
                directory.addContact(new Person(name, phoneNumber, address));
                break;

            case 2:
                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter phone number: ";
                cin >> phoneNumber;
                cout << "Enter company name: ";
                cin.ignore();
                getline(cin, companyName);
                directory.addContact(new Business(name, phoneNumber, companyName));
                break;

            case 3:
                cout << "Enter name to search: ";
                cin.ignore();
                getline(cin, name);
                directory.searchContact(name);
                break;

            case 4:
                directory.displayContacts();
                break;

            case 5:
                cout << "Enter name to delete: ";
                cin.ignore();
                getline(cin, name);
                directory.deleteContact(name);
                break;

            case 6:
                cout << "Enter file name to save contacts: ";
                cin.ignore();
                getline(cin, filename);
                directory.saveContactsToFile(filename);
                break;

            case 7:
                cout << "Enter file name to load contacts from: ";
                cin.ignore();
                getline(cin, filename);
                directory.loadContactsFromFile(filename);
                break;

            case 8:
                cout << "Exiting the program." << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }

    } while (choice != 8);

    return 0;
}
