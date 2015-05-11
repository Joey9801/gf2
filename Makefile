CC := g++

SRCDIR := src
TESTDIR := tests
BUILDDIR := build
TARGET := bin/runner
TESTTARGET := bin/tester

SRCEXT := cc
HDREXT := h
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
TESTSOURCES := $(shell find $(TESTDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/main/%,$(SOURCES:.$(SRCEXT)=.o))
TESTOBJECTS := $(patsubst $(TESTDIR)/%,$(BUILDDIR)/tests/%,$(TESTSOURCES:.$(SRCEXT)=.o))
CFLAGS := -std=c++0x -O -Wall -Werror -pedantic
LIB := -Llib
INC := -I include

all: main tests
	$(TESTTARGET)

main: $(TARGET)

tests: $(TESTTARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $(TARGET));
	@echo " Linking main..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

-include $(OBJECTS:.o=.d)
-include $(TESTOBJECTS:.o=.d)

$(BUILDDIR)/main/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)/main
	@mkdir -p $(@D)
	@echo " $(CC) -MM -MT"$@" $(INC) $(CFLAGS) $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/main/$*.d"; $(CC) -MM -MT"$@" $(INC) $(CFLAGS) $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/main/$*.d
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(TESTTARGET): $(filter-out build/main/main.o, $(OBJECTS)) $(TESTOBJECTS)
	@mkdir -p $(dir $(TESTTARGET));
	@echo " Linking main..."
	@echo " Linking tests..."
	@echo " $(CC) $^ -o $(TESTTARGET) $(LIB)"; $(CC) $^ -o $(TESTTARGET) $(LIB)

$(BUILDDIR)/tests/%.o: $(TESTDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)/tests
	@mkdir -p $(@D)
	@echo " $(CC) -MM -MT"$@" $(INC) $(CFLAGS) $(TESTDIR)/$*.$(SRCEXT) > $(BUILDDIR)/tests/$*.d"; $(CC) -MM -MT"$@" $(INC) $(CFLAGS) $(TESTDIR)/$*.$(SRCEXT) > $(BUILDDIR)/tests/$*.d
	@echo " $(CC) $(CFLAGS) $(INC) -I $(SRCDIR) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -I $(SRCDIR) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET) $(TESTTARGET)"; $(RM) -r $(BUILDDIR) $(TARGET) $(TESTTARGET)


.PHONY: all
.PHONY: main
.PHONY: tests
.PHONY: clean
