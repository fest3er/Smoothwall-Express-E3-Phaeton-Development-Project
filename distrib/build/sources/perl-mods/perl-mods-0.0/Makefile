# SmoothWall Build System
#
# (c) SmoothWall Ltd 2005
#
# This code isdistributed under the terms of the GPL v2.

all:
	@+for i in $(PERL_MODS); do \
	  make -C $$i install; \
	done

download:
	@+for i in $(PERL_MODS); do \
	  echo; \
	  echo "$$i"; \
	  make 
	      -C $$i download; \
	done

clean:
	@+for i in $(PERL_MODS); do \
	  make --no-print-directory -C $$i clean; \
	done >/dev/null 2>&1
