#include "stdio.h"

typedef struct Player {
    struct Player* prev;
    struct Player* next;
    char name[32];
    int health;
    int x;
    int y;
    float speed;
} Player;

extern Player player2;
Player player1 = {0, &player2, "Placeh@lder", 40};
Player player2 = {&player1, 0, "Placeh@lder", 80};

void iter1(Player* player) {
    for (; player != 0; player = player->next) {
        printf("%s\n", player->name);
    }
}

void iter2() {
    for (Player* p = &player1; p; p = p->next) {
        printf("%s\n", p->name);
    }
}

void iter3() {
    int i;

    for (i = 0; i < 32; i++) {
        if (player1.name[i] == '@') {
            player1.name[i] = ' ';
        }
    }
}

void iter4() {
    int i;
    for (i = 0; i < player1.health; i++) {
        printf("%d\n", player1.health);
    }
}

void iter5() {
    Player* p;
    for (p = &player1; p != NULL; p = p->next) {
        p->health--;
    }
}

int iter6() {
    int i = 0;
    int j = 0;
    int output = 0;
    for (; i < 1000;) {
        j--;
        if (j == 1) {
            i++;
        }
    }
    return output;
}

int main() {
    iter1(&player1);
    iter2();
    iter3();
    iter4();
    iter5();
    iter6();
    return 0;
}
