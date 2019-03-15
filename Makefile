CC=cc
CXX=clang++
RM=rm -f

CPPFLAGS= -g -std=c++14 -DSIL -c -I .
LDFLAGS= -g
LDLIBS=

BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
FIRMWAREDIR = $(BUILDDIR)/stripped-firmware

EMPTY_FW_FILE = ./sample_firmware/empty.cc
CODEFILES = code0 code1 code2 code3 code4

SRCS= $(wildcard *.cc) $(wildcard includes/*.cc) $(wildcard components/*.cc) $(wildcard components/controlled/*.cc) $(addprefix $(FIRMWAREDIR)/,$(addsuffix .cc,$(CODEFILES)))
OBJS= $(addprefix $(OBJDIR)/, $(subst .cc,.o,$(SRCS)))

all: builddir $(CODEFILES) $(BUILDDIR)/sil

$(BUILDDIR)/sil: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(BUILDDIR)/sil $(OBJS) $(LDLIBS)

$(OBJDIR)/%.o: %.cc
	$(CXX) $(CPPFLAGS) -o $@ $<

# After the objdir so that those files won't be caught
code%: # Rebuilds every time because the codeN could change values
	$(if $($@), \
		@echo "================Processing $@ ($($@))=================" && python3 ./process_firmware.py $(FIRMWAREDIR) $@ $($@), \
		@echo "================Using default file ($(EMPTY_FW_FILE)) for unset $@=================" && python3 ./process_firmware.py $(FIRMWAREDIR) $@ $(EMPTY_FW_FILE))


builddir:
	mkdir -p $(BUILDDIR)
	mkdir -p $(OBJDIR)
	mkdir -p $(FIRMWAREDIR)
	$(foreach file,$(dir $(OBJS)),mkdir -p $(file);)

clean:
	$(RM) -r $(BUILDDIR)
