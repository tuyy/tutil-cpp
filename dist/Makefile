# vim 설정
# vim:ts=4:noexpandtab:shiftwidth=4:fileencoding=utf-8:

.PHONY: pkg all clean dist

NOW         = $(shell date '+%Y.%m%d.%H%M')

PKG_NAME = tuyy-bin-$(NOW).tar.bz2

SCRIPTS_TO_PACK = run.sh

dist:
	$(info ===================================)
	$(info Installing files into dist directory)
	$(info ===================================)
	$(RM) -r conf bin lib
	mkdir -p conf
	mkdir -p bin
	mkdir -p lib
	cp ../conf/*                                                                     conf/
	cp ../build/main                                                                 bin/

pkg:
	$(info ===================================)
	$(info Archiving files to make package $(PKG_NAME))
	$(info ===================================)
	$(RM) tuyy-*.tar.bz2
	@echo "============================================================="
	@echo "Archiving files to make final tutil package."
	@echo "----> $(PKG_NAME)"
	@echo "============================================================="
	tar cjvf $(PKG_NAME)_TEMP $(SCRIPTS_TO_PACK) conf/ bin/ lib/
	cat $(PKG_NAME)_TEMP > $(PKG_NAME)
	chmod +x $(PKG_NAME)
	rm -f $(PKG_NAME)_TEMP

run: dist
	./run.sh

clean:
	$(RM) -r conf bin lib *.txt
	$(RM) *TEMP
	$(RM) tuyy-bin-*.tar.bz2

