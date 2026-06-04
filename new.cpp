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

void writeBits(ofstream &out,string encoded){
    unsigned char byte=0;
    int bitCount=0;
    int padding=0;
    for(char bit : encoded) {
        byte=(byte<<1)|(bit-'0');
        bitCount++;
        if(bitCount==8){
            out.write((char*)&byte,1);
            byte=0;
            bitCount=0;
        }
    }
    if(bitCount>0) {
        padding=8-bitCount;
        byte<<=padding;
        out.write((char*)&byte,1);
    }
    out.put(padding);
}

string readBits(ifstream &in,int size,int padding){
    string bits="";
    unsigned char byte;
    for(int i=0;i<size;i++) {
        in.read((char*)&byte,1);
        for(int j=7;j>=0;j--) {
            bits+=((byte>>j) & 1) ?'1':'0';
        }
    }
    if(padding>0)
        bits.erase(bits.end()-padding,bits.end());
    return bits;
}

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

void writeBits(ofstream &out,string encoded){
    unsigned char byte=0;
    int bitCount=0;
    int padding=0;
    for(char bit : encoded) {
        byte=(byte<<1)|(bit-'0');
        bitCount++;
        if(bitCount==8){
            out.write((char*)&byte,1);
            byte=0;
            bitCount=0;
        }
    }
    if(bitCount>0) {
        padding=8-bitCount;
        byte<<=padding;
        out.write((char*)&byte,1);
    }
    out.put(padding);
}

string readBits(ifstream &in,int size,int padding){
    string bits="";
    unsigned char byte;
    for(int i=0;i<size;i++) {
        in.read((char*)&byte,1);
        for(int j=7;j>=0;j--) {
            bits+=((byte>>j) & 1) ?'1':'0';
        }
    }
    if(padding>0)
        bits.erase(bits.end()-padding,bits.end());
    return bits;
}

void compressFile(){
    string inFile,outFile;
    cout<<"Input file:";
    cin>>inFile;
    cout<<"Output file:";
    cin>>outFile;
    ifstream in(inFile,ios::binary);
    ofstream out(outFile,ios::binary);
    if(!in ||!out){
        cout<<"File error\n";
        return;
    }
    string text((istreambuf_iterator<char>(in)),{});
    unordered_map<char,int>freq;
    for(char c : text)
        freq[c]++;
    node* root = tree(freq);
    unordered_map<char,string> codes;
    generate(root, "", codes);
    string encoded = "";
    for(char c : text)
        encoded += codes[c];
    int n = freq.size();
    out.write((char*)&n,sizeof(n));
    for(auto &x : freq) {
        out.write((char*)&x.first,sizeof(x.first));
        out.write((char*)&x.second,sizeof(x.second));
    }
    writeBits(out, encoded);
    cout<<"Compression done\n";
    delete root;
    in.close();
    out.close();
}

void decompressFile() {
    string inFile,outFile;
    cout<<"Input file:";
    cin>>inFile;
    cout<<"Output file:";
    cin>>outFile;
    ifstream in(inFile,ios::binary);
    ofstream out(outFile,ios::binary);
    if(!in ||!out) {
        cout<<"File error\n";
        return;
    }
    int n;
    in.read((char*)&n,sizeof(n));
    unordered_map<char,int> freq;
    for(int i=0;i<n;i++){
        char c;
        int f;
        in.read((char*)&c,sizeof(c));
        in.read((char*)&f,sizeof(f));
        freq[c]=f;
    }
    node* root=tree(freq);
    vector<char> buffer((istreambuf_iterator<char>(in)),{});
    int padding = buffer.back();
    buffer.pop_back();
    string bits="";
    for(unsigned char byte : buffer) {
        for(int i=7;i>=0;i--)
            bits+=((byte>>i) & 1) ?'1':'0';
    }
    bits.erase(bits.end()-padding,bits.end());
    node* curr=root;
    string result="";
    for(char bit : bits) {
        curr=(bit=='0')?curr->left:curr->right;
        if(!curr->left && !curr->right){
            result+=curr->ch;
            curr=root;
        }
    }
    out<<result;
    cout<<"Decompression done\n";
    delete root;
    in.close();
    out.close();
}

int main(){
    int choice;
    do{
        cout<<"1.Compress\n";
        cout<<"2.Decompress\n";
        cout<<"3.Exit\n";
        cout<<"Choice:";
        cin>>choice;
        switch(choice){
            case 1: compressFile();
            break;
            case 2: decompressFile();
            break;
            case 3: cout<<"exiting..";
            break;
            default: cout<<"Invalid\n";
        }
    } while(choice != 3);
    return 0;
}