project(
  'amdcalc',
  'cpp',
  default_options: ['cpp_std=c++17'],
)

muparser = subproject('muparser').get_variable('muparser_dep')

amdcalc = static_library('amdcalc', ['src/amdcalc.cpp'], dependencies: [muparser])

executable(
  'amdcalc',
  'src/cli.cpp',
  link_with: amdcalc,
  dependencies: [muparser],
)