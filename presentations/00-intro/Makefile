EXAMPLE_C_FILES=$(wildcard examples/*.c)
EXAMPLE_ASM_FILES=$(EXAMPLE_C_FILES:.c=.s)
SVG_FILES=$(wildcard images/*.svg)
SVG_PNG_FILES=$(SVG_FILES:.svg=.png)
ASCIIDOC_FLAGS=--attribute duration=180 --attribute data-uri --attribute stylesheet=prosa.css

.phony: watch clean all no_presentation

all: index.html

no_presentation:

index.html: index.asciidoc text/*.asciidoc $(EXAMPLE_ASM_FILES) $(SVG_PNG_FILES)
	asciidoc $(ASCIIDOC_FLAGS) --backend slidy2 --out-file=- $< | sed -e '/id="footer"/,+6d' -e 's/next_slide(false/next_slide(true/' -e 's/previous_slide(false/previous_slide(true/' -e 's/key == 188/key == 116 || key == 27/' > $@

examples/liball.so: $(EXAMPLE_C_FILES)
	$(CC) -m32 -fno-stack-protector -shared -fPIC -o $@ $^

examples/%.s: examples/%.c
	./disassemble_function.py $< > $@

images/%.png: images/%.svg
	inkscape --export-area-drawing --export-png=$@ $<

clean:
	rm -f index.html examples/*.s examples/*.so

watch: all
	wait_for_change mystyle.css Makefile *.asciidoc text/*.asciidoc | while read f; do rm -f index.html; make; done
