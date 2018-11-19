#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char const* argv[]) {
    struct passwd *pwd;
    if ((pwd = getpwuid(getuid()))) {
        printf("%s/%d/%d: %s\n", pwd->pw_name, pwd->pw_uid, pwd->pw_gid, pwd->pw_shell);
    }
    return 0;
}
