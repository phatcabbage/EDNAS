CC = g++
LD = ld
LD_COMBINE = -Ur
DEBUG_OPTS = -g3 -Wall
RELEASE_OPTS = -O2
OPTS = $(DEBUG_OPTS)
PROJECT_DIR = /opt/sean/workspace/SLAAP
INCLUDE = -I$(PROJECT_DIR)/include

OBJS = ChunkRequest.o DownloadManager.o Peer.o PeerController.o PeerGroup.o  PeerMessages.o PeeringManager.o PeeringTimers.o UploadManager.o

OBJ_NAME = PeerDir.o

clean:
	rm *.o

all: $(OBJ_NAME)

$(OBJ_NAME): $(OBJS)
	$(LD) $(LD_COMBINE) $(OBJS) -o $(OBJ_NAME)

%.o: %.c++
	$(CC) $(INCLUDE) -c $< $(OPTS)