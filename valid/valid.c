#include <makise.h>
#include <capstone/capstone.h>
#include <stdio.h>

#define BASE 0
#define ARM_INSN_SIZE_BYTES 4

void makise_eval_one_genotype(Genotype *g) {
  int c;
  size_t i;
  cs_insn *insn;
  csh cs_handle;
  
  if (!g->evaluated) {
    g->evaluated = true;

    if ((c = cs_open (CS_ARCH_ARM, CS_MODE_ARM, &cs_handle)) != CS_ERR_OK) {
      fprintf (stderr, "Can't load capstone");
      exit (c);
    }

    i = cs_disasm (cs_handle, g->dna, g->length, 0, 0, &insn);
    if (i < 0) i = 0;
    g->fitness.hits = i;
    g->fitness.value = (double)g->fitness.hits / (g->length / ARM_INSN_SIZE_BYTES);
    
    cs_free (insn, i);
    cs_close (&cs_handle);
  }
}
