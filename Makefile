WARNINGS :=	-Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
		-Wwrite-strings -Wredundant-decls -Wnested-externs -Winline \
					-Wno-long-long -Wuninitialized

CC=gcc
CFLAGS +=-c -std=gnu17 -MMD -MP
LDFLAGS +=
PROJDIRS=src/
BINBASEDIR=build
BINDIRS=$(patsubst %,$(BINBASEDIR)/%,$(PROJDIRS))
SRCFILES=$(shell find $(PROJDIRS) -type f -name "*.c")
HDRFILES=$(shell find $(PROJDIRS) -type f -name "*.h")
OBJFILES=$(patsubst %.c,$(BINBASEDIR)/%.o,$(SRCFILES))
DEPFILES=$(patsubst %.c,$(BINBASEDIR)/%.d,$(SRCFILES))
ALLFILES=$(SRCFILES) $(HDRFILES)
TARGET=$(BINBASEDIR)/ghost-sh

.PHONY: all clean todolist make-tests test install

all: $(TARGET)

$(TARGET): make-tests $(BINDIRS) $(OBJFILES)
	$(CC) $(LDFLAGS) $(OBJFILES) -o $(TARGET)
	@echo "Ghost Shell built. Run with: "
	@echo $(TARGET)
	@echo "make test to check it"
	@echo "make install to install it"


$(BINBASEDIR)/%.o: %.c Makefile
	$(CC) $(CFLAGS) $(WARNINGS) $< -o $@

$(BINDIRS):
	@mkdir -p -v $(BINDIRS)

todolist:
	-@echo > TODO && for file in $(ALLFILES:Makefile=); do fgrep -H -e TODO -e FIXME $$file >> TODO; done; true

clean:
	-rm -rvf $(BINBASEDIR)

print:
	@echo $(ALLFILES)

make-tests:
	@echo "Generating test files"
	./make-tests > $(PROJDIRS)/AllTests.c

test: $(TARGET)
	@$(TARGET) --self-check

install:
	cp $(TARGET) /usr/bin/ghost-sh

-include $(DEPFILES)
