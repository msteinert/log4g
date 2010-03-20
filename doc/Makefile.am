#                                        -*- Automake -*-
# Process this file with automake to produce Makefile.in.

# add `doc' target
doc: Doxyfile
	@if [ -z $(DOXYGEN) ]; then \
		echo "make doc: Doxygen is not installed (http://www.doxygen.org/)"; \
	else \
		$(DOXYGEN) $(DOXYFLAGS) Doxyfile; \
	fi;
.PHONY: doc
# cleanup
clean-local:
	-rm -rf html
MAINTAINERCLEANFILES = Makefile.in Doxyfile
