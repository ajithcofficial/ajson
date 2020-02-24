
default:	build

clean:
	rm -rf objs

build:
	test -d objs || mkdir objs
	$(MAKE) -f src/Makefile
	rm -rf objs

