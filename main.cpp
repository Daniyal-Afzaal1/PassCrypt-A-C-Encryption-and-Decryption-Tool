#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
using namespace std;

int id=1;

int getNextId() {
    ifstream inFile("pass.txt");  
    string line;
    int lastId = 0;  
             
    while (getline(inFile, line)) {
        if (line.empty()) continue; 
        size_t p1 = line.find("|");

        // make sure "|" was found in the line
        if (p1 >= 0 && p1 < line.length()) {        
            int fileId = stoi(line.substr(0, p1));

         // update lastId if this fileId is bigger
            if (fileId > lastId) 
                lastId = fileId;
        }
    }
    inFile.close();
    // return the next unused ID
    return lastId + 1;
}

void Encryption(){
    string pass;
    int shift;
    srand(time(0)); 

    while(true){
    cout<<"------RULE BOOK------"<<endl;
    cout<<"1. Password should be 8 characters long"<<endl;
    cout<<"2. Password should have one uppercase, lowercase, digit and a symbol"<<endl<<endl;
    cout<<"Enter the Password to be Encrypted: "<<endl;
    getline(cin,pass);
    if(pass.empty()){
        cout<<"You Entered Nothing!, Type Something"<<endl;
        continue;
    }
    if(pass.length()<8){
        cout<<"Password too short, Type atleast 8 characters"<<endl;
        continue;
    }
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSymbol = false;
    for(char c : pass){
            if(isupper(c)) hasUpper = true; 
            else if(islower(c)) hasLower = true;
            else if(isdigit(c)) hasDigit = true;
            else if(ispunct(c)) hasSymbol = true;
        }
        bool valid = true;

        if(!hasUpper){
            cout << "Password must contain at least one uppercase letter" << endl;
            valid = false;
        }
        if(!hasLower){
            cout << "Password must contain at least one lowercase letter" << endl;
            valid = false;
        }
        if(!hasDigit){
            cout << "Password must contain at least one digit" << endl;
            valid = false;
        }
        if(!hasSymbol){
            cout << "Password must contain at least one special symbol (@, #, $, %, etc.)" << endl;
            valid = false;
        }

        if(!valid){ 
            continue;  //tells that the condition is not met so will take again input
        }

        cout << "\nPassword is valid and ready for encryption!" << endl;
        break;
    }

    cout<<"How many shifts so you want: "<<endl;
    cin>>shift;
    
    // Generate random salt
    int salt = rand() % 10000;

    int HashShift = (shift * 13 + salt) % 10000;

    for(int i=0; i<pass.length(); i++){
        pass[i]=32+((pass[i]-32+shift)%95);   //to keep the numbers in range after adding shift
    }
    
    ofstream outFile("pass.txt",ios::app);
    if(outFile.is_open()){
        int NotChID;
        NotChID=id;
        outFile<<id<<"|"<<pass<<"|"<<HashShift<<"|"<<salt<<endl;
        id++;
        outFile.close();
        cout<<"Password Saved Successfully!"<<endl;
        cout<<"Remember your Shift and ID as it will help you decrypt "<<endl;
        cout<<"Shift Key: "<<shift<<" ,ID: "<<NotChID<<endl;
        
    }
    else{
        cout << "Error: Could not open file." << endl;
    }    
}

void Decryption(){
    int Shift,ID;
    while(true){
    cout<<"Enter the Shift Key: "<<endl;
    cin>>Shift;
    if(cin.fail()){
        cout<<"Invalid Input, Write the Correct Shift key!"<<endl;
        cin.clear();
        cin.ignore(10000,'\n');
        continue;
    }
    else{
        break;
    }
}

while(true){
    cout<<"Enter the ID: "<<endl;
    cin>>ID;
    if(cin.fail()){
        cout<<"Invalid Input, Write the Correct ID!"<<endl;
        cin.clear();
        cin.ignore(10000,'\n');
        continue;
    }
    else{
        break;
    }
}

    int CompHash;
    string line;
    bool found = false;

    ifstream inFile("pass.txt", ios::in);
    if(!inFile.is_open()){
        cout<<"Error! Could not Open the File"<<endl;
        return;
    }
    else{
        while(getline(inFile,line)){
            int fileId, fileHash, fileSalt;
            string encryptedPass;

             if(line.empty()) continue;

            // Format: ID|EncryptedPassword|HashShift|Salt
            size_t p1= line.find("|");
            size_t p2= line.find("|",p1+1);
            size_t p3= line.find("|",p2+1);
            

            fileId=stoi(line.substr(0,p1));
            encryptedPass=line.substr(p1+1,p2-p1-1);
            fileHash=stoi(line.substr(p2+1,p3-p2-1));
            fileSalt=stoi(line.substr(p3+1));

            if(fileId==ID){
                found=true;
                CompHash= (Shift * 13 + fileSalt) % 10000;
                if(CompHash==fileHash){
                    for(int i=0; i<encryptedPass.length(); i++){
                        encryptedPass[i] = 32 + ((encryptedPass[i] - 32 - Shift + 95) % 95);
                    }
                    cout<<"Decrypted Password is: "<<encryptedPass<<endl;

                }
                else{
                    cout<<"Wrong Shift, Cant Decrypt."<<endl;
                }
                break;
            }
        }
    }
    if(!found){
            cout<<"Such ID not found in the File"<<endl;
        }
    inFile.close();
}


int main(){
    id = getNextId();

    int choice;
    string input;
    do{
        
        cout<<"---------Menu---------"<<endl;
        cout<<"1. Encrypt a Message "<<endl;
        cout<<"2. Decrypt a Message "<<endl;
        cout<<"3. Exit"<<endl;
        cout<<"----------------------"<<endl<<endl;
        cout<<"Enter your Choice: (1/2/3): "<<endl;
        cin>>input;
        if(input=="1"||input=="2"||input=="3"){
            choice=stoi(input);
        }
        else{
            cout<<"Wrong Choice Select, Select from 1 , 2, 3"<<endl;
            continue;
        }
        if(choice==1){
            cin.ignore();
            Encryption();
        }
        else if(choice==2){
            Decryption();
        }
        else{
            cout<<"Exiting"<<endl;
        }

    }while(choice!=3);

    return 0;

}

