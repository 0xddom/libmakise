meta:
  id: makise_state_file
  endian: le
seq:
  - id: population_size
    type: u4
  - id: seed
    type: u4
  - id: dna_length
    type: u4
  - id: default_seed
    type: u1
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
  - id: crossover_func_name
    type: strz
    encoding: ascii
  - id: mutation_func_name
    type: strz
    encoding: ascii
  - id: population
    type: genotype
    repeat: expr
    repeat-expr: population_size
types:
  genotype:
    seq:
      - id: dna
        size: _root.dna_length * 4
      - id: evaluated
        type: u1
      - id: hits
        type: u4
      - id: fitness_value
        type: f8
