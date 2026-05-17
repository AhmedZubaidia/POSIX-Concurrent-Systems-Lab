# ---------------------------------------------------------------------------
# Supermarket Product Shelving Simulation
#
# Builds five cooperating Unix processes. main.c orchestrates the simulation
# and execs the four worker binaries (customer, drawer, shelving_team, timer)
# via relative paths, so every binary must land in the repository root.
# ---------------------------------------------------------------------------

CC       := gcc
CFLAGS   := -g -Wall -Iinclude
PTHREAD  := -lpthread -lrt
GL_LIBS  := -lglut -lGLU -lGL -lm $(PTHREAD)

BINS     := main customer drawer timer shelving_team

.PHONY: all clean

all: $(BINS)

main: src/main.c
	$(CC) $(CFLAGS) $< -o $@

customer: src/customer.c
	$(CC) $(CFLAGS) $< -o $@

drawer: src/drawer.c
	$(CC) $(CFLAGS) $< -o $@ $(GL_LIBS)

timer: src/timer.c
	$(CC) $(CFLAGS) $< -o $@

shelving_team: src/shelving_team.c
	$(CC) $(CFLAGS) $< -o $@ $(PTHREAD)

clean:
	rm -f $(BINS)
