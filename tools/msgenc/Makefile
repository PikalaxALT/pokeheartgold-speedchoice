CXX := g++
OPTFLAGS = -O2 -DNDEBUG
ifneq ($(DEBUG),)
OPTFLAGS = -g
endif
CXXFLAGS := -std=c++17 $(OPTFLAGS) -Wall -Wno-switch -Wno-unused-but-set-variable
LDFLAGS  :=


DEPDIR := .deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

SRCS := \
	msgenc.cpp \
	Options.cpp \
	MessagesConverter.cpp \
	MessagesDecoder.cpp \
	MessagesEncoder.cpp \
	Gmm.cpp \
	pugixml.cpp

OBJS := $(SRCS:%.cpp=%.o)

.PHONY: all clean

all: msgenc
	@:

clean:
	$(RM) -r msgenc msgenc.exe $(OBJS) $(DEPDIR)

msgenc: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
%.o: %.cpp $(DEPDIR)/%.d | $(DEPDIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c -o $@ $<

$(DEPDIR): ; @mkdir -p $@

DEPFILES := $(SRCS:%.cpp=$(DEPDIR)/%.d)
$(DEPFILES):

include $(wildcard $(DEPFILES))
