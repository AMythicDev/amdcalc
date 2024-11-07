project(
  'amdcalc',
  'cpp',
  default_options: ['cpp_std=c++17'],
)

muparser = subproject('muparser').get_variable('muparser_dep')
cpp_terminal = subproject('cpp-terminal').get_variable('cpp_terminal_dep')
eigen = subproject('eigen').get_variable('eigen_dep')

amdcalc = static_library('amdcalc', ['src/amdcalc.cpp'], dependencies: [muparser, eigen])

executable(
  'amdcalc',
  'src/cli.cpp',
  link_with: amdcalc,
  dependencies: [muparser, cpp_terminal, eigen],
)