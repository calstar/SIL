CC=cc
CXX=clang++
RM=rm -f

CPPFLAGS= -g -std=c++14 -DSIL
LDFLAGS= -g
LDLIBS=

BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
FIRMWAREDIR = firmware

SRCS=Simulator.cc SIL.cc Harness.cc common.cc Output.cc Sensors.cc $(FIRMWAREDIR)/code0.cc $(FIRMWAREDIR)/code1.cc $(FIRMWAREDIR)/code2.cc $(FIRMWAREDIR)/code3.cc $(FIRMWAREDIR)/code4.cc
OBJS=$(addprefix $(OBJDIR)/, $(subst .cc,.o,$(SRCS)))

all: builddir $(BUILDDIR)/sil

$(BUILDDIR)/sil: $(OBJS)
	$(CXX) $(LDFLAGS) -o $(BUILDDIR)/sil $(OBJS) $(LDLIBS)

$(OBJDIR)/%.o: %.cc
	$(CXX) $(CPPFLAGS) -c -I . -o $@ $<

builddir:
	mkdir -p $(BUILDDIR)
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/$(FIRMWAREDIR)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) -r $(BUILDDIR)
