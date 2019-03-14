CC=cc
CXX=clang++
RM=rm -f

CPPFLAGS= -g -std=c++14 -DSIL -c -I .
LDFLAGS= -g
LDLIBS=

BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
FIRMWAREDIR = $(BUILDDIR)/stripped-firmware

FILL_EMPTY = 1
EMPTY_FIRMWARE = sample_firmware/empty.cc

FIRMWARE_SRCS = $(FIRMWAREDIR)/code0.cc $(FIRMWAREDIR)/code1.cc $(FIRMWAREDIR)/code2.cc $(FIRMWAREDIR)/code3.cc $(FIRMWAREDIR)/code4.cc
SRCS= $(wildcard *.cc) $(wildcard includes/*.cc) $(wildcard components/*.cc) $(FIRMWARE_SRCS)
OBJS= $(addprefix $(OBJDIR)/, $(subst .cc,.o,$(SRCS)))

all: builddir firmware $(BUILDDIR)/sil


ifneq ($(FILL_EMPTY), 0)
ifndef code0
code0 = $(EMPTY_FIRMWARE)
endif
ifndef code1
code1 = $(EMPTY_FIRMWARE)
endif
ifndef code2
code2 = $(EMPTY_FIRMWARE)
endif
ifndef code3
code3 = $(EMPTY_FIRMWARE)
endif
ifndef code4
code4 = $(EMPTY_FIRMWARE)
endif
endif

firmware:
	$(if $(code0),,$(error "code0 is unset"))
	python3 ./process_firmware.py $(FIRMWAREDIR) 0 $(code0)
	$(if $(code1),,$(error "code1 is unset"))
	python3 ./process_firmware.py $(FIRMWAREDIR) 1 $(code1)
	$(if $(code2),,$(error "code2 is unset"))
	python3 ./process_firmware.py $(FIRMWAREDIR) 2 $(code2)
	$(if $(code3),,$(error "code3 is unset"))
	python3 ./process_firmware.py $(FIRMWAREDIR) 3 $(code3)
	$(if $(code4),,$(error "code4 is unset"))
	python3 ./process_firmware.py $(FIRMWAREDIR) 4 $(code4)

$(BUILDDIR)/sil: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(BUILDDIR)/sil $(OBJS) $(LDLIBS)

$(OBJDIR)/%.o: %.cc
	$(CXX) $(CPPFLAGS) -o $@ $<

builddir:
	mkdir -p $(BUILDDIR)
	mkdir -p $(OBJDIR)
	mkdir -p $(FIRMWAREDIR)
	$(foreach file,$(dir $(OBJS)),mkdir -p $(file);)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) -r $(BUILDDIR)
