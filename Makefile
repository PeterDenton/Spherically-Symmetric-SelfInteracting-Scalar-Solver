CXX=g++

CFlags=-c -Wall -O3 -MMD -std=c++11 -Werror
CFlags+=-fext-numeric-literals

GSLFlags=-L/home/pdenton/include/gsl/lib -lgsl -lgslcblas

Sources=$(wildcard src/*.cpp)

IncludeDir=-I./include
IncludeDir+=$(GSLFlags)
IncludeDir+=-I/home/pdenton/include
IncludeDir+=-I/home/pdenton/include/gsl/include

AllObjects=$(addprefix obj/,$(notdir $(Sources:.cpp=.o)))
Executables=main
Objects=$(filter-out $(addprefix obj/,$(Executables:=.o)),$(AllObjects))

all: $(Sources) $(Executables)

$(Executables): $(AllObjects)
	@mkdir -p data obj fig
	$(CXX) $(Objects) $(addprefix obj/,$@.o) $(GSLFlags) -o $@

obj/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFlags) $(IncludeDir) $< -o $@

-include $(AllObjects:.o=.d)

test: $(Executables)
	$(foreach exe,$(Executables),./$(exe);)

clean:
	rm -rf obj/*.o obj/*.d $(Executables)

