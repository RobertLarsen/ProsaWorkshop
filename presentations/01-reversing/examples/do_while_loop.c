int do_while_loop(int n) {
    int i = 0, sum = 0;
    do {
        sum += i;
    } while (++i < n);
    return sum;
}
