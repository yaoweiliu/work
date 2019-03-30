TOP_DIR= $(join $(shell pwd),/..)
include $(TOP_DIR)/etc/mips.mk

all program libraries module:
	make --directory=Build/Widora $@

clean:
	make --directory=Build/Widora $@

release_program:
	make --directory=Build/Widora/Appl $@
	
