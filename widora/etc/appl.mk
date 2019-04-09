ifeq "${TOP_DIR}" ""
TOP_DIR = /widora
endif 

CWD = $(shell pwd)
TARGET = $(shell basename $(CWD))
HOSTNAME = $(shell basename `cd $(LEVEL); pwd`)
SRC_PATH := $(subst Build/$(HOSTNAME)/,,$(CWD))

$(warning "the value of CWD is $(CWD)")
$(warning "the value of TARGET is $(TARGET)")
$(warning "the value of HOSTNAME is $(HOSTNAME)")
$(warning "the value of SRC_PATH is $(SRC_PATH)")

include $(TOP_DIR)/etc/$(HOSTNAME).mk
# include the source list file if the dir is not lib dir
exist = $(shell if [ -f $(SRC_PATH)/source.mk ]; then echo "exist"; else ech    o "notexist"; fi;)
ifeq "$(exist)" "exist"
include $(SRC_PATH)/source.mk
endif

#error code.
ifneq "${EDFILE}" ""
EDBASE = $(basename $(notdir $(EDFILE)))
EDSRC = $(EDBASE:%=%.cpp)
SRCS += $(EDSRC)
$(warning "the value of SRCS is $(SRCS)")
endif
                                                                       
SRCBASE = $(basename $(SRCS))
$(warning "the value of SRCBASE is $(SRCBASE)")
OBJS = $(SRCBASE:%=%.o)
$(warning "the value of OBJS is $(OBJS)")
OBJ_FILE := $(foreach file, $(OBJS), $(shell basename $(file)) )
DEPENDS := $(SRCBASE:%=%.d)

CC = gcc
CPP = g++
LD = g++
INCDIRS += -I. -I$(SRC_PATH)/.

$(warning "debug build111111111111111111111...")

.PHONY : clean libraries program programtest
all:: libraries program

$(warning "debug build222222222222222222222...")

.SUFFIXES: .c .cpp .cc .o
.c.o:
	$(CC) -c ${CFLAGS} ${INCDIRS} $< 
	$(warning "the value of CC is $(CC)")
.cc.o:
	${CPP} -c ${CPPFLAGS} $(INCDIRS) $< 
	$(warning "the value of CPP is $(CPP)")
.cpp.o:
	${CPP} -c ${CPPFLAGS} $(INCDIRS) $< 
	$(warning "the value of C++ is $(CPP)")

$(warning "debug build333333333333333333333...")

libraries:: $(LIB_TARGET)
ifeq "$(LIBRARY)" ""
$(LIB_TARGET):
	${NOTHING}
else
$(LIB_TARGET):$(OBJS)
	@echo $(SRC_PATH)
	$(AR) cr $(LIB_TARGET) $^
	cp -f $(LIB_TARGET) $(LIB_DIR)
	
clean::
	rm -f $(LIB_DIR)/$(LIB_TARGET)
	rm -f $(TEST_REPORT_DIR)/$(PROGRAM)*.xml
	rm -rf .cccc
endif

$(warning "debug build444444444444444444444...")

program:: $(PROGRAM)
ifeq "$(PROGRAM)" ""
$(PROGRAM)::
	${NOTHING}
else
$(PROGRAM)::$(OBJS)
	$(warning "start build...")
	${CPP} -c ${CPPFLAGS} $(INCDIRS)
	${CC} -c ${CFLAGS}
	$(LD) -o $(PROGRAM) $(OBJS) $(LDFLAGS) 
endif