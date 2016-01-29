//Author : Nandan Nayak
//Date : 27-Dec-2015

#include "iostream"
#include "stdio.h"
#include "iomanip"
#include "string.h"
using namespace std;

#define DASHES 50 //Count for no. of dashes
#define TX 10 //No. of entries in the Transaction History

//Macros for Actions:
#define SAV_DEP 21 //Deposit into Savings
#define CHK_DEP 22 //Deposit into Checking
#define SAV_WD 31 //Withdrawal from Savings
#define CHK_WD 32 //Withdrawal from checking
#define TX_SC 41 //Fund transfer from savings to checking
#define TX_CS 42 //Fund transfer from checking to savings
#define TX_OUT 431 //Outward wire transfer
#define TX_IN 432 //Inward wire transfer

/*
Description: Stores account number and the amount to be transfered into that account
*/
struct accDetail
{
    int accNumber;
    int amount;
};

/*
Description: Used to record the action, amount and time of the transaction for an account
*/
struct contents
{
    int action;
    int amount;
    time_t t;
};

/*
Description: prints out the dashes
Parameters: n - represents the no. of dashes
*/
void dash(int n)
{
    for(int i=0;i<n;i++)
    {
        cout<<"-";
    }
    cout<<endl;
}

/*
Description: Prints $ on console

Parameters: out - Reference for cout
*/
ostream& usd(ostream& out)
{
    out<<"$"<<flush;
}

/*
Description: Prints out "Option:"

Parameters: none
*/
void Option()
{
    cout<<endl<<"Option : ";
}

/*
Description: This class is used to overload operator<< and use the
             setfill(), setprecision() and setw() functions in a single
             function call.
*/
class manip
{
private:
    int width,precision;
    char ch;
public:
    manip(int,int,int);
    friend ostream& operator<<(ostream&,manip);
};

/*
Description: Overloads operator<<

Parameters:out-reference for cout
           m - object of class manip
*/
ostream& operator<<(ostream& out,manip m)
{
    out<<setw(m.width)<<setprecision(m.precision)<<setfill(m.ch);
    return out;
}


/*
Description: Constructor of class manip

Parameters: w-width
            p-precision
            c-character constant
*/
manip::manip(int w,int p,int c)
{
    width=w;
    precision=p;
    ch=c;
}

/*
Description: Calls manip's constructor and returns an instance of it

Parameters: w-width
            p-precision
            c-character constant
*/
manip set_fill(int w=0,int p=0, char c=' ')
{
    return manip(w,p,c);
}
//--------------------------------------------------------------------

/*
Description: This class is used to maintain the transaction history of an account
*/
class History
{
private:
    int count;
    contents arr[TX];
public:
    History ();
    void push(int,int);
    void dispTx();
    int flushOldTx();
    void sortTx();
};

/*
Description: Sorts the transactions based on time in descending order

Parameters: none
*/
void History::sortTx()
{
    for(int i=0;i<TX-1;i++)
    {
        for(int j=0;j<TX-1-i;j++)
        {
            if(arr[j].t<arr[j+1].t)
            {
                contents temp;
                temp=arr[j];
                arr[j]=arr[j+1];
                arr[j+1]=temp;
            }
        }
    }
}

/*
Description: Displays all the transactions

Parameters:none
*/
void History::dispTx()
{
#define W 40
    sortTx();
    cout<<set_fill(5)<<"Date"<<set_fill(W)<<"Description"<<set_fill(20)<<"Amount"<<endl;
    dash(DASHES+20);
    for(int i=0;i<TX;i++)
    {
        cout<<set_fill(5)<<arr[i].t;
        switch(arr[i].action)
        {
        case SAV_DEP:
            cout<<set_fill(W)<<"Deposited into Savings";
            break;
        case CHK_DEP:
            cout<<set_fill(W)<<"Deposited into Checking";
            break;
        case SAV_WD:
            cout<<set_fill(W)<<"Withdrawn from Savings";
            break;
        case CHK_WD:
            cout<<set_fill(W)<<"Withdrawn from Checking";
            break;
        case TX_SC:
            cout<<set_fill(W)<<"Transferred from Savings to Checking";
            break;
        case TX_CS:
            cout<<set_fill(W)<<"Transferred from Checking to Savings";
            break;
        case TX_OUT:
            cout<<set_fill(W)<<"Outward wire Transfer";
            break;
        case TX_IN:
            cout<<set_fill(W)<<"Inward wire Transfer";
            break;
        default:
            cout<<set_fill(W+5)<<" ";
            break;
        }
        cout<<set_fill(10)<<usd<<arr[i].amount<<endl;
    }
    dash(DASHES+20);
}

/*
Description: Removes the oldest transaction and creates a space for a new entry

Parameters:none
*/
int History::flushOldTx()
{
    int t1_index=0;
    time_t t1_smallest=arr[0].t;
    for(int i=0;i<TX;i++)
    {
        if(arr[i].t<t1_smallest)
        {
            t1_smallest=arr[i].t;
            t1_index=i;
        }
    }
    arr[t1_index].action=0;
    return t1_index;
}

/*
Description:Stores a transaction

Parameters:action-represents one of the macros for actions
           amount-represents the amount in that transaction
*/
void History::push(int action=0, int amount=0)
{
    contents c1;
    time_t* t1=NULL;
    c1.t=time(t1);
    c1.action=action;
    c1.amount=amount;
    if(count<TX)
    {
       arr[count++]=c1;
    }
    else
    {
        int idx=flushOldTx();
        arr[idx]=c1;
    }
}

/*
Description:Constructor for History

Parameters:none
*/
History::History()
{
    for(int i=0;i<TX;i++)
    {
        arr[i].action=0;
        arr[i].amount=0;
        arr[i].t=0;
    }
    count=0;
}



//------------------------------------------------------------------------------------

/*
Description: This class stores all the account related information

*/
class account
{
private:
    int accNo;
    char usrName[20];
    char pswd[20];
    int savings,checking;
    History rec;

public:
    account();
    friend ostream& operator<<(ostream&, account&);
    friend istream& operator>>(istream&, account&);
    void accSummary();
    void accDeposit();
    bool accSearch(char[], char[]);
    char* accName();
    void accWithdraw();
    accDetail accTxFunds();
    bool accSearch(accDetail);
    void TxFail();
    void TxSuccess();
    void dispTx();
};

/*
Description: Calls rec.dispTx to display all transactions of an account

Parameters:none
*/
void account::dispTx()
{
    rec.dispTx();
}

/*
Description:Prints a message of successful transaction

Parameters:none
*/
void account::TxSuccess()
{
    cout<<"Transaction success"<<endl;
}

/*
Description:Prints a message of failed transaction

Parameters:none
*/
void account::TxFail()
{
    cout<<"Insufficient funds"<<endl;
}

/*
Description:Searches for an account and transfers funds into it

Parameters:acc-stores the account details required to facilitate the transfer
*/
bool account::accSearch(accDetail acc)
{
    if(acc.accNumber==accNo)
    {
        checking += acc.amount;
        rec.push(TX_IN,acc.amount);
        return true;
    }
    return false;
}

/*
Description:Displays a Menu for transferring funds and takes appropriate actions

Parameters:none
*/
accDetail account::accTxFunds()
{
    int option,amount,acc=0;
    cout<<endl<<"1.From Savings to Checking"<<endl
        <<"2.From Checking to Savings"<<endl
        <<"3.To Other Account"<<endl;
    Option();
    cin>>option;
    cout<<"Enter amount:";
    cin>>amount;
    switch(option)
    {
    case 1:
        if(amount>savings)
        {
            TxFail();
            break;
        }
        savings -=amount;
        checking += amount;
        TxSuccess();
        rec.push(TX_SC,amount);
        break;
    case 2:
        if(amount>checking)
        {
            TxFail();
            break;
        }
        checking -= amount;
        savings += amount;
        TxSuccess();
        rec.push(TX_CS,amount);
        break;
    case 3:
        if(amount>checking)
        {
            TxFail();
            break;
        }
        checking -=amount;
        cout<<"Enter account no.:";
        cin>>acc;
        rec.push(TX_OUT,amount);
        break;
    default:
        cout<<"Invalid option"<<endl;
        break;
    }
    accDetail a1;
    a1.accNumber = acc;
    a1.amount=amount;
    return a1;
}

/*
Description:Displays a menu for cash withdrawal and takes appropriate actions

Parameters:none
*/
void account::accWithdraw()
{
    int option,amount;
    cout<<"1.Savings"<<endl
        <<"2.Checking"<<endl;
    Option();
    cin>>option;
    cout<<"Enter the amount:";
    cin>>amount;
    switch(option)
    {
    case 1:
        if(amount>savings)
        {
            TxFail();
            break;
        }
        savings-=amount;
        TxSuccess();
        rec.push(SAV_WD,amount);
        break;
    case 2:
        if(amount>checking)
        {
            TxFail();
            break;
        }
        checking-=amount;
        TxSuccess();
        rec.push(CHK_WD,amount);
        break;
    default:
        cout<<"Invalid option"<<endl;
        break;
    }
}

/*
Description:Returns the user name of the account

Parameters:none
*/
char* account::accName()
{
    return usrName;
}

/*
Description:Displays a Menu for cash deposit and takes appropriate action

Parameters:none
*/
void account::accDeposit()
{
    int option,amount;
    cout<<"1.Savings"<<endl
        <<"2.Checking"<<endl;
    Option();
    cin>>option;
    cout<<"Enter the amount:";
    cin>>amount;
    switch(option)
    {
    case 1:
        savings+=amount;
        rec.push(SAV_DEP,amount);
        break;
    case 2:
        checking+=amount;
        rec.push(CHK_DEP,amount);
        break;
    default:
        cout<<"Invalid option"<<endl;
        break;
    }
}

/*
Description:Grants access to an individual's account by verifying the accounts username and password

Parameters:user[]-takes in user name
           ps[]-takes in password
*/
bool account::accSearch(char user[], char ps[])
{
    if((strcmp(user,usrName)==0) and (strcmp(ps,pswd)==0))
    {
        return true;
    }
    return false;
}

/*
Description:Displays the account summary on the console

Parameters:none
*/
void account::accSummary()
{
    cout<<set_fill(20)<<"Balance Summary"<<set_fill(15)<<"Currency"<<set_fill(10)<<"Assets"<<endl;
    dash(DASHES);
    cout<<set_fill(20)<<"Savings account"<<set_fill(15)<<usd<<set_fill(10)<<savings<<endl
        <<set_fill(20)<<"Checking account"<<set_fill(15)<<usd<<set_fill(10)<<checking<<endl;
    dash(DASHES);
    cout<<set_fill(20)<<"Total"<<set_fill(15)<<usd<<set_fill(10)<<checking+savings<<endl;
    dash(DASHES);
}

/*
Description:Overloads >> to take user name, password and account number

Parameters:in-reference to cin
           acc-stores required account details
*/
istream& operator>>(istream& in, account& acc)
{
    cout<<"Set User Name:";
    in>>acc.usrName;
    cout<<"Set Password:";
    in>>acc.pswd;
    cout<<"Set Account No.:";
    in>>acc.accNo;
    return in;
}

/*
Description:Overloads << to display the username and account no. of an account

Parameters:out-reference for cout
           acc-stores the required account info
*/
ostream& operator<<(ostream& out,account& acc)
{
    out<<set_fill(15)<<acc.usrName<<set_fill(3)<<"|"<<set_fill(15)<<acc.accNo<<endl;
}

/*
Description:Constructor for account and call the constructor of rec

Parameters:none
*/
account::account():rec()
{
    accNo=0;
    usrName[20]=pswd[20]={'\0'};
    savings=checking=0;
}

//_________________________________________________________________________*/
/*
Description:Maintains a linked list of accounts
*/
class List
{
private:
    account acc;
    List* next;
public:
    List();
    void insert(List*);
    void displayAll();
    friend ostream& operator<<(ostream&,List*);
    friend istream& operator>>(istream&,List*);
    List* del(int);
    void delAll();
    List* accSearch(char[],char[]);
    void accSummary();
    void accDeposit();
    char* accName();
    void accWithdraw();
    accDetail accTxFunds();
    bool accSearch(accDetail);
    void accTxDisp();
    ~List();
};

/*
Description:Calls acc.dispTx() to display all the recent account transactions

Parameters:none
*/
void List::accTxDisp()
{
    acc.dispTx();
}

/*
Description:Searches for the required account to do the wire transfer

Parameters:a1-stores the required account details
*/
bool List::accSearch(accDetail a1)
{
    List* current=this;
    while(current->acc.accSearch(a1)==0)
    {
        current = current->next;
        if(current==NULL)
        {
            return false;
        }
    }
    return true;
}

/*
Description:Calls acc.accTxFunds() to facilitate funds transfer

Parameters:none
*/
accDetail List::accTxFunds()
{
    return acc.accTxFunds();
}

/*
Description: calls acc.accWithdraw() to facilitate cash withdrawal

Parameters:none
*/
void List::accWithdraw()
{
    acc.accWithdraw();
}

/*
Description: Returns the account holder's name

Parameters:none
*/
char* List::accName()
{
    return acc.accName();
}

/*
Description:Facilitates cash deposits into the account

Parameters:none
*/
void List::accDeposit()
{
    acc.accDeposit();
}

/*
Description:Displays the balance in Savings and Checking accounts

Parameters:none
*/
void List::accSummary()
{
    acc.accSummary();
}

/*
Description:Searches for account with valid username and password in the linked list

Parameters:user[] - STores the username
           paswd[]-Stores the password
*/
List* List::accSearch(char user[],char pswd[])
{
    List* node=this;
    while(node!=NULL)
    {
       if(node->acc.accSearch(user,pswd))
       {
           return node;
       }
       node=node->next;
    }
    return NULL;
}

/*
Description:Deletes all the nodes(accounts) in the Linked List

Parameters:none
*/
void List::delAll()
{
    List* current=this,*nxt=this;
    while(nxt!=NULL)
    {
        nxt=current->next;
        current->next=NULL;
        delete current;
    }
}

/*
Description:Calls the List destructor

Parameters:none
*/
List::~List()
{
    delete next;
}

/*
Description:Deletes a particular node(account) based on its index

Parameters:idx-takes in the index
*/
List* List::del(int idx)
{
    int count=0;
    List* current=this,*prev=this,*temp=NULL,*first=NULL;
    if(idx==0)
    {
        temp=this->next;
        this->next=NULL;//It is necessary to update the next pointer to NULL, else the system crashes
        return temp;
    }
    while(count!=idx)
    {
        prev=current;
        current=current->next;
        count++;
    }
    prev->next=current->next;
    current->next=NULL;
    delete current;
}

/*
Description: Overloads >> to get the account details

Parameters:in-reference for cin
           node-stores the details of an account for that node
*/
istream& operator>>(istream& in,List* node)
{
    cout<<"Enter Account Details:"<<endl;
    in>>node->acc;
    return in;
}

/*
Description:Overloads << to print the account details

Parameters:out-reference for out
           node-has the account details
*/
ostream& operator<<(ostream& out,List* node)
{
    out<<node->acc;
    return out;
}

/*
Description:Displays all the accounts in the linked list

Parameters:none
*/
void List::displayAll()
{
    List* current=this;
    cout<<set_fill(15)<<"Account Holder"<<set_fill(3)<<"|"<<set_fill(15)<<"Account No."<<endl;
    dash(DASHES);
    while(current)
    {
        cout<<current;
        current=current->next;
    }
    dash(DASHES);
}

/*
Description:Inserts a node at the end of the list

Parameters:node-represents the node to be inserted
*/
void List::insert(List* node)
{
    List* current=this;
    while(current->next!=NULL)
    {
        current=current->next;
    }
    current->next=node;
}

/*
Description:Constructor of List, also calls the constructor of account

Parameters:none
*/
List::List():acc()
{
    next=NULL;
}

int main()
{
    int option1,option2;
    List *first=NULL,*node=NULL;
    char usrName[20]={'\0'},pswd[20]={'\0'};
    while(1)
    {
        cout<<endl<<"What would you like to do:"<<endl
        <<"1.Create a new Account"<<endl
        <<"2.Display all Accounts"<<endl
        <<"3.Delete an Account"<<endl
        <<"4.Do Account Transaction:"<<endl
        <<"5.Quit"<<endl;
        Option();
        cin>>option1;
        cout<<endl;
        switch(option1)
        {
        case 1:
            node=new List;
            cin>>node;
            if(first==NULL)
            {
                first=node;
            }
            else
            {
                first->insert(node);
            }
            break;
        case 2:
            cout<<"Displaying all nodes..."<<endl;
            first->displayAll();
            break;
        case 3:
            int index;
            cout<<"Enter the index:";
            cin>>index;
            List* temp;
            if(index==0)
            {
                temp=first->del(index);
                delete first;
                first=temp;
            }
            else
            {
                first->del(index);
            }
            break;
        case 4:
            cout<<"Enter User Name:";
            cin>>usrName;
            cout<<"Enter Password:";
            cin>>pswd;
            if((node=first->accSearch(usrName,pswd))!=NULL)
            {
               while(1)
               {
                   cout<<endl
                       <<"Dear "<<node->accName()<<","<<endl
                       <<"What would you like to do with your account?"<<endl
                       <<"1.View Account Summary"<<endl
                       <<"2.Deposit"<<endl
                       <<"3.Withdraw"<<endl
                       <<"4.Transfer Funds"<<endl
                       <<"5.Transaction History"<<endl
                       <<"6.Log Out from your account"<<endl;
                    Option();
                    cin>>option2;
                    switch(option2)
                    {
                    case 1:
                        node->accSummary();
                        break;
                    case 2:
                        node->accDeposit();
                        break;
                    case 3:
                        node->accWithdraw();
                        break;
                    case 4:
                        accDetail a1;
                        a1=node->accTxFunds();
                        bool accExist;
                        if(a1.accNumber!=0)
                        {
                            accExist=first->accSearch(a1);
                            if(accExist==false)
                            {
                                cout<<"Account "<<a1.accNumber<<" does not exist"<<endl;
                                cout<<"Transaction failed"<<endl;
                            }
                            else
                            {
                                cout<<"Transaction successful"<<endl;
                            }
                        }
                        break;
                    case 5:
                        node->accTxDisp();
                        break;
                    case 6:
                        break;
                    default:
                        break;
                    }
                    if(option2==6)
                    {
                        break;
                    }
               }
            }
            else
            {
               cout<<"Sorry, your user name or password does not match" <<endl;
            }
            break;
        case 5:
            cout<<"Terminating..."<<endl;
            first->delAll();
            first=NULL;//Once you delete all nodes, and then you insert a new node, first node should be NULL.
            return 0;
            break;
        default:
            cout<<"Invalid option"<<endl;
            break;
        }
    }
    return 0;
}
