#include<bits/stdc++.h>
#include<fstream>
using namespace std;

struct node{
    char ch;
    int freq;
    node* left;
    node* right;
    node(char c,int f){
        ch=c;
        freq=f;
        left=NULL;
        right=NULL;
    }
    node(int f){
        ch='\0';
        freq=f;
        left=NULL;
        right=NULL;
    }
};

struct compare{
    bool operator()(node* a,node* b){
        return a->freq > b->freq;
    }
};

node* tree(unordered_map<char,int>&freq){
    priority_queue<node*, vector<node*>, compare> p;

    for(auto &x:freq){
        p.push(new node(x.first,x.second));
    }

    if(p.empty()) return NULL;
    while(p.size()>1){
        node* left=p.top(); p.pop();
        node* right=p.top(); p.pop();
        node* parent=new node(left->freq + right->freq);
        parent->left=left;
        parent->right=right;
        p.push(parent);
    }
    return p.top();
}

void generate(node* root,string x,unordered_map<char,string>&m){
    if(!root) return;
    if(!root->left && !root->right){
        if(x=="") x="0";  
        m[root->ch]=x;
    }
    generate(root->left,x+"0",m);
    generate(root->right,x+"1",m);
}

string compress(string &text,node* &root,unordered_map<char,string>&m){
    unordered_map<char,int>freq;
    for(char c:text){
        freq[c]++;
    }
    root=tree(freq);
    if(!root) return "";
    generate(root,"",m);
    string encoded;
    encoded.reserve(text.size()*2); 
    for(char c:text){
        encoded+=m[c];
    }
    return encoded;
}

string decompress(unordered_map<string,char>&rev,string &encoded){
    string temp="", decoded="";
    for(char bit:encoded){
        temp += bit;
        if(rev.count(temp)){
            decoded += rev[temp];
            temp="";
        }
    }
    return decoded;
}

void freeTree(node* root){
    if(!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

void compressFile(){
    string inputFile, outputFile;
    cout<<"Enter input file name:";
    cin>>inputFile;
    cout<<"Enter output encoded file name:";
    cin>>outputFile;
    ifstream infile(inputFile);
    ofstream outfile(outputFile);
    if(!infile){
        cout<<"Error opening input file\n";
        return;
    }
    if(!outfile){
        cout<<"Error opening output file\n";
        return;
    }
    string text,line;
    while(getline(infile,line)){
        text+=line +'\n';
    }
    if(text.empty()){
        cout<<"Empty file\n";
        return;
    }
    node* root=NULL;

    unordered_map<char,string> m;
    string encoded = compress(text, root, m);

    outfile <<m.size()<<'\n';
    for(auto &x:m){
        outfile<<(int)x.first<<" "<< x.second<<'\n';
    }

    outfile<<encoded<<'\n';
    cout<<"File compressed successfully\n";

    int beforesize=text.size()*8;
    int aftersize=encoded.size();
    cout<<"Original size:"<<beforesize<<"bits\n";
    cout<<"Compressed size:"<<aftersize<<"bits\n";
    double ratio=(double)aftersize/beforesize*100;
    cout<<"Compression ratio:"<<ratio<<"%\n";

    freeTree(root);
    infile.close();
    outfile.close();
}

void decompressFile(){
    string inputFileName, outputFileName;
    cout<<"Enter encoded file name:";
    cin>>inputFileName;
    cout<<"Enter output decoded file name:";
    cin>>outputFileName;

    ifstream infile(inputFileName);
    ofstream outfile(outputFileName);

    if(!infile){
        cout<<"Error opening file\n";
        return;
    }
    if(!outfile){
        cout<<"Error opening output file\n";
        return;
    }

    int n;
    infile>>n;
    unordered_map<string,char> rev;
    for(int i=0;i<n;i++){
        int ascii;
        string code;
        infile>>ascii>>code;
        rev[code] = (char)ascii;
    }
    string encoded, part;
    while(infile >> part){
        encoded += part;
    }
    if(encoded.empty()){
        cout<<"No encoded data found\n";
        return;
    }
    string decoded = decompress(rev, encoded);
    outfile << decoded;
    cout<<"File decompressed successfully\n";
    infile.close();
    outfile.close();
}

int main(){
    int choice;
    do{
        cout<<"\n HUFFMAN CODING MENU \n";
        cout<<"1. Compress File\n";
        cout<<"2. Decompress File\n";
        cout<<"3. Exit\n";
        cout<<"Enter your choice:";
        cin>>choice;
        switch(choice){
            case 1:compressFile(); break;
            case 2:decompressFile(); break;
            case 3:cout<<"Exiting..\n"; break;
            default:cout<<"Invalid choice\n";
        }
    }while(choice!=3);
    return 0;
}