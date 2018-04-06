// Weijing Zeng, Anqi Shao, Zixuan Zhang

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class TrieArticles {
public:
    //struct of trie
    struct T_node {
        int count = 0; // count the number of same word
        T_node* next[26]; //pointer to point child
        bool exist = false; //whether exist a word
    };
    
    //initilaize the trie
    void init() {
        node = new T_node();
        node->count = 0;
        node->exist = false;
    }
    
    //insert word into trie
    void insert(string str)
    {
        T_node* newNode = node;
        for (int i = 0; i < str.length(); i++)
        {
            //find root position
            int id = (int)str[i] - (int)'a';
            //if there is no root of the word, new a root
            if (newNode->next[id] == nullptr)
                newNode->next[id] = new T_node();
            //root exist, go through
            newNode = newNode->next[id];
        }
        //flag that complete a new word
        newNode->exist = true;
    }
    
    
    
    //get names from companies.dat file
    void getNames() {
        ifstream inFile("/Users/shaoanqi/Documents/c++ workspace/tril/tril/companies.dat");
        string line;
        
        while (getline(inFile, line)) {
            int len = line.size();
            string name;
            vector<string> allname;
            int i = 0;
            
            while (i < len) {
                if (line[i] != '\t') {
                    if ((line[i] >= 'A' && line[i] <= 'Z') || (line[i] == ' ') || (line[i] >= 'a' && line[i] <= 'z')) {
                        if (line[i] >= 'A'&&line[i] <= 'Z')
                            name.push_back(line[i] + 32);
                        else name.push_back(line[i]);
                        i++;
                    }
                    else
                        i++;
                }
                else {
                    allname.push_back(name);
                    name.clear();
                    i++;
                }
            }
            allname.push_back(name);
            comName.push_back(allname);
        }
    }
    
    //read article and get words
    void read() {
        ifstream inFile("/Users/shaoanqi/Documents/c++ workspace/tril/tril/news.dat");
        string line;
        int i = 0;
        
        while (getline(inFile, line)) {
            string str;
            
            while (line[i] != '.') {
                if (line[i] != ' ') {
                    if (line[i] >= 'A'&&line[i] <= 'Z')
                        str.push_back(line[i] + 32);
                    else
                        str.push_back(line[i]);
                    i++;
                }
                else {
                    // ignore words" a, an, the, and, or, but "
                    if (str == "a" || str == "an" || str == "the" || str == "but") {
                        str.clear();
                        i++;
                        continue;
                    }
                    words.push_back(str);
                    str.clear();
                    i++;
                }
            }
            words.push_back(str);
            break;
        }
    }
    
    //put company names into a trie
    void creatTrie()
    {
        //all company names
        for (int i = 0; i < comName.size(); i++)
            //synonyms
            for (int j = 0; j < comName[i].size(); j++)
                insert((comName[i])[j]);
    }
    
    //compare whether the words in article and company names in trie are same, if same, add count
    void computeCount(string str)
    {
        T_node *newNode = node;
        for (int i = 0; i < str.length(); i++)
        {
            if (newNode->next[((int)str[i] - (int)'a')] == nullptr)
                return;
            else
                newNode = newNode->next[((int)str[i] - (int)'a')];
        }
        //if two words are same, then count add one
        if (newNode->exist) {
            newNode->count++; return;
        }
        else return;
    }
    
    //to get the count of company names
    int getCount(string str)
    {
        T_node *newNode = node;
        for (int i = 0; i < str.length(); i++)
        {
            if (newNode->next[((int)str[i] - (int)'a')] == nullptr)
                return 0;
            else
                newNode = newNode->next[((int)str[i] - (int)'a')];
        }
        if (newNode->exist)
            return newNode->count++;
        else
            return 0;
    }
    
    //compute the relevance of company name
    void relevance()
    {
        for (int i = 0; i < words.size(); i++)
        {
            computeCount(words[i]);     
        }
        for (int i = 0; i < comName.size(); i++) 
        {
            int num = 0;
            for (int j = 0; j < (comName[i]).size(); j++)     
            {
                num = num + getCount((comName[i])[j]);  
            }
            relev.push_back(num);
        }
    }
    
    //output the result
    void print() {
        int size = comName.size();
        float rel = 0, sumR = 0;
        int hit = 0, sumH = 0;
        cout << "Company Name" << "\t" << "Hit Count" << '\t' << "Relevance" << endl;
        for (int i = 0; i < size; i++)
        {
            rel = (relev[i]/words.size()) * 100;
            hit = comName[i].size();
            cout << (comName[i])[0] << "\t" << hit << "\t" << rel <<"%"<< endl;
            sumR += rel;
            sumH += hit;
        }
        cout << "Total" << "\t" << sumH << '\t' << sumR << endl;
        cout << "Total Words" << "\t" << words.size() << endl;
    }	
    
private:
    vector<vector<string>> comName;  //container of all company names
    vector<int> relev;      // relevance of each company
    vector<string> words;  //words in the news article
    T_node* node;
};

int main()
{
    TrieArticles ta;
    ta.getNames();
    ta.read();
    ta.init();
    ta.creatTrie();
    ta.relevance();
    ta.print();
    
}
