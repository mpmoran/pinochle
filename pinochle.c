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
const int NUM_CARDS_PER_PLAYER = 12;
const int NUM_CARDS_DEALT_AT_ONCE = 3;
/* sometimes it's 4. */
// const int NUM_CARDS_DEALT_AT_ONCE = 4;

/* TODO think about this -> should i use enums for ranks and suits? */
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
 * these are worthless (i.e., no points). the scoring system
 * should be agreed on before play begins (i.e., the first
 * card is dealt).
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

int card_is_noncounter(struct card *card)
{
    enum rank rank = card->rank;
    for (unsigned long i = 0; i < sizeof(NONCOUNTERS); i++) {
        if (rank == NONCOUNTERS[i]) {
            return 1;
        }
    }
    return 0;
}

struct player {
    int id;
    char *name;
    int is_dealer;
};

void player_init(struct player *player, char *name, int is_dealer)
{
    player->id = rand();  /* random number */
    player->name = name;  /* TODO think about this -> is this bad practice? */
    player->is_dealer = is_dealer;
}

int player_is_dealer(struct player *player)
{
    return player->is_dealer;
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

    printf("===\n");

    struct card *second_card = malloc(sizeof(struct card));
    printf("I think we got another card.\n");
    card_init(second_card, queen, clubs);
    printf("The second card has rank %d and suit %d.\n", second_card->rank, second_card->suit);
    printf("Is the second card a \"noncounter\" card?\n");
    if (card_is_noncounter(second_card) == 1) {
        printf("Yes.\n");
    } else {
        printf("No.\n");
    }
    printf("Destroying the second card.\n");
    free(second_card);
    printf("Goodbye.\n");

    printf("===\n");

    /* create a player who is a dealer */
    srand(1);  /* TODO do something about this. */
    char *player1_name = "yo-yo mendez";
    int player1_is_dealer = 1;
    printf("Creating the first player.\n");
    struct player *player1 = malloc(sizeof(struct player));
    player_init(player1, player1_name, player1_is_dealer);
    printf("The first player's id is %d and name is %s.\n", player1->id, player1->name);
    printf("Is the first player a dealer?\n");
    if (player_is_dealer(player1) == 1) {
        printf("Yes.\n");
    } else {
        printf("No.\n");
    }
    printf("Ummm . . . retiring the first player.\n");
    free(player1);

    printf("===\n");

    /* create a player who is not a dealer */
    char *player2_name = "silvio dante";
    int player2_is_dealer = 0;
    printf("Creating the second player.\n");
    struct player *player2 = malloc(sizeof(struct player));
    player_init(player2, player2_name, player2_is_dealer);
    printf("The second player's id is %d and name is %s.\n", player2->id, player2->name);
    printf("Is the second player a dealer?\n");
    if (player_is_dealer(player2) == 1) {
        printf("Yes.\n");
    } else {
        printf("No.\n");
    }
    printf("Ummm . . . retiring the second player.\n");
    free(player2);

    return 0;
}
