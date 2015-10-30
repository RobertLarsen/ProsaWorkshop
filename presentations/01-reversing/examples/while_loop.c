int while_loop(int n) {
    int i = 0, sum = 0;
    while (i < n) {
        sum += i++;
    }
    return sum;
}
