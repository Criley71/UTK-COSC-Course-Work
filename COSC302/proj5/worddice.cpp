//Project 5 letter Dice
//Connor Riley
//5/8/23
//Dr. Emrich UTK COSC302
#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<iterator>
#include<queue>
#include<climits>
#include<string>

using namespace std;

/*I used Camille Crumptons videos to get started and help build the classes and know what functions were needed
https://www.youtube.com/watch?v=baCpaf8s_yk&list=PLQFSAfh8OMT6qqWxJlcRrK7poAk5VcmxS 
*/
typedef enum {SOURCE, SINK, DICE, WORD} Node_Type;
//The Node class will either be the Source, a dice, a letter, or the Sink.
//they have an adjacency matrix of edges that will be what they are connected to as the source will be connected
//to the dice, the dice to letters that are possible, letters to the sink. Letters will also be connected to the dice 
//the backEdge will be the edge that it took to reach the node. visited will be used during bfs.
//the bool letter checks if it is a letter and the letters vector is size 26, where each corresponds to if it is that letter
class Node{
    public:
    Node();
    Node(int, Node_Type, string);
    bool letter;
    int id;
    Node_Type type;
    vector<bool> letters;
    bool visited;
    vector<class Edge*> adjMatrix;
    class Edge* backedge; 
};
/*The edge class has a constructor that takes the to and from node and if it is a reverse edge. this will
determine if the original and residual values are 1 or 0.The set reverse function takes an edge and sets it as the
reverse
*/
class Edge{
    public:
    Edge();
    Edge(Node* toParam, Node* fromParam, bool isReverse);
    Edge* reverseEdge;
    Node* to;
    Node* from;
    int original;
    int residual;
    void setReverse(Edge* reverse);
};
/*the graph class has a constructor that takes a char* which will be a command line argument for the dice file, 
and a string which will be the word we are checking if we can spell. The source Node is the source and the sink Node is the sink/
the nodes vector will have the 0 index as the source and the last index being the sink. so the dice are from index 1-diceCount variable
which is the amount of dice entered. then each letter is added into the nodes graph then finally the sink.
The addDice function takes in the dice letters and ID and will connect the dice to the source when added.
the addWord function takes in the word and the ID and will connect them to possible dice, as well as connect the dice to them and connect 
each letter to the sink. The can spell function checks if bfs is possible and will output the dice order if it can be spelled. reverseFlow
function swaps the original and residual values of an edge and its reverse edge.
*/
class Graph{
    public:
    Graph();
    Graph(char* dice, string words);
    Node* source;
    Node* sink;
    vector<Node*> nodes;
    int diceCount;
    string word;
    void addDice(string dice, int id);
    void addWord(string word, int &id);
    bool bfs();
    bool canSpell();
    void reverseFlow(Edge* edge);
};

Node::Node(){}
//Node constructor that sets the ID, the type, and word/letter. sink and Source will leave the wordparam as an empty string
//set visited to false by default, the backEdge to null and go through and set the letters vector to true depending on the letters it contains
Node::Node(int idParam, Node_Type typeParam, string wordParam){
    id = idParam;
    type = typeParam;
    visited = false;
    backedge = nullptr;
    letters = vector<bool>(26, false);
    for(int i = 0; i < wordParam.size(); i++){
        letters[wordParam[i] - 65] = true;
    }
    
}

Edge::Edge(){}
/*The edge constructor takes the to node and from node and will set the residual and original values depending on
if it is a reverse edge
*/
Edge::Edge(Node* toParam, Node* fromParam, bool isReverse){
    to = toParam;
    from = fromParam;
    if(!isReverse){
        original = 1;
        residual = 0;
    }else{
        original = 0;
        residual = 1;
    }
}
//simple function that sets an edge as the reverse edge
void Edge::setReverse(Edge* reverse){
    this->reverseEdge = reverse;
    reverse->reverseEdge = this;
}

Graph::Graph(){}
/*The graph constructor starts by making the source Node and having the dice count set to 1
as the source shifts the index. We then open the dice file and call the addDice function  for each line
We then make the word node equal to the dicecount as that will be the index the words start at. we call the add
word function then make the sink node connected to the word nodes letters. we update the adjacency matrices and call the canSpell function
to determine if we can spell the word.
*/
Graph::Graph(char* diceFile, string words){
    diceCount = 1;
    source = new Node(diceCount, SOURCE, "");
    nodes.push_back(source);
    

    ifstream finDice;
    finDice.open(diceFile);
    if(!finDice.is_open()){
        perror("Error: ");
    }
    string diceString;
    while(finDice >> diceString){
        addDice(diceString, diceCount);
        diceCount++;
    }
    finDice.close();

    int wordNode = diceCount;
    addWord(words, wordNode);

    sink = new Node(wordNode+words.length(), SINK, "");
    for(int i = diceCount; i < nodes.size(); i++){
        Edge* wordSink = new Edge(sink, nodes[i], false);
        Edge* wordSinkReverse = new Edge(nodes[i], sink, true);
        wordSink->setReverse(wordSinkReverse);
        nodes[i]->adjMatrix.push_back(wordSink);
    }
    nodes.push_back(sink);

    if(canSpell() == true){
        cout << words << "\n";
    }else{
        cout << "Cannot spell " << words << "\n";
    }
}
/*This function takes in the dice letters and id and makes the dice node, and connects the source to the dice
as well as make the reverse edges. we then add the dice nodes to the nodes vector*/
void Graph::addDice(string dice, int id){
    Node* d = new Node(diceCount, DICE, dice);
    Edge* sourceDice = new Edge(d, source, false);
    Edge* sourceDiceReverse = new Edge(source, d, true);
    sourceDice->setReverse(sourceDiceReverse);
    source->adjMatrix.push_back(sourceDice);
    nodes.push_back(d);
}
/*the addword function adds the word to the graph and connects the dice nodes to the letters and sets the reverse edges
we check if the word node has equal letters to a dice node and if so we make an edge between them. we then push the letters into
the nodes vector*/
void Graph::addWord(string word, int &id){
    int wordId = id;
    for(int i = 0; i < word.length(); i++){
        string letterItr(1, word[i]);
        Node* wordNode = new Node(wordId, WORD, letterItr);
        wordId++;
        for(int j = 0; j < diceCount; j++){
        if(nodes[j]->letters[word[i]-65] == true && nodes[j]->type == DICE){
            Edge* diceWord = new Edge(wordNode, nodes[j], false);
            Edge* diceWordReverse = new Edge(nodes[j], wordNode, true);
            diceWord->setReverse(diceWordReverse);
            nodes[j]->adjMatrix.push_back(diceWord);
            wordNode->adjMatrix.push_back(diceWordReverse);
          }
        }
        nodes.push_back(wordNode);
    }
    
}
/*the bfs function performs bfs on the nodes vector. it starts by resetting all the backedges and them not being visited.
We then start at the source and push in the adjacent nodes.*/
bool Graph::bfs(){
    for(int i = 0; i < nodes.size(); i++){
        nodes[i]->backedge = nullptr;
        nodes[i]->visited = false;
    }
    queue<Node*> q;
    q.push(nodes[0]);

    while(!q.empty()){
        Node* current = q.front();
        q.pop();

        if(current->type == SINK){
            return true;
        }

        for(int i = 0; i < current->adjMatrix.size(); i++){
            Edge* e = current->adjMatrix[i];
            Node* n = e->to;

            if(e->original == 1 && n->visited == false){
                n->visited = true;
                n->backedge = e->reverseEdge;
                q.push(n);
            }
        }
    }
    return false;
}

/*the canSpell function uses bfs to determine if it can spell the word. if so then it prints the 
order of dice used to spell the word*/
bool Graph::canSpell(){
    int totalDice = diceCount-1; //dice count includes source so remove
    int wordStart = nodes.size() - diceCount;
    vector<int> diceLetters(totalDice, -1);

    while(bfs()){
        Edge* current = nodes[nodes.size() - 1]->backedge; //start at sinks backedge
        while(current != nullptr){
            if(current->to->type == WORD){
                int idWord = current->to->id;
                int idDice = current->to->backedge->to->id; //may need to be backEdge
                diceLetters[idDice-1] = idWord-wordStart;
            }
            reverseFlow(current);
            current = current->to->backedge;
        }
    }
    for(int i = diceCount; i < nodes.size()-1; i++){
        Node* currentNode = nodes[i];
        for(int j = 0; j < currentNode->adjMatrix.size(); j++){
            if(currentNode->adjMatrix[j]->to->type == SINK){
                if(currentNode->adjMatrix[j]->residual == 0){
                    return false;
                }
            }
        }
    }

    vector<int> spellOrder;
    for(int i = 0; i < diceLetters.size(); i++){
        int min = INT_MAX;
        int index = -1;
        for(unsigned int j = 0; j < diceLetters.size(); j++){
            if(diceLetters[j] < min && diceLetters[j] != -1){
                min = diceLetters[j];
                index = j;
            }
        }
        if(index != -1){
            diceLetters[index] = INT_MAX;
            spellOrder.push_back(index);
        }
    }
    for(int i = 0; i < spellOrder.size(); i++){
        cout << spellOrder[i];
        if(i == spellOrder.size()-1){
            cout << ": ";
        }else{
            cout << ",";
        }
    }
    return true;
}

//swaps the original and residual of an edge and its reverse edge
void Graph::reverseFlow(Edge* edge){
    if(edge->original == 1){
        edge->original = 0;
        edge->residual = 1;
        edge->reverseEdge->original = 1;
        edge->reverseEdge->residual = 0;
    }else{
        edge->original = 1;
        edge->residual = 0;
        edge->reverseEdge->original = 0;
        edge->reverseEdge->residual = 1;
    }
}


/*The main opens the file and calls the graph constructor*/
int main(int argc, char* argv[]){
    ifstream fin(argv[2]);
    if(!fin.is_open()){
        perror("Error: ");
        return 1;
    }
    string word;
    while(fin >> word){
        Graph graph = Graph(argv[1], word);
    }

    return 0;
}