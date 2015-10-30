#include <stdio.h>
#include <string.h>

/***************************************************************************
 *
 * rivendel har to anti debugging tricks.
 * 1) Den crasher gdb (dette kan omgåes ved at strippe filen)
 * 2) Den ptracer sig selv to steder (dette kan omgåes ved at patche filen eller rette i resultatet med debuggeren)
 *    Put følgende i .gdbinit:
 *    -------------------------------
 *         b *0x08048956
 *         commands
 *             set $eax = 0
 *             continue
 *         end
 *         
 *         b *0x080487cf
 *         commands
 *             set $eax = 0
 *             continue
 *         end
 *    -------------------------------
 * 
 * 
 * Programmet har en main og en _start funktion, men de er der bare for at forvirre, for dens entrypoint er lidt off:
 * 
 * $ readelf -h breakme | grep 'Entry point'
 *   Entry point address:               0x80487b6
 * 
 * Det er den funktion som hedder aA_reAdCxzeTgbcAweGnjkgTREGvxzAergFDsEwaAAqEWdbVBa__DASs56gfds.
 * 
 * Et forsøg på at omskrive den og hvad den kalder videre til om til C giver mig følgende:
 * 
 * void aA_reAdCxzeTgbcAweGnjkgTREGvxzAergFDsEwaAAqEWdbVBa__DASs56gfds() {
 *     char password[120];
 *     char user[41];
 *     
 *     if (ptrace(0, 0, 1, 0) < 0) {
 *         puts("\nYou are debugging me, I don't like it");
 *         exit(1);
 *     } else {
 *         puts("Keygen me... if you can ;-)");
 *         puts("Have fun & Good luck!");
 *         printf("User (1 - 40 chars: ");
 *         if (fgets(user, sizeof(user), stdin)) {
 *             user[strlen(user) - 1] = 0;
 *             printf("Password for the user %s (%d bytes): ", user, strlen(user) * 3);
 *             if (aA_reAdCxzeTgbcAweGnjkgTREGvxzAergFDsEwaAAqEWdcVBa__DASs56gfds(password, strlen(user) * 3 + 1)) {
 *                 password[strlen(password) - 1] = 0;
 *                 if (aA_reAdCxzeTgbcAweGnjkgTREGvjzAergFDjEwaAAqRWzbVBa__DASs56gfds(user, password, 0, 0, strlen(user)) == 0) {
 *                     puts("\nYout got it, or not... :)");
 *                 }
 *                 exit(1);
 *             }
 *         }
 *         puts("Invalid input!");
 *         exit(1);
 *     }
 * }
 * 
 * int aA_reAdCxzeTgbcAweGnjkgTREGvxzAergFDsEwaAAqEWdcVBa__DASs56gfds(char * password, int len) {
 *     int i;
 *     for (i = 0; i < len; i++) {
 *         password[i] = getchar();
 *     }
 * 
 *     return 1;
 * }
 * 
 * int aA_reAdCxzeTgbcAweGnjkgTREGvjzAergFDjEwaAAqRWzbVBa__DASs56gfds(char * user, char * password, size_t index, char arg_C, size_t user_len) {
 *     if (user_len * 3 != index) {
 *         if (index >= user_len * 2) {
 *             arg_C = user[index % user_len] ^ ((arg_C - 'L') * 2);
 *             if (arg_C != password[index]) {
 *                 WRONG_SERIAL:
 *                 puts("WRONG SERIAL :-(");
 *                 exit(1);
 *             } else {
 *                 aA_reAdCxzeTgbcAweGnjkgTREGvjzAergFDjEwaAAqRWzbVBa__DASs56gfds(user, password, index + 1, arg_C, user_len);
 *             }
 *         } else if (index >= user_len) {
 *             arg_C = user[index % user_len] ^ (18 - arg_C);
 *             if (++index == user_len * 2) {
 *                 arg_C += 'q';
 *             }
 *             aA_reAdCxzeTgbcAweGnjkgTREGvjzAergFDjEwaAAqRWzbVBa__DASs56gfds(user, password, index, arg_C, user_len);
 *         } else {
 *             arg_C = user[index] ^ (arg_C + 15);
 *             if (arg_C != password[index]) {
 *                 goto WRONG_SERIAL;
 *             } else {
 *                 if (++index == user_len) {
 *                     arg_C -= 10;
 *                 }
 *                 aA_reAdCxzeTgbcAweGnjkgTREGvjzAergFDjEwaAAqRWzbVBa__DASs56gfds(user, password, index, arg_C, user_len);
 *             }
 *         }
 *     }
 *     return 0;
 * }
 * 
 ***************************************************************************/

int main(int argc, char *argv[]) {
    char serial[128];

    char c;
    size_t i, len;

    if (argc == 1) {
        fprintf(stderr, "Usage: %s <name> | ./breakme\n", argv[0]);
    } else {
        memset(serial, 0, sizeof(serial));
        c = 0;
        len = strlen(argv[1]);
        for (i = 0; i < len * 3; i++) {
            if (i >= len * 2) {
                serial[i] = c = argv[1][i % len] ^ ((c - 'L') * 2);
            } else if (i >= len) {
                serial[i] = c = argv[1][i % len] ^ (18 - c);
                if (i + 1 == len * 2) {
                    c += 'q';
                }
            } else {
                serial[i] = c = argv[1][i] ^ (c + 15);
                if (i + 1 == len) {
                    c -= 10;
                }
            }
        }

        printf("%s\n%s\n", argv[1], serial);
    }
    return 0;
}
