CC=cc
CXX=clang++
RM=rm -f

CPPFLAGS= -g -std=c++14 -DSIL
LDFLAGS= -g
LDLIBS=

BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
FIRMWAREDIR = $(BUILDDIR)/stripped-firmware

EMPTY_FW_FILE = ./sample_firmware/empty.cc

SRCS= $(wildcard *.cc) $(wildcard includes/*.cc) $(wildcard components/*.cc) $(wildcard $(FIRMWAREDIR)/*.cc)
OBJS= $(addprefix $(OBJDIR)/, $(subst .cc,.o,$(SRCS)))

all: builddir firmware $(BUILDDIR)/sil

firmware: code0 code1 code2 code3 code4

code%: $($@)
	$(if $($@), \
		@echo "================Processing $@ ($($@))=================" && python3 ./process_firmware.py $(FIRMWAREDIR) $@ $($@), \
		@echo "================Using default file ($(EMPTY_FW_FILE)) for unset $@=================" && python3 ./process_firmware.py $(FIRMWAREDIR) $@ $(EMPTY_FW_FILE))

$(BUILDDIR)/sil: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(BUILDDIR)/sil $(OBJS) $(LDLIBS)

$(OBJDIR)/%.o: %.cc
	$(CXX) $(CPPFLAGS) -c -I . -o $@ $<

builddir:
	mkdir -p $(BUILDDIR)
	mkdir -p $(OBJDIR)
	mkdir -p $(FIRMWAREDIR)
	$(foreach file,$(dir $(OBJS)),mkdir -p $(file);)

clean:
	$(RM) -r $(BUILDDIR)
