/*
 * notes and code for learning pinochle
 *
 * by michael p. moran
 *
 * SOURCES
 * https://bicyclecards.com/how-to-play/pinochle-2/
 * https://gamerules.com/rules/pinochle-card-game/
 */

#include <assert.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

#define PROJECT_NAME "pinochle"

const guint32 NUM_CARDS_PER_PLAYER = 12;
const guint32 NUM_CARDS_DEALT_AT_ONCE = 3;
/* sometimes it's 4. */
// const guint32 NUM_CARDS_DEALT_AT_ONCE = 4;

/* disable bidding; TODO implement bidding system */
const guint32 WITH_BIDS = 0;

/* TODO think about this -> should i use enums for ranks and suits? */
const guint32 NRANK = 6;
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
const guint32 NCOUNTERS = 3;
const enum rank COUNTERS[] = { ace, ten, king };
/* sometimes there's such a thing as "noncounter" cards.
 * these are worthless (i.e., no points). the scoring system
 * should be agreed on before play begins (i.e., the first
 * card is dealt).
 */
const guint32 NNONCOUNTERS = 3;
const enum rank NONCOUNTERS[] = { queen, jack, nine };

const guint32 NSUIT = 4;
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

struct card*
card_new(enum rank rank, enum suit suit)
{
    struct card* c = g_slice_new(struct card);
    c->suit = suit;
    c->rank = rank;

    return c;
}

void
card_free(struct card* c)
{
    g_slice_free(struct card, c);
}

void
card_free_gfunc(gpointer data, gpointer user_data)
{
    card_free((struct card*)data);
}

int
card_is_counter(struct card* card)
{
    enum rank rank = card->rank;
    for (unsigned long i = 0; i < NCOUNTERS; i++) {
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
    for (unsigned long i = 0; i < NNONCOUNTERS; i++) {
        if (rank == NONCOUNTERS[i]) {
            return 1;
        }
    }
    return 0;
}

GString*
card_str(struct card* card)
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
card_show(struct card* card, const gchar* fmtstr)
{
    GString* buf = card_str(card);
    printf(fmtstr, buf->str);
    g_string_free(buf, FALSE);
}

void
card_tests()
{
    printf("[+] Running tests for card.\n");

    /* test creation */
    struct card* c11 = card_new(ace, spades);
    assert(c11->rank == ace);
    assert(c11->suit == spades);
    card_free(c11);

    /* test is_counter() */
    struct card* c21 = card_new(ace, spades);
    assert(card_is_counter(c21) == 1);
    card_free(c21);
    struct card* c22 = card_new(queen, spades);
    assert(card_is_counter(c22) == 0);
    card_free(c22);

    /* test is_noncounter() */
    struct card* c31 = card_new(queen, spades);
    assert(card_is_noncounter(c31) == 1);
    card_free(c31);
    struct card* c32 = card_new(ace, spades);
    assert(card_is_noncounter(c32) == 0);
    card_free(c32);

    /* test str() */
    struct card* c41 = card_new(ace, spades);
    GString* c41_str = card_str(c41);
    assert(g_strcmp0(c41_str->str, "ace of spades") == 0);
    g_string_free(c41_str, FALSE);
    card_free(c41);
    struct card* c42 = card_new(queen, hearts);
    GString* c42_str = card_str(c42);
    assert(g_strcmp0(c42_str->str, "queen of hearts") == 0);
    g_string_free(c42_str, FALSE);
    card_free(c42);

    /* test show() */
    struct card* c51 = card_new(ace, spades);
    card_show(c51, "card is %s.\n");
    card_free(c51);

    printf("[+] Finished tests for card.\n");
}

const guint32 PACK_CARD_COUNT = 48;
struct deck
{
    GList* cards;
    guint32 ncards;
};

struct deck*
deck_new()
{
    struct deck* d = g_slice_new(struct deck);
    d->ncards = PACK_CARD_COUNT;
    d->cards = NULL;

    /* for each rank, make a card of each suit */
    for (unsigned long i = 0; i < NRANK; i++) {
        for (unsigned long j = 0; j < NSUIT; j++) {
            struct card* cur = g_slice_new(struct card);
            cur->rank = RANKS[i];
            cur->suit = SUITS[j];
            d->cards = g_list_append(d->cards, cur);
        }
    }

    return d;
}

void
deck_free(struct deck* d)
{
    g_list_foreach(d->cards, card_free_gfunc, NULL);
    g_slice_free(struct deck, d);
}

struct card*
deck_get(struct deck* d, guint32 pos)
{
    struct card* c = g_list_nth_data(d->cards, pos);
    return c;
}

struct card*
deck_draw(struct deck* d, guint32 pos)
{
    struct card* c = deck_get(d, pos);
    d->cards = g_list_remove(d->cards, c);
    d->ncards = d->ncards - 1;

    return c;
}

// GHashTable* deck_hash_table(struct deck *d)
// {
//     GHashTable* ht = g_hash_table_new(g_str_hash, g_str_equal);
//     for (guint32 i = 0; i < d->ncards; i++) {
//         struct card *c = deck_get(d, i);
//         g_hash_table_insert(ht, card_human(d->), d->cards[i])
//     }
// }

void
deck_show(struct deck* deck)
{
    g_list_foreach(deck->cards, (GFunc)card_show, NULL);
}

void
deck_tests()
{
    printf("[+] Running tests for deck.\n");

    /* test new() */
    struct deck* d11 = deck_new();
    assert(d11->ncards == PACK_CARD_COUNT);
    assert(g_list_length(d11->cards) == PACK_CARD_COUNT);
    deck_free(d11);

    /* test get() */
    struct deck* d21 = deck_new();
    struct card* c21 = deck_get(d21, 0);
    assert(c21->rank == ace && c21->suit == clubs);
    struct card* c22 = deck_get(d21, PACK_CARD_COUNT - 1);
    assert(c22->rank == nine && c22->suit == spades);
    deck_free(d21);

    /* test hashmap() */
    // struct deck* d21 = deck_new();
    // GHashTable* t21 = deck_hash_table(d21);
    // assert(g_hash_table_size(t21) == d->ncards);
    // g_hash_table_destroy(t21);
    // deck_free(d21);

    printf("[+] Finished tests for deck.\n");
}

const guint32 NPLAYERS =
  2; /* 4 players can play in 2 teams of 2. is this right? */
struct player
{
    guint32 id;
    char* name;
    guint32 is_dealer;
    GList* hand;
};

void
player_init(struct player* player, char* name, guint32 is_dealer, GList* hand)
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

GRand* grand = NULL;
int
get_rand_int(guint32 begin, guint32 end)
{
    if (grand == NULL) {
        grand = g_rand_new_with_seed(1);
    }

    guint32 num = g_rand_int_range(grand, begin, end);

    return num;
}

int
main(int argc, char** argv)
{
    printf("Received %d arguments. They are.\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    card_tests();

    printf("Goodbye.\n");
    return 0;
}
