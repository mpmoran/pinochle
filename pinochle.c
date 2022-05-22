/*
 * notes and code for learning pinochle
 *
 * by michael p. moran
 *
 * SOURCES
 * https://bicyclecards.com/how-to-play/pinochle-2/
 * https://gamerules.com/rules/pinochle-card-game/
 */

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

#define PROJECT_NAME "pinochle"

const int NUM_CARDS_PER_PLAYER = 12;
const int NUM_CARDS_DEALT_AT_ONCE = 3;
/* sometimes it's 4. */
// const int NUM_CARDS_DEALT_AT_ONCE = 4;

/* TODO think about this -> should i use enums for ranks and suits? */
const int NRANK = 6;
enum rank
{
    ace,
    ten,
    king,
    queen,
    jack,
    nine
};
const enum rank RANKS[] = { ace, ten, king, queen, jack, nine };
/* "counter" cards are ALWAYS worth points. */
const enum rank COUNTERS[] = { ace, ten, king };
/* sometimes there's such a thing as "noncounter" cards.
 * these are worthless (i.e., no points). the scoring system
 * should be agreed on before play begins (i.e., the first
 * card is dealt).
 */
const enum rank NONCOUNTERS[] = { queen, jack, nine };

const int NSUIT = 4;
enum suit
{
    clubs,
    diamonds,
    hearts,
    spades
};
const enum suit SUITS[] = { clubs, diamonds, hearts, spades };

struct card
{
    enum suit suit;
    enum rank rank;
};

void
card_init(struct card* card, enum rank rank, enum suit suit)
{
    card->suit = suit;
    card->rank = rank;
}

int
card_is_counter(struct card* card)
{
    enum rank rank = card->rank;
    for (unsigned long i = 0; i < sizeof(COUNTERS); i++) {
        if (rank == COUNTERS[i]) {
            return 1;
        }
    }
    return 0;
}

int
card_is_noncounter(struct card* card)
{
    enum rank rank = card->rank;
    for (unsigned long i = 0; i < sizeof(NONCOUNTERS); i++) {
        if (rank == NONCOUNTERS[i]) {
            return 1;
        }
    }
    return 0;
}

GString*
card_human(struct card* card)
{
    GString* buf = g_string_new("");
    enum rank rank = card->rank;
    if (rank == ace) {
        g_string_append(buf, "ace");
    } else if (rank == ten) {
        g_string_append(buf, "ten");
    } else if (rank == king) {
        g_string_append(buf, "king");
    } else if (rank == queen) {
        g_string_append(buf, "queen");
    } else if (rank == jack) {
        g_string_append(buf, "jack");
    } else if (rank == nine) {
        g_string_append(buf, "nine");
    }

    g_string_append(buf, " of ");

    enum suit suit = card->suit;
    if (suit == clubs) {
        g_string_append(buf, "clubs");
    } else if (suit == diamonds) {
        g_string_append(buf, "diamonds");
    } else if (suit == hearts) {
        g_string_append(buf, "hearts");
    } else if (suit == spades) {
        g_string_append(buf, "spades");
    }

    return buf;
}

void
card_show(struct card* card)
{
    GString* buf = card_human(card);
    printf("%s\n", buf->str);
    g_string_free(buf, FALSE);
}

const int PACK_CARD_COUNT = 48;
struct deck
{
    GList* cards;
    int ncards;
};

struct deck*
deck_new()
{
    struct deck* d = malloc(sizeof(struct deck));
    d->ncards = PACK_CARD_COUNT;
    d->cards = NULL;

    /* for each rank, make a card of each suit */
    for (unsigned long i = 0; i < NRANK; i++) {
        for (unsigned long j = 0; j < NSUIT; j++) {
            struct card* cur = malloc(sizeof(struct card));
            cur->rank = RANKS[i];
            cur->suit = SUITS[j];
            d->cards = g_list_append(d->cards, cur);
        }
    }

    return d;
}

void
deck_free(struct deck *deck)
{
    g_list_free_full(deck->cards, free);
    free(deck);
}

void
deck_show(struct deck* deck)
{
    g_list_foreach(deck->cards, (GFunc)card_show, NULL);
}

struct player
{
    int id;
    char* name;
    int is_dealer;
    GList* hand;
};

void
player_init(struct player* player, char* name, int is_dealer, GList* hand)
{
    player->id = rand(); /* random number */
    player->name = name; /* TODO think about this -> is this bad practice? */
    player->is_dealer = is_dealer;
    player->hand = hand;
}

int
player_is_dealer(struct player* player)
{
    return player->is_dealer;
}

int
main(int argc, char** argv)
{
    printf("Hello.\n");
    if (argc != 1) {
        printf("%s takes no arguments.\n", argv[0]);
        return 1;
    }
    printf("This is project %s.\n", PROJECT_NAME);

    struct card* first_card = malloc(sizeof(struct card));
    printf("I think we got a card.\n");
    card_init(first_card, ace, clubs);
    printf("The first card has rank %d and suit %d.\n",
           first_card->rank,
           first_card->suit);
    printf("Is the first card a \"counter\" card?\n");
    if (card_is_counter(first_card) == 1) {
        printf("Yes.\n");
    } else {
        printf("No.\n");
    }

    printf("===\n");

    struct card* second_card = malloc(sizeof(struct card));
    printf("I think we got another card.\n");
    card_init(second_card, queen, clubs);
    printf("The second card has rank %d and suit %d.\n",
           second_card->rank,
           second_card->suit);
    printf("Is the second card a \"noncounter\" card?\n");
    if (card_is_noncounter(second_card) == 1) {
        printf("Yes.\n");
    } else {
        printf("No.\n");
    }

    printf("===\n");

    /* create a player who is a dealer */
    srand(1); /* TODO do something about this. */
    char* player1_name = "yo-yo mendez";
    int player1_is_dealer = 1;
    printf("Creating the first player.\n");
    struct player* player1 = malloc(sizeof(struct player));
    GList* player1_hand = NULL;
    player_init(player1, player1_name, player1_is_dealer, player1_hand);
    printf("The first player's id is %d and name is %s.\n",
           player1->id,
           player1->name);
    printf("Is the first player a dealer?\n");
    if (player_is_dealer(player1) == 1) {
        printf("Yes.\n");
    } else {
        printf("No.\n");
    }
    player1->hand = g_list_append(player1->hand, first_card);
    GString* player1_card_str = card_human(player1->hand->data);
    printf("The first player's first card is %s.\n", player1_card_str->str);
    printf("Ummm . . . retiring the first player.\n");
    g_string_free(player1_card_str, FALSE);
    g_list_free(player1->hand);
    free(player1);

    printf("===\n");

    /* create a player who is not a dealer */
    char* player2_name = "silvio dante";
    int player2_is_dealer = 0;
    printf("Creating the second player.\n");
    struct player* player2 = malloc(sizeof(struct player));
    GList* player2_hand = NULL;
    player_init(player2, player2_name, player2_is_dealer, player2_hand);
    printf("The second player's id is %d and name is %s.\n",
           player2->id,
           player2->name);
    printf("Is the second player a dealer?\n");
    if (player_is_dealer(player2) == 1) {
        printf("Yes.\n");
    } else {
        printf("No.\n");
    }
    player2->hand = g_list_append(player2->hand, second_card);
    GString* player2_card_str = card_human(player2->hand->data);
    printf("The second player's first card is %s.\n", player2_card_str->str);
    printf("Ummm . . . retiring the second player.\n");
    g_string_free(player2_card_str, FALSE);
    g_list_free(player2->hand);
    free(player2);

    printf("Destroying the first card.\n");
    free(first_card);
    printf("Destroying the second card.\n");
    free(second_card);

    printf("===\n");

    printf("Making a deck.\n");
    struct deck* first_deck = deck_new();
    deck_show(first_deck);
    deck_free(first_deck);

    printf("Goodbye.\n");
    return 0;
}
