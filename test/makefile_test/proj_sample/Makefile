subdirs = \
		  sources\

.PHONY: all clean help $(subdirs)

all clean help: $(subdirs)

$(subdirs):
	make -C $@ $(MAKECMDGOALS)
