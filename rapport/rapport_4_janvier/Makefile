MAIN = main



MAIN_TEX = $(MAIN).tex
MAIN_PDF = $(MAIN).pdf

all: cleaned
	evince $(MAIN_PDF) &

cleaned: main
	rm -f *.ps *.dvi *.aux *.out *.toc *.log *.lof *.lot *~

main: $(MAIN_TEX)
	pdflatex $(MAIN_TEX)
	bibtex $(MAIN)
	pdflatex $(MAIN_TEX)
	bibtex $(MAIN)
	pdflatex $(MAIN_TEX)
	bibtex $(MAIN)

clean:
	rm -f *.pdf *.ps *.dvi *.aux *.out *.toc *.log *.lof *.lot *.bbl *.blg *~
