CC=cc
CXX=clang++
RM=rm -f

CPPFLAGS= -g -std=c++14 -DSIL
LDFLAGS= -g
LDLIBS=

BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
FIRMWAREDIR = $(BUILDDIR)/stripped-firmware

FIRMWARE = code0 code1 code2 code3 code4

SRCS= *.cc includes/*.cc $(addsuffix .cc, $(addprefix $(FIRMWAREDIR)/, $(FIRMWARE)))
OBJS=$(addprefix $(OBJDIR)/, $(subst .cc,.o,$(SRCS)))

all: clean firmware builddir $(BUILDDIR)/sil
	echo $(SRCS)
	echo $(OBJS)

firmware:
	for file in $(FIRMWARE); do \
		test $(file) || error "$(file) is unset" \
		echo $($(file)) \
	done

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
