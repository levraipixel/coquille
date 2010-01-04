SHELLDIR = shell

all:
	sh $(SHELLDIR)/compile.sh

doc:
	sh $(SHELLDIR)/doc.sh

clean:
	sh $(SHELLDIR)/clean.sh

deb:
	sh $(SHELLDIR)/deb.sh

