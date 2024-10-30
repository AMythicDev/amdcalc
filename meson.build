project(
  'amdcalc',
  'cpp',
  default_options: ['cpp_std=c++17'],
)

muparser = subproject('muparser').get_variable('muparser_dep')

executable(
  'amdcalc',
  'src/amdcalc.cpp',
  dependencies: [muparser],
)