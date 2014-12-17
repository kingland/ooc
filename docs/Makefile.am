info_TEXINFOS = ooc.texinfo

html-local:
	cp *.png $(HTMLS)
	rm -f $(HTMLS)/logo_big.png
	doxygen Doxyfile.linux

clean-local:
	rm -rf api

install-html-local:
	mkdir -p $(DESTDIR)${docdir}
	cp -r api $(DESTDIR)${docdir}

uninstall-local:
	rm -rf $(DESTDIR)${docdir}/api

EXTRA_DIST = Doxyfile.linux ooc.html api ooc.pdf *.png

dist-hook:
	zip -r ../ooc-doc-$(VERSION).zip ooc.pdf ooc.html api

