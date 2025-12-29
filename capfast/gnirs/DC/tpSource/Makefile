DIRS = icon seq 
#diags

none:
	@echo "you can make: naac all clean install "

naac all clean install:
	for x in $(DIRS); \
	do \
	    (cd $$x; make $@) \
	done
