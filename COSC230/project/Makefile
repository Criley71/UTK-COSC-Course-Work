CXX=g++
CXXFLAGS=-g -O0
LDFLAGS=-g -O0
SOURCES=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,objs/%.o,$(SOURCES))
OUT=project

all: $(OUT)

include $(wildcard deps/objs/*.d)

$(OUT): $(OBJS) Makefile
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

objs/%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MD -MF ./deps/$(patsubst %.o,%.d,$@) -o $@ -c $<

.PHONY: clean

clean:
	rm -f $(OUT) objs/*.o deps/objs/*.d

