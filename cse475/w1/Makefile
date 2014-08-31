#
#  Origin:  Lehigh CSE Department
#  Course:  CSE 375/475
#  Date:    2013-9-01
#  Version: 1
#
#  Description: A basic makefile for generating pdf files from LaTeX documents
#

#
# Name any .tex or .bib files that we need to use
#
TEXFILES = w1.tex w1_supp.tex
BIBFILES = cse375.bib

#
# Provide the name of the .tex file that is our starting point.  This will
# also determine the name of our final output file
#
BASENAME = w1

#
# This is the file we'll create
#

#
# Standard targets
#
all: $(BASENAME).pdf

clean:
	@rm -f *.aux *.bbl *.blg *.log

realclean: clean
	@rm -f $(BASENAME).pdf

#
# LaTeX is a bit annoying, in that when we have a bibliography we have to
# invoke code four times...
#
$(BASENAME).pdf: $(TEXFILES) $(BIBFILES)
	@pdflatex $(BASENAME)
	@bibtex $(BASENAME)
	@pdflatex $(BASENAME)
	@pdflatex $(BASENAME)

.PHONY: all clean realclean
