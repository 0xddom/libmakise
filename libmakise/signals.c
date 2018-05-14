#include <signal.h>
#include <signals.h>
#include <stdlib.h>
#include <serialization.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errors.h>

#define STATE_PATH "./makise.state"

static Parameters *params_ptr;
static Problem *problem_ptr;

static void sigint_handler(int signum) {
  int err;
  
  err = serialize_program_state_to_file (params_ptr, problem_ptr, STATE_PATH, false);
  
  fprintf (stderr, "\nThe program state was saved.\n"
	   "You can rerun the program using this data with:\n"
	   "%s --restore="STATE_PATH"\n", params_ptr->prog_name);
  
  exit (err);
}

static void sigabrt_handler(int signum) {
  char tmpname[4096];
  void *data;
  int fd, size;
  char *args[3];
  
  fprintf (stderr, "The program aborted. Trying to recover and restart...\n");

  strcpy (tmpname, "/tmp/makise_XXXXXX");
  fd = mkstemp (tmpname);
  
  fprintf (stderr, "Using the path %s to relaunch the program\n", params_ptr->prog_name);
  fprintf (stderr, "Saving state to %s\n", tmpname);

  size = serialize_program_state (params_ptr, problem_ptr, &data, true);

  if (size < 0) {
    fprintf (stderr, "Can't serialize the program.\n");
    close (fd);
    exit (ALLOC_ERROR);
  }

  if (write (fd, data, size) < 0) {
    fprintf (stderr, "Can't write the state.\n");
    close (fd);
    exit (ALLOC_ERROR);
  }

  close (fd);

  fprintf (stderr, "Everything seems ok. Restarting the program...\n");
  
  args[0] = params_ptr->prog_name;
  args[1] = "-R";
  args[2] = tmpname;

  execve (args[0], args, NULL);
  
}

void setup_signals(Parameters *params, Problem *p) {
  params_ptr = params;
  problem_ptr = p;
  
  signal (SIGINT, sigint_handler);
  signal (SIGTERM, sigint_handler);
  signal (SIGABRT, sigabrt_handler);
}
