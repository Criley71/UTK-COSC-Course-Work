#include <iostream>
#include <algorithm>
#include <random>
class CardDeck {
private:
    int *deck;
    int size;
public:
    CardDeck(int = 52);
    CardDeck(const CardDeck &obj);
    ~CardDeck();
    const CardDeck &operator=(const CardDeck &obj);

    int getSize();
    void shuffle();
    void display();
    void deal(int *&playerHand, int size);
};

