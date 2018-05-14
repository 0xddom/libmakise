#include <makise.h>
#include <capstone/capstone.h>
#include <unicorn/unicorn.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BASE 0x100000
#define INPUT_ADDR 0x300000
#define OUTPUT_ADDR 0x400000
#define STACK_ADDR 0x500000
#define ARM_INSN_SIZE_BYTES 4
#define EMU_MEM_SIZE 10 * 1024 * 2014 /* 10 MB */

bool is_valid_code(Genotype *);
void *emulated_memcpy(Genotype *, void *, void *, size_t);

typedef struct armcpu_t {
  uint32_t regs[UC_ARM_REG_ENDING];
} ARMCPU;

void makise_eval_one_genotype(Genotype *g) {
  void *dest, *r;
  int i;
  
  if (!g->evaluated) {
    g->evaluated = true;

    if (!is_valid_code (g)) {
      g->fitness.hits = 0;
      g->fitness.value = 0.0;
      return;
    }

    dest = (void*)malloc (g->length);
    r = emulated_memcpy (g, dest, g->dna, g->length);

    if (r == NULL) {
      g->fitness.hits = 0;
      g->fitness.value = 0.0;
      goto cleanup;
    }
    
    for (i = 0; i < g->length; i++) {
      if (((uint8_t*)dest)[i] == g->dna[i]) g->fitness.hits++;
    }
    g->fitness.value = (double)g->fitness.hits / g->length;

  cleanup:
    free (dest);
    return;
  }
}


void *emulated_memcpy(Genotype *g, void *dest, void *src, size_t n) {
#if 0
  return memcpy (dest, src, n);
#endif

#define r0 cpu.regs[UC_ARM_REG_R0]
#define r1 cpu.regs[UC_ARM_REG_R1]
#define r2 cpu.regs[UC_ARM_REG_R2]
#define sp cpu.regs[UC_ARM_REG_R13]
  
  uc_err err;
  uc_engine *uc;
  ARMCPU cpu, caller_cpu;
  void *ret;
  int i;

  err = uc_open (UC_ARCH_ARM, UC_MODE_ARM, &uc);
  if (err != UC_ERR_OK) {
    fprintf (stderr, "Can't load unicorn");
    exit ((int)err);
  }
    
  // Map memory
  uc_mem_map (uc, BASE, EMU_MEM_SIZE, UC_PROT_ALL);
  
  if (uc_mem_write (uc, BASE, g->dna, g->length)) {
    ret = NULL;
    goto cleanup;
  }

  if (uc_mem_write (uc, INPUT_ADDR, src, n)) {
    ret = NULL;
    goto cleanup;
  }

#if 0
  if (uc_mem_write (uc, OUTPUT_ADDR, src, n)) {
    ret = NULL;
    goto cleanup;
  }
#endif

  // Setup CPU registers
  for (i = 0; i < UC_ARM_REG_ENDING; i++) {
    // A random value representing an arbitrary caller.
    cpu.regs[i] = caller_cpu.regs[i] = (uint32_t)rand (); 
  }
  // Set here input values for registers
  r0 = OUTPUT_ADDR;
  r1 = INPUT_ADDR;
  r2 = n;
  sp = STACK_ADDR;
  
  // Load the registers in the CPU
  for (i = 0; i < UC_ARM_REG_ENDING; i++) {
    uc_reg_write (uc, i, &cpu.regs[i]);
  }

  // Start emulation
  err = uc_emu_start (uc, BASE, BASE + g->length, 0, g->length * n * n);
  if (err) {
    ret = NULL;
    goto cleanup;
  }

  // Reload registers
  for (i = 0; i < UC_ARM_REG_ENDING; i++) {
    uc_reg_read (uc, i, &cpu.regs[i]);
  }
  
  // Sanity checks
  for (i = UC_ARM_REG_R4; i <= UC_ARM_REG_R11; i++) {
    if (cpu.regs[i] != caller_cpu.regs[i]) {
      ret = NULL;
      goto cleanup;
    }
  }
  if (r0 != OUTPUT_ADDR) {
    ret = NULL;
    goto cleanup;
  }
  
  err = uc_mem_read (uc, OUTPUT_ADDR, dest, n);
  if (err) {
    ret = NULL;
    goto cleanup;
  }

  ret = dest;

 cleanup:
  if (uc) uc_close (uc);
  return ret;
}

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
  cs_close (&cs_handle);
  
  return (i * ARM_INSN_SIZE_BYTES) == g->length;
}
