project(
  'amdcalc',
  'cpp',
  default_options: ['cpp_std=c++17'],
)

muparser = subproject('muparser').get_variable('muparser_dep')
cpp_terminal = subproject('cpp-terminal').get_variable('cpp_terminal_dep')
eigen = subproject('eigen').get_variable('eigen_dep')

deps = [muparser, eigen]
cpp_args = []

if get_option('use_lapacke')
  lapacke = dependency('lapacke', required: get_option('use_lapacke'))
  deps += lapacke
  cpp_args += '-DEIGEN_USE_LAPACKE=1'
endif

amdcalc = static_library(
  'amdcalc',
  ['src/amdcalc/amdcalc.cpp'],
  dependencies: deps,
  cpp_args: cpp_args,
)

executable(
  'amdcalc',
  ['src/cli.cpp', 'src/prompt.cpp'],
  link_with: amdcalc,
  dependencies: deps + cpp_terminal,
  cpp_args: cpp_args,
)