// challenge 8
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <map>
#include <iterator>
using namespace std;
int main(int argc, char *argv[])
{
    string dna, subDNA, prevDNA;
    priority_queue<pair<string, int>, vector<pair<string, int>>, greater<pair<string, int>>> DNApq;
    vector<string> DNA9;
    map<string, int> DNAseq;
    map<string, int>::iterator itr;
    bool insert = false;

    while (cin >> dna)
    {
        
        if (dna.size() < 9)
        {
            cout << "-1\n";
            continue;
        }
        for (int i = 0; i < dna.size() - 8; i++)
        {
            subDNA = dna.substr(i, 9);
            // cout << subDNA << "\n";
            insert = false;
            if (DNAseq.size() == 0)
            {
                //  cout << "size 0: " << subDNA << "\n";
                DNAseq.insert(make_pair(subDNA, 1));
            }
            else
            {
                for (itr = DNAseq.begin(); itr != DNAseq.end(); itr++)
                {
                    if (subDNA == itr->first)
                    {
                        //  cout << "match: " << subDNA << "\n";
                        itr->second = 2;
                        insert == true;
                    }
                }
                if (insert == false)
                {
                    DNAseq.insert(make_pair(subDNA, 1));
                    // cout << "new: " << subDNA << "\n";
                }
            }
        }
        for (itr = DNAseq.begin(); itr != DNAseq.end(); itr++)
        {
            if (itr->second != 1)
            {
                cout << itr->first << "\n";
            }
        }
        DNAseq.clear();
        cout << "-1\n";
    }

    return 0;
}
