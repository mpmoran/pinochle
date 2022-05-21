/*
 * notes and code for learning pinochle
 */ 
#include <stdio.h>
#include <stdlib.h>

#define PROJECT_NAME "pinochle"

const int PACK_CARD_COUNT = 48;

enum rank {
    ace,
    ten,
    king,
    queen,
    jack,
    nine
};
// "counter" cards are ALWAYS worth points.
const enum rank COUNTERS[] = {
    ace,
    ten,
    king
};

enum suit {
    clubs,
    diamonds,
    hearts,
    spades
};

struct card {
    enum suit suit;
    enum rank rank;
};

void card_init(struct card *card, enum rank rank, enum suit suit)
{
    card->suit = suit;
    card->rank = rank;
}

int main(int argc, char **argv) {
    printf("Hello.\n");
    if(argc != 1) {
        printf("%s takes no arguments.\n", argv[0]);
        return 1;
    }
    printf("This is project %s.\n", PROJECT_NAME);

    struct card *first_card = malloc(sizeof(struct card));
    printf("I think we got a card.\n");
    card_init(first_card, ace, clubs);
    printf("The first card has rank %d and suit %d.\n", first_card->rank, first_card->suit);
    printf("Destroying the first card.\n");
    free(first_card);

    printf("Goodbye.\n");
    return 0;
}
