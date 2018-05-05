project('makise', 'c')

capstone = dependency('capstone', version: '>=3.0.0')
unicorn = dependency('unicorn', version: '=1.0.2')

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
libmakise = static_library('makise', libmakise_src, include_directories: libmakise_incdir, dependencies: capstone)

incdir = include_directories('include')

executable('example', 'example/example.c', include_directories: incdir, link_with: libmakise)
executable('ascii', 'ascii/ascii.c', include_directories: incdir, link_with: libmakise)
executable('valid', 'valid/valid.c', include_directories: incdir, link_with: libmakise)
executable('armgp', 'armgp/main.c', include_directories: incdir, link_with: libmakise,
	   dependencies: [unicorn])
