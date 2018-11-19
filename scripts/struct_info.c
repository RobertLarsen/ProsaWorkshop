#include <stdio.h>
#include <string.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

int main(int argc, char const* argv[]) {
    int i, max_len = 0;
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <struct> <member1> [member2...]\n", argv[0]);
    } else {
        printf(
                "#include <stdio.h>\n"
                "#include <stddef.h>\n"
                "\n"
                "typedef struct {\n"
                "    char *name;\n"
                "    unsigned int size;\n"
                "    unsigned int offset;\n"
                "} member_t;\n"
                "\n"
                "int main(int argc, char ** argv) {\n"
                "    member_t members[%d];\n"
                "    int i, padding;\n"
                "    printf(\"Sizeof struct %s: %%d\\n\", sizeof(struct %s));\n", argc - 2, argv[1], argv[1]
        );

        for (i = 2; i < argc; i++) {
            printf("    members[%d].name = \"%s\";\n", i - 2, argv[i]);
            printf("    members[%d].size = sizeof( ((struct %s*)NULL)->%s );\n", i - 2, argv[1], argv[i]);
            printf("    members[%d].offset = offsetof(struct %s, %s);\n", i - 2, argv[1], argv[i]);
            max_len = max(max_len, strlen(argv[i]));
        }

        printf("\n");

        printf(
                "    for (i = 0; i < sizeof(members) / sizeof(member_t); i++) {\n"
                "        padding = (i == 0 ? 0 : members[i].offset - (members[i - 1].offset + members[i - 1].size));\n"
                "        printf(\"%%%ds (%%d) : 0x%%03x /%%3d\", members[i].name, members[i].size, members[i].offset, members[i].offset);\n"
                "        if (padding > 0) {\n"
                "            printf(\"   <- Needs to be offset by %%d bytes\\n\", padding);\n"
                "        } else {\n"
                "            printf(\"\\n\");\n"
                "        }\n"
                "    }\n", max_len
        );

        printf("\n    return 0;\n}\n");
    }
    
    return 0;
}
