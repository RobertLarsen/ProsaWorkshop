EXAMPLE_ASM_FILES=$(wildcard examples/*.asm)
EXAMPLE_OBJECTS=$(EXAMPLE_ASM_FILES:.asm=.o)
EXAMPLE_ELF=$(EXAMPLE_ASM_FILES:.asm=.elf)
EXAMPLE_SHELLCODE=$(EXAMPLE_ASM_FILES:.asm=.shellcode)
SVG_FILES=$(wildcard images/*.svg)
SVG_PNG_FILES=$(SVG_FILES:.svg=.png)
ASCIIDOC_FLAGS=--attribute duration=240 --attribute data-uri --attribute stylesheet=prosa.css

.phony: watch clean all

all: index.html $(EXAMPLE_ELF) $(EXAMPLE_SHELLCODE)

index.html: index.asciidoc text/*.asciidoc $(SVG_PNG_FILES)
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

clean:
	rm -rf index.html $(SVG_PNG_FILES) $(EXAMPLE_ELF) $(EXAMPLE_SHELLCODE) $(EXAMPLE_OBJECTS)

watch: all
	wait_for_change Makefile *.asciidoc text/*.asciidoc examples/*.asm | while read f; do rm -f index.html; make; done
