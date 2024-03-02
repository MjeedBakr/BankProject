#include <iostream>
#include <fstream>
#include <vector>
#include<string>
#include<iomanip>

using namespace std;

const string fileName = "Clients.txt";
struct stClient
{
    string accountNumber;
    string PinCode;
    string name;
    string phoneNumber;
    double accountBalance;
    bool markForEdit = false;
};

enum enScreen{ClientsList = 1, AddClient = 2, DeleteClient = 3, UpdateClient = 4, FindClient = 5, Exit = 6};
vector <stClient> vClients;
stClient client;

string readString(string message)
{
    string str;
    cout << message;
    getline(cin >> ws, str);

    return str;
}

double readDouble(string message)
{
    double number;
    cout << message;
    cin >> number;

    while (cin.fail())
    {
        cin.clear();
        cin.ignore(std::numeric_limits < std::streamsize>::max(), '\n');

        cout << "Invalid number, enter a valid one:";
        cin >> number;
    }

    return number;
}

vector<string> splitSentence(string sentence, string delimiter)
{
    string word = "";
    vector<string> vWords;
    int position = 0;

    while ((position = sentence.find(delimiter)) != std::string::npos)
    {
        word = sentence.substr(0, position);
        if (!word.empty())
        {
            vWords.push_back(word);
        }

        sentence.erase(0, position + delimiter.length());

    }

    if (!sentence.empty())
    {
        vWords.push_back(sentence);
    }

    return vWords;
}

stClient convertLineToRecord(string lineData, string delimiter = "#//#")
{
    stClient client;
    vector<string> vClientData;

    vClientData = splitSentence(lineData, delimiter);

    client.accountNumber = vClientData[0];

    client.PinCode = vClientData[1];

    client.name = vClientData[2];

    client.phoneNumber = vClientData[3];

    client.accountBalance = stod(vClientData[4]);

    return client;
}

string converRecordToLine(stClient client, string delimiter = "#//#")
{
    string stClientRecord = "";

    stClientRecord += client.accountNumber + delimiter;
    stClientRecord += client.PinCode + delimiter;
    stClientRecord += client.name + delimiter;
    stClientRecord += client.phoneNumber + delimiter;
    stClientRecord += to_string(client.accountBalance);

    return stClientRecord;
}

vector<stClient> loadClientsDataFromFile(string fileName)
{

    vector <stClient> vClients;

    fstream myFile;
    myFile.open(fileName, ios::in); //read mode

    if (myFile.is_open())
    {
        string line;
        stClient client;

        while (getline(myFile, line))
        {
            client = convertLineToRecord(line);
            vClients.push_back(client);
        }

        myFile.close();
    }

    return vClients;
}

bool findClientByAccountNumber(string accountNumber, vector <stClient> vClients, stClient& client)
{
    for (stClient c : vClients)
    {
        if (c.accountNumber == accountNumber)
        {
            client = c;
            return true;
        }
    }
    return false;

}

stClient readClientData()
{
    stClient client;

    client.accountNumber = readString("Enter Account Number: ");
    while (findClientByAccountNumber(client.accountNumber, vClients, client))
    {
        cout << "Client with [" << client.accountNumber << "] already exists, Enter Another Account Number : ";
        cin >> client.accountNumber;
    }
    client.PinCode = readString("Enter PinCode: ");
    client.name = readString("Enter Name: ");
    client.phoneNumber = readString("Enter Phone: ");
    client.accountBalance = readDouble("Enter AccountBalance: ");

    return client;
}

void printClientCard(stClient client)
{
    cout << "\nThe follwing are the client details:\n";
    cout << "\nAccount Number: " << client.accountNumber;
    cout << "\nPin Code      : " << client.PinCode;
    cout << "\nName          : " << client.name;
    cout << "\nPhone         : " << client.phoneNumber;
    cout << "\nAccount Balance: " << client.accountBalance;
}

void printClientRecord(stClient client)
{
    cout << "| " << setw(15) << left << client.accountNumber;
    cout << "| " << setw(10) << left << client.PinCode;
    cout << "| " << setw(40) << left << client.name;
    cout << "| " << setw(12) << left << client.phoneNumber;
    cout << "| " << setw(12) << left << client.accountBalance;
}

void appendClientDataToFile(string fileName, string lineData)
{
    fstream myFile;

    myFile.open(fileName, ios::out | ios::app);

    if (myFile.is_open())
    {
        myFile << lineData << endl;
        myFile.close();
    }

}

void addClientsDataToFile(vector <stClient> &vClients)
{
    fstream myFile;

    myFile.open(fileName, ios::out);

    string lineData;
    if (myFile.is_open())
    {
        for (stClient& c : vClients)
        {
            lineData = converRecordToLine(c);
            myFile << lineData << endl;
        }
    }
    myFile.close();
    
}

vector <stClient> deleteClient(stClient client)
{
    vector <stClient> newVClients;
    for (stClient c : vClients)
    {
        if (c.accountNumber != client.accountNumber)
        {
            newVClients.push_back(c);
        }
    }
    
    return newVClients;
}

bool markClientForEditByAccountNumber(string accountNumber, vector <stClient>& vClients)
{
    for (stClient& c : vClients)
    {
        if (c.accountNumber == accountNumber)
        {
            c.markForEdit = true;
            return true;
        }
    }

    return false;
}

void updateClientData(stClient& client)
{

    client.PinCode = readString("Enter PinCode: ");
    client.name = readString("Enter Name: ");
    client.phoneNumber = readString("Enter Phone: ");
    client.accountBalance = readDouble("Enter AccountBalance: ");

}

vector <stClient> saveClientsDataToFile(string fileName, vector <stClient> vClients)
{
    fstream myFile;
    myFile.open(fileName, ios::out);//overwrite

    string lineContent;

    if (myFile.is_open())
    {
        for (stClient c : vClients)
        {
            if (c.markForEdit == true)
                updateClientData(c);

            lineContent = converRecordToLine(c);
            myFile << lineContent << endl;
        }

        myFile.close();
    }

    return vClients;
}

void showMainMenu()
{
    cout << "=======================================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "=======================================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Exit.\n";
    cout << "=======================================================\n";
}

void backToMain()
{
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    system("cls");
}

void showClientsList()
{
    cout << "\n\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (stClient client : vClients)
    {
        printClientRecord(client);
        cout << endl;
    }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

void showAddNewClient()
{
    cout << "\n-------------------------------\n";
    cout << "\tAdd New Clients Screen\n";
    cout << "\n-------------------------------\n";
    cout << "Adding New Client:\n\n";

    client = readClientData();
    vClients.push_back(client);
    appendClientDataToFile(fileName, converRecordToLine(client));

    char choice = 'n';
    cout << "\nClient Added Successfully, do you want to add more clients? y/n: ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y')
    {
        showAddNewClient();
    }


}

void showDeleteClient()
{
    cout << "\n--------------------------------------\n";
    cout << "\tDelete Client Screen\n";
    cout << "--------------------------------------\n";

    string accountNumber = readString("Please enter Account Number: ");

    if (findClientByAccountNumber(accountNumber, vClients, client))
    {
        printClientCard(client);

        char choice = 'n';
        cout << "\n\nAre you sure you want to delete this client? y/n: ";
        cin >> choice;

        if (choice == 'y'|| choice == 'Y')
        {
            vClients = deleteClient(client);
            addClientsDataToFile(vClients);
            cout << "\n\nClient Deleted Successfully.\n";
        }
    }
    else
    {
        cout << "Client with Account Number (" << accountNumber << ") is Not Found!";
    }

}

void showUpdateClient()
{
    cout << "\n--------------------------------------\n";
    cout << "\tUpdate Client Info Screen\n";
    cout << "--------------------------------------\n";

    string accountNumber = readString("Please enter Account Number: ");

    if (findClientByAccountNumber(accountNumber, vClients, client))
    {
        printClientCard(client);

        char choice = 'n';
        cout << "\n\nAre you sure you want to update this client? y/n: ";
        cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            markClientForEditByAccountNumber(accountNumber, vClients);
            //update data
            saveClientsDataToFile(fileName, vClients);

            //refresh clients vector
            vClients = loadClientsDataFromFile(fileName);

            cout << "\n\nClient updated Successfully.";
        }
    }
    else
    {
        cout << "Client with Account Number (" << accountNumber << ") is Not Found!";
    }

}

void showFindClient()
{
    cout << "\n--------------------------------------\n";
    cout << "\tFind Client Screen\n";
    cout << "--------------------------------------\n";

    string accountNumber = readString("Please enter Account Number: ");

    if (findClientByAccountNumber(accountNumber, vClients, client))
    {
        printClientCard(client);
    }
    else
    {
        cout << "Client with Account Number (" << accountNumber << ") is Not Found!";
    }
}

void showProgramEnd()
{
    cout << "\n--------------------------------------\n";
    cout << "\tProgram Ends :-)\n";
    cout << "--------------------------------------\n";
}

void mainMenu()
{
    vClients = loadClientsDataFromFile(fileName);
    showMainMenu();

    short choice;
    cout << "Choose what do you want to do? [1 to 6]: ";
    cin >> choice;

    switch (enScreen(choice))
    {
    case ClientsList:
        system("cls");
        showClientsList();
        break;
    case AddClient:
        system("cls");
        showAddNewClient();
        break;
    case DeleteClient:
        system("cls");
        showDeleteClient();
        break;
    case UpdateClient:
        system("cls");
        showUpdateClient();
        break;
    case FindClient:
        system("cls");
        showFindClient();
        break;
    case Exit:
        system("cls");
        showProgramEnd();
        return;
    }

    backToMain();
    mainMenu();
}

void startApp()
{
    mainMenu();
}


int main()
{
    startApp();

    system("pause>0");

    return 0;
}

