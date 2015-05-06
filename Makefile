CC := g++

SRCDIR := src
TESTDIR := tests
BUILDDIR := build
TARGET := bin/runner
TESTTARGET := bin/tester

SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
TESTSOURCES := $(shell find $(TESTDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/main/%,$(SOURCES:.$(SRCEXT)=.o))
TESTOBJECTS := $(patsubst $(TESTDIR)/%,$(BUILDDIR)/tests/%,$(TESTSOURCES:.$(SRCEXT)=.o))
CFLAGS := -std=c++11 -O -Wall -Werror -pedantic
LIB := -Llib
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking main..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/main/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)/main
	@mkdir -p $(@D)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(TESTTARGET): $(filter-out build/main/main.o, $(OBJECTS)) $(TESTOBJECTS)
	@echo " Linking tests..."
	@echo " $(CC) $^ -o $(TESTTARGET) $(LIB)"; $(CC) $^ -o $(TESTTARGET) $(LIB)

$(BUILDDIR)/tests/%.o: $(TESTDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)/tests
	@mkdir -p $(@D)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET) $(TESTTARGET)"; $(RM) -r $(BUILDDIR) $(TARGET) $(TESTTARGET)

tests: $(TESTTARGET)

.PHONY: clean
.PHONY: tests
