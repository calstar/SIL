CC=cc
CXX=clang++
RM=rm -f

ifeq (, $(shell which $(CXX)))
$(info $(CXX) compiler not found, trying g++...)
CXX=g++
endif
ifeq (, $(shell which $(CXX)))
$(info $(CXX) compiler not found, trying c++...)
CXX=c++
endif
ifeq (, $(shell which $(CXX)))
$(error No compiler found)
endif

CPPFLAGS= -g -std=c++14 -DSIL -c -I src -I src/lib
LDFLAGS= -g
LDLIBS=

BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
FIRMWAREDIR = $(BUILDDIR)/stripped-firmware

EMPTY_FW_FILE = ./sample_firmware/empty.cc
CODEFILES = code0 code1 code2 code3 code4

SRCS= $(shell find src -type f -name '*.cc') $(addprefix $(FIRMWAREDIR)/,$(addsuffix .cc,$(CODEFILES)))
OBJS= $(addprefix $(OBJDIR)/, $(subst .cc,.o,$(SRCS)))

all: builddir $(CODEFILES) $(BUILDDIR)/sil $(BUILDDIR)/terminal

$(BUILDDIR)/sil: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(BUILDDIR)/sil $(OBJS) $(LDLIBS)

$(BUILDDIR)/terminal: tools/terminal.cc
	$(CXX) -o $@ $<


$(OBJDIR)/%.o: %.cc
	$(CXX) $(CPPFLAGS) -o $@ $<

# After the objdir so that those files won't be caught
code%: # Rebuilds every time because the codeN could change values
	$(if $($@), \
		@echo "================Processing $@ ($($@))=================" && python ./process_firmware.py $(FIRMWAREDIR) $@ $($@), \
		@echo "================Using default file ($(EMPTY_FW_FILE)) for unset $@=================" && python ./process_firmware.py $(FIRMWAREDIR) $@ $(EMPTY_FW_FILE) \
	)
	@cmp --silent $(FIRMWAREDIR)/$@.cc $(FIRMWAREDIR)/$@.cc.new && echo "$@ unchanged..." || cp $(FIRMWAREDIR)/$@.cc.new $(FIRMWAREDIR)/$@.cc

builddir:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(OBJDIR)
	@mkdir -p $(FIRMWAREDIR)
	@$(foreach file,$(dir $(OBJS)),mkdir -p $(file);)

clean:
	$(RM) -r $(BUILDDIR)
