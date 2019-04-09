TOP_DIR= $(join $(shell pwd),/..)
include $(TOP_DIR)/etc/mips.mk

all program libraries module:
	make --directory=Build/widora $@

clean:
	make --directory=Build/widora $@

release_program:
	make --directory=Build/widora/Appl $@
	
