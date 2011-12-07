CC = g++
LD= ld
LD_COMBINE = -Ur
DEBUG_OPTS = -g3 -Wall
RELEASE_OPTS = -O2
OPTS = $(DEBUG_OPTS) -D__ABSTRACT_PEER__
PROJECT_DIR = /opt/sean/workspace/SLAAP/
INCLUDE = -I$(PROJECT_DIR)/src -I./

OBJS = Tracker.o TrackerRequest.o TrackerResponse.o PeerInfo.o

clean:
	rm -f *.o *~

all: $(OBJS)

%.o: %.c++ %.h++
	$(CC) $(INCLUDE) -c $< $(OPTS)
