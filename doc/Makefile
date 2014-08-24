# Makefile rules to generate html from markdown using pandoc
# Note that pandoc supports most of the extra markup supported
# by github

HTML=$(patsubst %.md,%.html,$(wildcard *.md))

MARKDOWN=pandoc --from markdown_github-hard_line_breaks --to html --standalone --number-sections

%.html: %.md
	$(MARKDOWN) -o $@ $<

all: $(HTML)

clean:
	rm -f *.html

