/*
 * notes and code for learning pinochle
 *
 * by michael p. moran
 * 
 * SOURCES
 * https://bicyclecards.com/how-to-play/pinochle-2/
 * https://gamerules.com/rules/pinochle-card-game/
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
/* "counter" cards are ALWAYS worth points. */
const enum rank COUNTERS[] = {
    ace,
    ten,
    king
};
/* sometimes there's such a thing as "noncounter" cards.
 * these are worthless (i.e., no points).
 */
const enum rank NONCOUNTERS[] = {
    queen,
    jack,
    nine
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

int card_is_counter(struct card *card)
{
    enum rank rank = card->rank;
    for (unsigned long i = 0; i < sizeof(COUNTERS); i++) {
        if (rank == COUNTERS[i]) {
            return 1;
        }
    }
    return 0;
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
    printf("Is the first card a \"counter\" card?\n");
    if (card_is_counter(first_card) == 1) {
        printf("Yes.\n");
    } else {
        printf("No.\n");
    }
    printf("Destroying the first card.\n");
    free(first_card);

    printf("Goodbye.\n");
    return 0;
}
