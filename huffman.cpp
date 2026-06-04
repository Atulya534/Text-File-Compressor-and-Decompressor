#include<bits/stdc++.h>
#include<fstream>
using namespace std;

struct node {
    char ch;
    int freq;
    node *left, *right;

    node(char c, int f) {
        ch = c;
        freq = f;
        left = right = NULL;
    }
};

// ----------- STABLE COMPARATOR -----------
struct compare {
    bool operator()(node* a, node* b) {
        if (a->freq == b->freq)
            return a->ch > b->ch; // tie-break
        return a->freq > b->freq;
    }
};

// ----------- DELETE TREE -----------
void deleteTree(node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

// ----------- BUILD TREE -----------
node* buildTree(map<char,int>& freq) {

    priority_queue<node*, vector<node*>, compare> pq;

    for (auto &x : freq)
        pq.push(new node(x.first, x.second));

    if (pq.empty()) return NULL;

    while (pq.size() > 1) {

        node* l = pq.top(); pq.pop();
        node* r = pq.top(); pq.pop();

        node* parent = new node(min(l->ch, r->ch), l->freq + r->freq);
        parent->left = l;
        parent->right = r;

        pq.push(parent);
    }

    return pq.top();
}

// ----------- GENERATE CODES -----------
void generateCodes(node* root, string s, map<char,string>& mp) {

    if (!root) return;

    if (!root->left && !root->right) {
        mp[root->ch] = (s == "" ? "0" : s);
        return;
    }

    generateCodes(root->left, s + "0", mp);
    generateCodes(root->right, s + "1", mp);
}

// ----------- WRITE BITS -----------
void writeBits(ofstream &out, string encoded) {

    unsigned char byte = 0;
    int bitCount = 0;
    int padding = 0;

    for (char bit : encoded) {
        byte = (byte << 1) | (bit - '0');
        bitCount++;

        if (bitCount == 8) {
            out.write((char*)&byte, 1);
            byte = 0;
            bitCount = 0;
        }
    }

    if (bitCount > 0) {
        padding = 8 - bitCount;
        byte <<= padding;
        out.write((char*)&byte, 1);
    }

    out.put(padding);
}

// ----------- COMPRESS -----------
void compressFile() {

    string inFile, outFile;

    cout << "Input file: ";
    cin >> inFile;

    cout << "Output file: ";
    cin >> outFile;

    ifstream in(inFile, ios::binary);
    ofstream out(outFile, ios::binary);

    if (!in || !out) {
        cout << "File error\n";
        return;
    }

    string text((istreambuf_iterator<char>(in)), {});

    if (text.empty()) {
        cout << "Empty file\n";
        return;
    }

    map<char,int> freq;

    for (char c : text)
        freq[c]++;

    node* root = buildTree(freq);

    map<char,string> codes;
    generateCodes(root, "", codes);

    string encoded = "";

    for (char c : text)
        encoded += codes[c];

    int n = freq.size();
    out.write((char*)&n, sizeof(n));

    for (auto &x : freq) {
        out.write((char*)&x.first, sizeof(x.first));
        out.write((char*)&x.second, sizeof(x.second));
    }

    writeBits(out, encoded);

    cout << "Compression done!\n";

    deleteTree(root);
}

// ----------- DECOMPRESS -----------
void decompressFile() {

    string inFile, outFile;

    cout << "Input file: ";
    cin >> inFile;

    cout << "Output file: ";
    cin >> outFile;

    ifstream in(inFile, ios::binary);
    ofstream out(outFile, ios::binary);

    if (!in || !out) {
        cout << "File error\n";
        return;
    }

    int n;
    in.read((char*)&n, sizeof(n));

    map<char,int> freq;

    for (int i = 0; i < n; i++) {
        char c;
        int f;
        in.read((char*)&c, sizeof(c));
        in.read((char*)&f, sizeof(f));
        freq[c] = f;
    }

    node* root = buildTree(freq);

    // SINGLE CHARACTER CASE
    if (!root->left && !root->right) {
        for (int i = 0; i < root->freq; i++)
            out.put(root->ch);

        cout << "Decompression done!\n";
        deleteTree(root);
        return;
    }

    vector<char> buffer((istreambuf_iterator<char>(in)), {});

    int padding = (unsigned char)buffer.back();
    buffer.pop_back();

    string bits = "";

    for (unsigned char byte : buffer) {
        for (int i = 7; i >= 0; i--)
            bits += ((byte >> i) & 1) ? '1' : '0';
    }

    if (padding > 0)
        bits.erase(bits.end() - padding, bits.end());

    node* curr = root;

    for (char bit : bits) {

        if (bit == '0')
            curr = curr->left;
        else
            curr = curr->right;

        if (!curr->left && !curr->right) {
            out.put(curr->ch);
            curr = root;
        }
    }

    cout << "Decompression done!\n";

    deleteTree(root);
}

// ----------- MAIN -----------
int main() {

    int choice;

    do {
        cout << "\nHUFFMAN COMPRESSOR\n";
        cout << "1. Compress\n";
        cout << "2. Decompress\n";
        cout << "3. Exit\n";
        cout << "Choice: ";

        cin >> choice;

        switch (choice) {
            case 1: compressFile(); break;
            case 2: decompressFile(); break;
            case 3: cout << "Bye\n"; break;
            default: cout << "Invalid\n";
        }

    } while (choice != 3);

    return 0;
}