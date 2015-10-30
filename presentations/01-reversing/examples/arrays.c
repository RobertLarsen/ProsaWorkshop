int global[10];

void arrays() {
    int local[10];
    int i;
    for (i = 0; i < 10; i++) {
        global[i] = i;
    }

    for (i = 0; i < 10; i++) {
        local[i] = i;
    }
}
