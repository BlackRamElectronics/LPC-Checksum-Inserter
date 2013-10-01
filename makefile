# I am a comment, and I want to say that the variable CC will be
# the compiler to use.
CC=gcc

BUILD_DIR = Debug

LIBS =
LIBS_DIR =
INCLUDES=

CFLAGS=-c -Wall $(INCLUDES)
CSOURCES= LPC-Checksum-Inserter.c
CPPSOURCES= 

OBJECTS=${CPPSOURCES:.cpp=.o} ${CSOURCES:.c=.o}
OBJECTS_DEBUG=$(addprefix ${BUILD_DIR}/,$(CSOURCES:.c=.o)) $(addprefix ${BUILD_DIR}/,$(CPPSOURCES:.cpp=.o))
VPATH = Debug

.PHONY: directories

all: directories $(OBJECTS)
	echo "Linking $(OBJECTS_DEBUG)"
	$(CC) $(OBJECTS_DEBUG) -o LPC-Checksum-Inserter $(LIBS_DIR) $(LIBS)

%.o: %.c
	echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o ${BUILD_DIR}/$@
	
%.o: %.cpp
	echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o ${BUILD_DIR}/$@
	
%dir: %.c
	echo "Generating dir $(subst /,\,$(dir ${BUILD_DIR}\$@))"
	mkdir $(subst /,\,$(dir ${BUILD_DIR}\$@))
	
directories: ${OUT_DIR}

${OUT_DIR}:
#        ${MKDIR_P} ${OUT_DIR}
	echo "Generating dir $(subst /,\,$(dir ${BUILD_DIR}\$@))"
	mkdir $(subst /,\,$(dir ${BUILD_DIR}\$@))

clean:
	rm -rf *o AtDev
	rm $(OBJECTS_DEBUG)
