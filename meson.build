project('makise', 'c')

libmakise_incdir = include_directories('include/makise')
libmakise_src = [
  'libmakise/main.c',
  'libmakise/ga.c',
  'libmakise/mutation.c',
  'libmakise/crossover.c',
  'libmakise/genotype.c',
  'libmakise/parameters.c',
  'libmakise/logging.c'
]
libmakise = static_library('makise', libmakise_src, include_directories: libmakise_incdir)

incdir = include_directories('include')
executable('example', 'example/example.c', include_directories: incdir, link_with: libmakise)
