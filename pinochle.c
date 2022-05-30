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

/* *** helpers *** */
GRand* grand = NULL;
guint32
get_rand_int()
{
    if (grand == NULL) {
        grand = g_rand_new_with_seed(1);
    }

    guint32 num = g_rand_int(grand);

    return num;
}

GRand* grand_range = NULL;
guint32
get_rand_int_range(guint32 begin, guint32 end)
{
    if (grand_range == NULL) {
        grand_range = g_rand_new_with_seed(1);
    }

    guint32 num = g_rand_int_range(grand_range, begin, end);

    return num;
}

gpointer
get_rand_list_elem(GList* lst)
{
    guint32 len = g_list_length(lst);
    guint32 rand_pos = get_rand_int_range(0, len);
    gpointer data = g_list_nth_data(lst, rand_pos);

    return data;
}
/* ***** */

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

enum card_state
{
    in_deck,
    in_play,
    in_pile
};


/* *** card *** */
struct card
{
    enum suit suit;
    enum rank rank;
    enum card_state state;
};

struct card*
card_new(enum rank rank, enum suit suit)
{
    struct card* c = malloc(sizeof(struct card));
    // struct card* c = malloc(sizeof(struct card));
    c->suit = suit;
    c->rank = rank;
    c->state = in_deck;

    return c;
}

void
card_free(struct card* c)
{
    free(c);
    // free(c);
}

guint32
card_is_valid(struct card* c)
{
    enum rank rank = c->rank;
    if (rank == ace || rank == ten || rank == king || rank == queen ||
        rank == jack || rank == nine) {
        enum suit suit = c->suit;
        if (suit == clubs || suit == diamonds || suit == hearts ||
            suit == spades) {
            return 1;
        } else {
            return 0;
        }

    } else {
        return 0;
    }
}

gint32
card_compare(struct card* c1, struct card* c2)
{
    if (c1->rank == c2->rank && c1->suit == c2->suit) {
        return 0;
    } else {
        return -1;
    }
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
    } else {
        g_string_append(buf, "unknown");

        return buf;
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
card_str_free(void* str)
{
    g_string_free(str, FALSE);
}

void
card_show(struct card* card, const gchar* fmtstr)
{
    GString* buf = card_str(card);
    printf(fmtstr, buf->str);
    g_string_free(buf, FALSE);
}

void
card_show_gfunc(gpointer card, gpointer fmtstr)
{
    card_show((struct card*)card, (const gchar*)fmtstr);
}

void
card_tests()
{
    printf("[+] Running tests for card.\n");

    /* test creation */
    struct card* c11 = card_new(ace, spades);
    assert(c11->rank == ace);
    assert(c11->suit == spades);
    assert(c11->state == in_deck);
    card_free(c11);

    /* test is_valid() */
    struct card* c61 = card_new(ace, spades);
    assert(card_is_valid(c61) == 1);
    card_free(c61);
    struct card* c62 = card_new(ace, spades);
    c62->rank = 99;
    assert(card_is_valid(c62) == 0);
    card_free(c62);
    struct card* c63 = card_new(ace, spades);
    c63->suit = 99;
    assert(card_is_valid(c63) == 0);
    card_free(c63);

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
    struct card* c43 = card_new(queen, hearts);
    GString* c43_str = card_str(c43);
    assert(g_strcmp0(c43_str->str, "queen of hearts") == 0);
    g_string_free(c43_str, FALSE);
    card_free(c43);

    /* test show() */
    struct card* c51 = card_new(ace, spades);
    card_show(c51, "card is %s.\n");
    card_free(c51);

    /* test compare() */
    struct card* c71 = card_new(ace, spades);
    struct card* c72 = card_new(queen, hearts);
    struct card* c73 = card_new(ace, spades);
    assert(card_compare(c71, c73) == 0);
    assert(card_compare(c71, c72) == -1);
    card_free(c71);
    card_free(c72);
    card_free(c73);

    printf("[+] Finished tests for card.\n");
}
/* ***** */


/* *** card_list *** */
struct card_list
{
    GList* cards;
};

struct card_list*
card_list_new()
{
    struct card_list* cl = malloc(sizeof(struct card_list));
    cl->cards = NULL;

    return cl;
}

void
card_list_free(struct card_list* cl)
{
    free(cl);
}

void
card_list_add(struct card_list* cl, struct card* c)
{
    cl->cards = g_list_append(cl->cards, c);
}

guint32
card_list_count(struct card_list* cl)
{
    return g_list_length(cl->cards);
}

struct card*
card_list_remove(struct card_list* cl, guint32 pos)
{
    guint32 len = g_list_length(cl->cards);
    if (len == 0) {
        printf("ERROR: Cannot remove card from empty card_list.\n");

        return NULL;
    }
    struct card* c = g_list_nth_data(cl->cards, pos);
    cl->cards = g_list_remove(cl->cards, c);

    return c;
}

void
card_list_tests()
{
    printf("[+] Running tests for card_list.\n");

    /* test creation */
    struct card_list* cl11 = card_list_new();
    assert(cl11->cards == NULL);
    card_list_free(cl11);

    /* test add() */
    struct card_list* cl21 = card_list_new();
    struct card* c21 = card_new(ace, spades);
    card_list_add(cl21, c21);
    assert(g_list_length(cl21->cards) == 1);
    card_list_free(cl21);

    /* test count() */
    struct card_list* cl31 = card_list_new();
    assert(card_list_count(cl31) == 0);
    struct card* c31 = card_new(ace, spades);
    card_list_add(cl31, c31);
    assert(card_list_count(cl31) == 1);
    card_list_free(cl31);

    /* test remove() */
    struct card_list* cl41 = card_list_new();
    struct card* c41 = card_new(ace, spades);
    card_list_add(cl41, c41);
    struct card* c42 = card_list_remove(cl41, 0);
    assert(card_list_count(cl41) == 0);
    assert(card_compare(c41, c42) == 0);
    assert(card_list_remove(cl41, 0) == NULL);
    card_list_free(cl41);

    printf("[+] Finished tests for card_list.\n");
}
/* ***** */

/* *** deck *** */
const guint32 DECK_CARD_COUNT = 24;
struct deck
{
    GList* cards;
};

struct deck*
deck_new()
{
    struct deck* d = malloc(sizeof(struct deck));
    d->cards = NULL;
    /* for each rank, make a card of each suit */
    for (unsigned long i = 0; i < NRANK; i++) {
        for (unsigned long j = 0; j < NSUIT; j++) {
            struct card* cur = card_new(RANKS[i], SUITS[j]);
            d->cards = g_list_append(d->cards, cur);
        }
    }

    return d;
}

guint32
deck_count(struct deck* d)
{
    return g_list_length(d->cards);
}

struct card*
deck_get(struct deck* d, guint32 pos)
{
    if (pos < deck_count(d)) {
        struct card* c = g_list_nth_data(d->cards, pos);
        return c;
    } else {
        printf("ERROR: a card at position %u in the deck does not exist.\n",
               pos);
        return NULL;
    }
}

void
deck_free(struct deck* d)
{
    // fix this g_list_foreach(d->cards, card_free_gfunc, NULL);
    for (guint32 i = 0; i < deck_count(d); i++) {
        card_free(deck_get(d, i));
    }
    free(d);
}

struct card*
deck_draw(struct deck* d, guint32 pos)
{
    struct card* c = deck_get(d, pos);
    c->state = in_play;

    return c;
}

struct card*
deck_draw_rand(struct deck* d)
{
    guint32 pos = get_rand_int_range(0, deck_count(d));
    struct card* c = deck_draw(d, pos);

    return c;
}

void
deck_add(struct deck* d, struct card* c)
{
    d->cards = g_list_append(d->cards, c);
}

gint32
deck_is_valid(struct deck* d)
{
    if (deck_count(d) > DECK_CARD_COUNT) {
        return 0;
    } else {
        return 1;
    }
    /* TODO check that every card is valid card_is_valid() */
    /* TODO check that every card is unique */
}

void
deck_hash_table_free_value(void* num)
{
    free(num);
}

GHashTable*
deck_hash_table_new(struct deck* d)
{
    GHashTable* ht = g_hash_table_new_full(
      g_str_hash, g_str_equal, card_str_free, deck_hash_table_free_value);
    for (guint32 i = 0; i < deck_count(d); i++) {
        struct card* c = deck_get(d, i);
        GString* c_str = card_str(c);
        guint32* cnt = malloc(sizeof(guint32));
        *cnt = 1;
        printf("Adding %s\n", c_str->str);
        g_hash_table_insert(ht, c_str, cnt);
    }

    return ht;
}

void
deck_hash_table_free(GHashTable* ht)
{
    g_hash_table_destroy(ht);
}

// void deck_str_list_gfunc(gpointer card, gpointer lst)
// {
//     GList **real_lst = lst;
//     GList *old_lst = *real_lst;
//     *real_lst = g_list_append(old_lst, card_str(card));
// }

// GList* deck_str_list(struct deck* d)
// {
//     gpointer lst = malloc(sizeof(gpointer));
//     g_list_foreach(d->cards, deck_str_list_gfunc, lst);

//     return (GList*)lst;
// }

// void deck_str_gfunc(gpointer str, gpointer str_main)
// {
//     g_string_append_printf((GString *)str_main, "%s\n", (char *)str);
// }

// GString*
// deck_str(struct deck *d)
// {
//     GList *lst = deck_str_list(d);
//     GString* str = g_string_new("");
//     g_list_foreach(lst, deck_str_gfunc, str);

//     return str;
// }

void
deck_show(struct deck* deck)
{
    g_list_foreach(deck->cards, card_show_gfunc, "%s\n");
}

void
deck_tests()
{
    printf("[+] Running tests for deck.\n");

    /* test new() */
    struct deck* d11 = deck_new();
    assert(deck_count(d11) == DECK_CARD_COUNT);
    assert(g_list_length(d11->cards) == DECK_CARD_COUNT);
    deck_free(d11);

    /* test count() */
    struct deck* d41 = deck_new();
    assert(deck_count(d41) == DECK_CARD_COUNT);
    deck_free(d41);

    /* test get() */
    struct deck* d21 = deck_new();
    struct card* c21 = deck_get(d21, 0);
    assert(c21->rank == ace && c21->suit == clubs);
    assert(deck_count(d21) == DECK_CARD_COUNT);
    struct card* c22 = deck_get(d21, DECK_CARD_COUNT - 2);
    assert(card_is_valid(c22) == 1);
    assert(deck_count(d21) == DECK_CARD_COUNT);
    struct card* c23 = deck_get(d21, 99); /* this should be an error */
    assert(c23 == NULL);
    assert(deck_count(d21) == DECK_CARD_COUNT);
    deck_free(d21);

    /* test draw() */
    struct deck* d31 = deck_new();
    struct card* c31 = deck_draw(d31, 0);
    assert(c31->rank == ace && c31->suit == clubs && c31->state == in_play);
    // assert(deck_count(d31) == DECK_CARD_COUNT - 1);
    struct card* c32 = deck_draw(d31, DECK_CARD_COUNT - 2);
    assert(card_is_valid(c32) == 1);
    // assert(deck_count(d31) == DECK_CARD_COUNT - 2);
    deck_free(d31);

    /* test draw_rand() */
    struct deck* d61 = deck_new();
    struct card* c61 = deck_draw_rand(d61);
    assert(card_is_valid(c61) == 1);
    // assert(deck_count(d61) == DECK_CARD_COUNT - 1);
    deck_free(d61);

    /* test hashmap() */
    // struct deck* d51 = deck_new();
    // GHashTable* ht51 = deck_hash_table_new(d51);
    // printf("ht51 count is %u.\nd51 count is %u.\n", g_hash_table_size(ht51),
    // deck_count(d51)); deck_show(d51); assert(g_hash_table_size(ht51) ==
    // deck_count(d51)); deck_hash_table_free(ht51); deck_free(d51);

    /* test show() */
    struct deck* d71 = deck_new();
    deck_show(d71);
    deck_free(d71);

    /* test add() */
    struct deck* d91 = deck_new();
    struct card* c91 = card_new(ace, clubs);
    deck_add(d91, c91);
    assert(deck_count(d91) == DECK_CARD_COUNT + 1);
    struct card* c92 = deck_get(d91, 0);
    assert(card_compare(c91, c92) == 0);
    deck_free(d91);

    /* test is_valid() */
    struct deck* d81 = deck_new();
    // struct deck *d82 = deck_new();
    assert(deck_is_valid(d81) == 1);
    // // TODO here
    deck_free(d81);
    // deck_free(d82);

    printf("[+] Finished tests for deck.\n");
}
/* ***** */

/* *** pinochle_deck *** */
const guint32 NDECKS = 2;
struct pinochle_deck
{
    GList* deck_pool;
    guint32 ndecks;
};

struct pinochle_deck*
pinochle_deck_new(guint32 ndecks)
{
    struct pinochle_deck* pd = malloc(sizeof(struct pinochle_deck));
    pd->deck_pool = NULL;
    for (guint32 i = 0; i < ndecks; i++) {
        struct deck* d = deck_new();
        pd->deck_pool = g_list_append(pd->deck_pool, d);
    }
    pd->ndecks = ndecks;

    return pd;
}

/* TODO add get() and draw() and is_valid() functions */

void
pinochle_deck_free(struct pinochle_deck* pd)
{
    free(pd);
}

struct deck*
pinochle_deck_get_deck(struct pinochle_deck* pd, guint32 pos)
{
    struct deck* d = g_list_nth_data(pd->deck_pool, pos);

    return d;
}

struct deck*
pinochle_deck_get_deck_rand(struct pinochle_deck* pd)
{
    guint32 rint = get_rand_int_range(0, pd->ndecks);
    struct deck* d = pinochle_deck_get_deck(pd, rint);

    return d;
}

struct card*
pinochle_deck_get_card(struct pinochle_deck* pd,
                       guint32 deck_pos,
                       guint32 card_pos)
{
    struct deck* d = pinochle_deck_get_deck(pd, deck_pos);
    struct card* c = deck_get(d, card_pos);

    return c;
}

struct card*
pinochle_deck_draw_rand(struct pinochle_deck* pd)
{
    struct deck* d = pinochle_deck_get_deck_rand(pd);
    struct card* c = deck_draw_rand(d);

    return c;
}

GList*
pinochle_deck_draw_rand_n(struct pinochle_deck* pd, guint32 ncards)
{
    GList* cards = NULL;
    for (guint32 i = 0; i < ncards; i++) {
        struct card* c = pinochle_deck_draw_rand(pd);
        cards = g_list_append(cards, c);
    }

    return cards;
}

void
pinochle_deck_tests()
{
    printf("[+] Running tests for pinochle_deck.\n");

    /* test new() */
    struct pinochle_deck* pd11 = pinochle_deck_new(2);
    assert(g_list_length(pd11->deck_pool) == 2);
    assert(pd11->ndecks == 2);
    pinochle_deck_free(pd11);

    /* test get_deck() */
    struct pinochle_deck* pd31 = pinochle_deck_new(2);
    struct deck* d31 = pinochle_deck_get_deck(pd31, 0);
    assert(deck_is_valid(d31) == 1);
    pinochle_deck_free(pd31);

    /* test get_deck_rand() */
    struct pinochle_deck* pd41 = pinochle_deck_new(2);
    struct deck* d41 = pinochle_deck_get_deck_rand(pd41);
    assert(deck_is_valid(d41) == 1);
    pinochle_deck_free(pd41);

    /* test get_card() */
    struct pinochle_deck* pd21 = pinochle_deck_new(2);
    struct card* c21 = pinochle_deck_get_card(pd21, 0, 0);
    assert(card_is_valid(c21) == 1);
    pinochle_deck_free(pd21);

    /* test draw_rand() */
    struct pinochle_deck* pd51 = pinochle_deck_new(2);
    struct card* c51 = pinochle_deck_draw_rand(pd51);
    assert(card_is_valid(c51) == 1);
    pinochle_deck_free(pd51);

    /* test draw_rand_n() */
    struct pinochle_deck* pd61 = pinochle_deck_new(2);
    GList* cl61 = pinochle_deck_draw_rand_n(pd61, 2);
    assert(g_list_length(cl61) == 2);
    for (guint32 i = 0; i < 2; i++) {
        struct card* c61 = g_list_nth_data(cl61, i);
        assert(card_is_valid(c61));
    }
    pinochle_deck_free(pd61);

    /* TODO test add() */

    printf("[+] Finished tests for pinochle_deck.\n");
}
/* ***** */


/* *** player *** */
const guint32 NPLAYERS =
  2; /* 4 players can play in 2 teams of 2. is this right? */
struct player
{
    guint32 id;
    GString* name;
    guint32 is_dealer;
    struct card_list* hand;
};

guint32 player_next_id = 0;
guint32
player_get_next_id()
{
    guint32 next_id = player_next_id;
    player_next_id = player_next_id + 1;

    return next_id;
}

struct player*
player_new(const gchar* name, guint32 is_dealer)
{
    struct player* p = malloc(sizeof(struct player));
    p->id = player_get_next_id();
    p->name = g_string_new(name);
    p->is_dealer = is_dealer;
    p->hand = card_list_new();

    return p;
}

void
player_free(struct player* p)
{
    g_string_free(p->name, FALSE);
    card_list_free(p->hand);
    free(p);
}

int
player_is_dealer(struct player* p)
{
    return p->is_dealer;
}

void
player_tests()
{
    printf("[+] Running tests for player.\n");

    /* test get_next_id() */
    guint32 id31 = player_get_next_id();
    assert(id31 == 0);
    guint32 id32 = player_get_next_id();
    assert(id32 == 1);

    /* test new() */
    GString* n11 = g_string_new("paulie walnuts");
    struct player *p11 = player_new(n11->str, 0);
    assert(p11->id == 2);
    assert(g_string_equal(p11->name, n11) == 1);
    assert(p11->is_dealer == 0);
    assert(card_list_count(p11->hand) == 0);
    g_string_free(n11, FALSE);
    player_free(p11);

    /* test is_dealer() */
    struct player *p21 = player_new("frank sinatra, jr.", 1);
    assert(player_is_dealer(p21) == 1);
    player_free(p21);

    printf("[+] Finished tests for player.\n");
}
/* ***** */


/* *** pinochle *** */
struct pinochle
{
    GList* players;
    struct pinochle_deck* deck;
};

struct pinochle*
pinochle_new(guint32 nplayers, guint32 ndecks, const gchar** names)
{
    struct pinochle* pn = malloc(sizeof(struct pinochle));
    pn->players = NULL;
    for (guint32 i = 0; i < nplayers; i++) {
        struct player* p = player_new(names[i], 0);
        pn->players = g_list_append(pn->players, p);
    }
    pn->deck = pinochle_deck_new(ndecks);

    /* pick dealer */
    struct player* dealer = get_rand_list_elem(pn->players);
    dealer->is_dealer = 1;

    return pn;
}

void
pinochle_free(struct pinochle* p)
{
    for (guint32 i = 0; i < g_list_length(p->players); i++) {
        player_free(g_list_nth_data(p->players, i));
    }
    g_list_free(p->players);
    pinochle_deck_free(p->deck);
    free(p);
}

const guint32 INIT_CARDS_PER_PLAYER = 12;
void pinochle_deal_init(struct pinochle* p)
{
}

void
pinochle_tests()
{
    printf("[+] Running tests for pinochle.\n");

    /* test new() */
    const gchar* n11[] = {"frank sinatra, jr.", "silvio dante"};
    struct pinochle* p11 = pinochle_new(2, 2, n11);
    assert(g_list_length(p11->players) == 2);
    assert(p11->deck->ndecks == 2);
    pinochle_free(p11);

    /* test deal_init() */
    const gchar* n21[] = {"frank sinatra, jr.", "silvio dante"};
    struct pinochle* p21 = pinochle_new(2, 2, n21);
    pinochle_free(p21);


    printf("[+] Finished tests for pinochle.\n");
}
/* ***** */


int
main(int argc, char** argv)
{
    printf("Received %d arguments. They are.\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("%s\n", argv[i]);
    }

    card_tests();
    card_list_tests();
    deck_tests();
    pinochle_deck_tests();
    player_tests();
    pinochle_tests();

    printf("Goodbye.\n");
    return 0;
}
