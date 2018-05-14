meta:
  id: makise_state_file
  endian: le
seq:
  - id: header
    type: header
  - id: population
    type: genotype
    repeat: expr
    repeat-expr: header.population_size
types:
  genotype:
    seq:
      - id: dna
        size: _root.header.dna_length * 4
      - id: evaluated
        type: u1
      - id: hits
        type: u4
      - id: fitness_value
        type: f8
  header:
    seq:
      - id: population_size
        type: u4
      - id: seed
        type: u4
      - id: default_seed
        type: u1
      - id: dna_length
        type: u4
      - id: generations
        type: s4
      - id: current_gen
        type: u4
      - id: tournament_size
        type: u4
      - id: use_csv
        type: u1
      - id: mutation_rate
        type: f8
      - id: n_crossover_funcs
        type: u4
      - id: crossover_func_names
        type: strz
        encoding: ascii
        repeat: expr
        repeat-expr: n_crossover_funcs
      - id: n_mutation_funcs
        type: u4
      - id: mutation_func_names
        type: strz
        encoding: ascii
        repeat: expr
        repeat-expr: n_mutation_funcs
