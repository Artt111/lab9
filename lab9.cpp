#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

// Նոդի կառուցվածքը Huffman-ի ծառի համար
struct Node {
    char ch;     // Նիշը, որը ներկայացնում է տվյալ նոդը
    int freq;    // Նշանակման հաճախականությունը
    Node* left;  // Ձախ երեխան
    Node* right; // Աջ երեխան

    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Հատուկ համեմատման ֆունկցիա առաջնահերթության հերթի համար
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq; 
    }
};

// Ֆունկցիա, որը կառուցում է Huffman-ի ծառը
Node* buildHuffmanTree(const string& text) {
   
    unordered_map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }

    // Ստեղծում ենք առաջնահերթության հերթ, որը կպահպանի Huffman-ի ծառի նոդերը
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

 
    for (const auto& pair : freqMap) {
        minHeap.push(new Node(pair.first, pair.second));
    }

   
    while (minHeap.size() > 1) {
        // Հանում ենք երկու ամենափոքր հաճախականությամբ նոդերը
        Node* left = minHeap.top();
        minHeap.pop();
        Node* right = minHeap.top();
        minHeap.pop();

        // Ստեղծում ենք նոր ներքին նոդ՝ համակցված հաճախականությամբ
        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;

        
        minHeap.push(merged);
    }

   
    return minHeap.top();
}

// Ֆունկցիա, որը գեներացնում է Huffman-ի կոդերը
void generateCodes(Node* root, const string& prefix, unordered_map<char, string>& codeMap) {
    if (!root) return; // Եթե նոդը դատարկ է, վերադարձնում ենք

    // Եթե հասել ենք տերևային նոդի, պահպանում ենք նիշը և նրա կոդը
    if (root->ch != '\0') {
        codeMap[root->ch] = prefix;
    }

    // Շարունակում ենք փնտրել ձախ և աջ ենթածառերում
    generateCodes(root->left, prefix + "0", codeMap);
    generateCodes(root->right, prefix + "1", codeMap);
}

// Ֆունկցիա, որը կոդավորում է տեքստը օգտագործելով գեներացված կոդերը
string huffmanEncode(const string& text, const unordered_map<char, string>& codeMap) {
    string encodedText;
    for (char ch : text) {
        encodedText += codeMap.at(ch); 
    }
    return encodedText;
}

// Ֆունկցիա, որը դեկոդավորում է տեքստը Huffman-ի ծառի միջոցով
string huffmanDecode(const string& encodedText, Node* root) {
    string decodedText;
    Node* currentNode = root;

    for (char bit : encodedText) {
        
        currentNode = (bit == '0') ? currentNode->left : currentNode->right;

        if (currentNode->ch != '\0') {
            decodedText += currentNode->ch;
            currentNode = root; 
        }
    }

    return decodedText;
}

int main() {
    string text = "grigoryan";

    Node* root = buildHuffmanTree(text);

    unordered_map<char, string> codeMap;
    generateCodes(root, "", codeMap);

    cout << "Huffman-ի կոդեր:" << endl;
    for (const auto& pair : codeMap) {
        cout << pair.first << ": " << pair.second << endl;
    }

    string encodedText = huffmanEncode(text, codeMap);
    cout << "Կոդավորված տեքստ: " << encodedText << endl;

    string decodedText = huffmanDecode(encodedText, root);
    cout << "Դեկոդավորված տեքստ: " << decodedText << endl;

    return 0;
}
