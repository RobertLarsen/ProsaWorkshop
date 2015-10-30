typedef enum {
    DO_STUFF,
    DO_OTHER_STUFF,
    DO_SOMETHING_ELSE
} command_t;

int condition_switch(command_t cmd) {
    switch (cmd) {
        case DO_STUFF:          return  7; break;
        case DO_OTHER_STUFF:    return  9; break;
        case DO_SOMETHING_ELSE: return 13; break;
        default:                return -1; break;
    }
}
