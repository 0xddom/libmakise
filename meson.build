project('makise', 'c', default_options : ['c_std=c11'])

cc = meson.get_compiler('c')

capstone = dependency('capstone', version: '>=3.0.0')
unicorn = dependency('unicorn', version: '=1.0.2')
threads = dependency('threads')
m = cc.find_library('m', required: false)
glib = dependency('glib-2.0')

libmakise_incdir = include_directories('include/makise')
libmakise_src = [
  'libmakise/main.c',
  'libmakise/ga.c',
  'libmakise/mutation.c',
  'libmakise/crossover.c',
  'libmakise/genotype.c',
  'libmakise/parameters.c',
  'libmakise/logging.c',
  'libmakise/signals.c',
  'libmakise/serialization.c'
]
libmakise = static_library('makise', libmakise_src, include_directories: libmakise_incdir,
			   dependencies: [m])

incdir = include_directories('include')

gep_incdir = include_directories('include/gep')
gep_src = [
  'gep/main.c',
  'gep/fenotype.c',
  'gep/tree.c'
]

executable('ones', 'examples/ones.c', include_directories: incdir, link_with: libmakise)
executable('ascii', 'examples/ascii.c', include_directories: incdir, link_with: libmakise)
executable('gep', gep_src, include_directories: [incdir, gep_incdir], link_with: libmakise)

#executable('valid', 'examples/valid.c', include_directories: incdir, link_with: libmakise)
#executable('armgp', 'armgp/main.c', include_directories: incdir, link_with: libmakise,
#	   dependencies: [unicorn, threads, m, glib])
