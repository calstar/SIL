CC=cc
CXX=clang++
RM=rm -f

CPPFLAGS= -g -std=c++14 -DSIL
LDFLAGS= -g
LDLIBS=

BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
FIRMWAREDIR = $(BUILDDIR)/stripped-firmware

SRCS= $(wildcard *.cc) $(wildcard includes/*.cc) $(wildcard components/*.cc) $(wildcard $(FIRMWAREDIR)/*.cc)
OBJS= $(addprefix $(OBJDIR)/, $(subst .cc,.o,$(SRCS)))

all: clean builddir firmware $(BUILDDIR)/sil

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
	$(CXX) $(CPPFLAGS) -c -I . -o $@ $<

builddir:
	mkdir -p $(BUILDDIR)
	mkdir -p $(OBJDIR)
	mkdir -p $(FIRMWAREDIR)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) -r $(BUILDDIR)
