#include <stdio.h>

#define PROJECT_NAME "pinochle"

const int PACK_CARD_COUNT = 48;

enum suit {
    clubs,
    diamonds,
    hearts,
    spades
};

enum rank {
    ace,
    ten,
    king,
    queen,
    jack,
    nine
};

struct card {
    enum suit suit;
    enum rank rank;
};

int main(int argc, char **argv) {
    if(argc != 1) {
        printf("%s takes no arguments.\n", argv[0]);
        return 1;
    }
    printf("This is project %s.\n", PROJECT_NAME);
    return 0;
}
