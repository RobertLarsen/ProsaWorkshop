ASSIGNMENT_GPG_FILES=$(wildcard assignments/*.gpg)
ASSIGNMENT_C_FILES=$(EXAMPLE_GPG_FILES:.gpg=.c)
ASSIGNMENT_BINARIES=$(ASSIGNMENT_C_FILES:.c=.bin)
EXAMPLE_C_FILES=$(wildcard examples/*.c)
EXAMPLE_ASM_FILES=$(EXAMPLE_C_FILES:.c=.s)
SVG_FILES=$(wildcard images/*.svg)
SVG_PNG_FILES=$(SVG_FILES:.svg=.png)
PROLOGUE_FILES=examples/prologue_epilogue.s_0
PROLOGUE_FILES+=examples/prologue_epilogue.s_1
PROLOGUE_FILES+=examples/prologue_epilogue.s_2
PROLOGUE_FILES+=examples/prologue_epilogue.s_3
PROLOGUE_FILES+=examples/prologue_epilogue.s_4
PROLOGUE_FILES+=examples/prologue_epilogue.s_5
ASCIIDOC_FLAGS=--attribute duration=240 --attribute data-uri --attribute stylesheet=prosa.css
SCRIPTS_LOCATION=../../scripts
EIP=$(SCRIPTS_LOCATION)/eip.py
DIS_FUNC=$(SCRIPTS_LOCATION)/disassemble_function.py

.phony: watch clean all encrypt-assignments no_presentation

all: index.html no_presentation

no_presentation: $(ASSIGNMENT_BINARIES)

index.html: index.asciidoc text/*.asciidoc $(EXAMPLE_ASM_FILES) $(SVG_PNG_FILES) $(PROLOGUE_FILES)
	asciidoc $(ASCIIDOC_FLAGS) --backend slidy2 --out-file=- $< | sed -e '/id="footer"/,+6d' -e 's/next_slide(false/next_slide(true/' -e 's/previous_slide(false/previous_slide(true/' -e 's/key == 188/key == 116 || key == 27/' > $@

examples/prologue_epilogue.s_0: examples/prologue_epilogue.s
	$(EIP) 1 < $< > $@
examples/prologue_epilogue.s_1: examples/prologue_epilogue.s
	$(EIP) 2 < $< > $@
examples/prologue_epilogue.s_2: examples/prologue_epilogue.s
	$(EIP) 3 < $< > $@
examples/prologue_epilogue.s_3: examples/prologue_epilogue.s
	$(EIP) 4 < $< > $@
examples/prologue_epilogue.s_4: examples/prologue_epilogue.s
	$(EIP) -4 < $< > $@
examples/prologue_epilogue.s_5: examples/prologue_epilogue.s
	$(EIP) -3 < $< > $@

examples/pic.s: examples/pic.c
	$(DIS_FUNC) $< -fPIC > $@

examples/%.s: examples/%.c
	$(DIS_FUNC) $< > $@

images/%.png: images/%.svg
	inkscape --export-area-drawing --export-png=$@ $<

assignments/%.bin: assignments/%.c
	$(CC) -m32 -fno-stack-protector -o $@ $<

assignments/%.c: assignments/%.gpg
	gpg --passphrase-file passphrase -o $@ $<

encrypt-assignments:
	for file in assignments/*.c; do gpg --symmetric --passphrase-file passphrase --yes -o $${file%.c}.gpg $${file}; done

clean:
	rm -f index.html examples/*.s examples/*.so examples/*.s_*

watch: all
	wait_for_change Makefile *.asciidoc text/*.asciidoc examples/*.c | while read f; do rm -f index.html; make; done
