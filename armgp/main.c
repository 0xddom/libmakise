#include <makise.h>
#include <capstone/capstone.h>
#include <unicorn/unicorn.h>
#include <stdbool.h>

#define BASE 0x1000000

bool is_valid_code(Genotype *g) {
  int c;
  size_t i;
  cs_insn *insn;
  csh cs_handle;

   if ((c = cs_open (CS_ARCH_ARM, CS_MODE_ARM, &cs_handle)) != CS_ERR_OK) {
     fprintf (stderr, "Can't load capstone");
     exit (c);
   }

   i = cs_disasm (cs_handle, g->dna, g->length, BASE, 0, &insn);

   cs_free (insn, i);
   cs_close (cs_handle);
   
   return i >= 0;
}

void makise_eval_one_genotype(Genotype *g) {
  uc_engine *uc;
  uc_err err;
  
  if (!g->evaluated) {
    g->evaluated = true;

    if (!is_valid_code (g)) {
      g->fitness.hits = 0;
      g->fitness.value = 0.0;
      return;
    }

    

  }
}
