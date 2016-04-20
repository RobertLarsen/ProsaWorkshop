EXAMPLE_ASM_FILES=$(wildcard examples/*.asm)
EXAMPLE_OBJECTS=$(EXAMPLE_ASM_FILES:.asm=.o)
EXAMPLE_ELF=$(EXAMPLE_ASM_FILES:.asm=.elf)
EXAMPLE_SHELLCODE=$(EXAMPLE_ASM_FILES:.asm=.shellcode)
SVG_FILES=$(wildcard images/*.svg)
SVG_PNG_FILES=$(SVG_FILES:.svg=.png)
IMPLS=assignments/execute_256 assignments/avoid_chars assignments/required_constants assignments/read_pointers assignments/write_pointers assignments/trampoline_few_bytes assignments/printable_shellcode assignments/large_in_64 assignments/egg_hunting assignments/stack_hunting
ASCIIDOC_FLAGS=--attribute duration=240 --attribute data-uri --attribute stylesheet=prosa.css
CFLAGS=-m32 -z execstack -fno-stack-protector -Wno-deprecated-declarations 
MAKE_CHROOT=../../scripts/make_chroot.sh

.phony: watch clean all no_presentation

all: index.html no_presentation

no_presentation: $(EXAMPLE_ELF) $(EXAMPLE_SHELLCODE) assignments/shelly assignments/root

assignments/execute_256: assignments/shelly.c
	awk '/execute_256 STARTS/{flag=1;next}/execute_256 ENDS/{flag=0}flag' $< > $@

assignments/avoid_chars: assignments/shelly.c
	awk '/avoid_chars STARTS/{flag=1;next}/avoid_chars ENDS/{flag=0}flag' $< > $@

assignments/required_constants: assignments/shelly.c
	awk '/required_constants STARTS/{flag=1;next}/required_constants ENDS/{flag=0}flag' $< > $@

assignments/read_pointers: assignments/shelly.c
	awk '/read_pointers STARTS/{flag=1;next}/read_pointers ENDS/{flag=0}flag' $< > $@

assignments/write_pointers: assignments/shelly.c
	awk '/write_pointers STARTS/{flag=1;next}/write_pointers ENDS/{flag=0}flag' $< > $@

assignments/trampoline_few_bytes: assignments/shelly.c
	awk '/trampoline_few_bytes STARTS/{flag=1;next}/trampoline_few_bytes ENDS/{flag=0}flag' $< > $@

assignments/printable_shellcode: assignments/shelly.c
	awk '/printable_shellcode STARTS/{flag=1;next}/printable_shellcode ENDS/{flag=0}flag' $< > $@

assignments/large_in_64: assignments/shelly.c
	awk '/large_in_64 STARTS/{flag=1;next}/large_in_64 ENDS/{flag=0}flag' $< > $@

assignments/egg_hunting: assignments/shelly.c
	awk '/egg_hunting STARTS/{flag=1;next}/egg_hunting ENDS/{flag=0}flag' $< > $@

assignments/stack_hunting: assignments/shelly.c
	awk '/stack_hunting STARTS/{flag=1;next}/stack_hunting ENDS/{flag=0}flag' $< > $@

index.html: index.asciidoc text/*.asciidoc $(SVG_PNG_FILES) $(IMPLS)
	asciidoc $(ASCIIDOC_FLAGS) --backend slidy2 --out-file=- $< | sed -e '/id="footer"/,+6d' -e 's/next_slide(false/next_slide(true/' -e 's/previous_slide(false/previous_slide(true/' -e 's/key == 188/key == 116 || key == 27/' > $@

images/%.png: images/%.svg
	inkscape --export-area-drawing --export-png=$@ $<

examples/%.elf: examples/%.o
	ld --strip-all -melf_i386 -o $@ $<

examples/%.o: examples/%.asm
	nasm -f elf -o $@ $<

examples/%.shellcode: examples/%.asm
	nasm -f bin -o $@ $<
	ndisasm -b 32 $@

assignments/shelly: assignments/shelly.c
	$(CC) $(CFLAGS) -o $@ $^

assignments/treebuilder: assignments/treebuilder.c
	$(CC) -m32 -fPIC -pie -ggdb -o $@ $^

assignments/root: assignments/treebuilder
	$(MAKE_CHROOT) $@/ $^ /bin/bash /bin/ls /bin/cat /usr/bin/id
	cp $@/bin/bash $@/bin/sh
	head -c 20 /dev/urandom | md5sum | awk '{print $$1}' > assignments/flag

clean:
	rm -rf index.html $(SVG_PNG_FILES) $(EXAMPLE_ELF) $(EXAMPLE_SHELLCODE) $(EXAMPLE_OBJECTS) assignments/root assignments/shelly assignments/treebuilder $(IMPLS)

watch: all $(IMPLS)
	wait_for_change Makefile *.asciidoc text/*.asciidoc examples/*.asm | while read f; do rm -f index.html; make; done

watch_shelly: assignments/shelly
	wait_for_change assignments/shelly.c | while read f; do make $<; done
