FROM ubuntu:14.04.5
MAINTAINER Robert Larsen <robert@the-playground.dk>

COPY presentations/02-exploitation/assignments/integer_conversion.c /02-exploitation/integer_conversion.c
COPY presentations/02-exploitation/assignments/integer_overflow.c   /02-exploitation/integer_overflow.c

COPY presentations/03-shellcoding/assignments/shelly.c      /03-shellcoding/shelly.c
COPY presentations/03-shellcoding/assignments/treebuilder.c /03-shellcoding/treebuilder.c

COPY presentations/04-advanced-exploitation/assignments/fmt.c    /04-advanced-exploitation/fmt.c


RUN groupadd -rg 1000 exploitable && \
    useradd -mrg exploitable -u 1000 exploitable && \
    apt-get update && \
    apt-get -y install libc6-dev-i386 execstack && \
    gcc -m32 -z execstack -Wno-deprecated-declarations -fno-stack-protector -o /02-exploitation/integer_conversion /02-exploitation/integer_conversion.c && \
    gcc -m32 -z execstack -Wno-deprecated-declarations -fno-stack-protector -o /02-exploitation/integer_overflow /02-exploitation/integer_overflow.c && \
    gcc -m32 -z execstack -Wno-deprecated-declarations -fstack-protector-all -o /02-exploitation/integer_conversion_canary /02-exploitation/integer_conversion.c && \
    gcc -m32 -z execstack -Wno-deprecated-declarations -fstack-protector-all -o /02-exploitation/integer_overflow_canary /02-exploitation/integer_overflow.c && \
    gcc -m32 -z execstack -Wno-deprecated-declarations -fpic -pie -fstack-protector-all -o /02-exploitation/integer_conversion_canary_pie /02-exploitation/integer_conversion.c && \
    gcc -m32 -z execstack -Wno-deprecated-declarations -fpic -pie -fstack-protector-all -o /02-exploitation/integer_overflow_canary_pie /02-exploitation/integer_overflow.c && \
    gcc -m32 -fPIC -pie -ggdb -o /03-shellcoding/treebuilder /03-shellcoding/treebuilder.c && \
    gcc -m32 -z execstack -fno-stack-protector -Wno-deprecated-declarations  -o /03-shellcoding/shelly /03-shellcoding/shelly.c && \
    gcc -m32 -fpic -pie -z execstack -Wno-deprecated-declarations -Wno-format-security -o /04-advanced-exploitation/fmt /04-advanced-exploitation/fmt.c && \
    cp /04-advanced-exploitation/fmt /04-advanced-exploitation/fmt_nx && \
    execstack -c /04-advanced-exploitation/fmt_nx && \
    apt-get -y remove libc6-dev-i386 execstack && \
    rm -rf /var/lib/apt/lists/*


USER exploitable
