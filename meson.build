project(
  'amdcalc',
  'cpp',
  default_options: ['cpp_std=c++17', 'warning_level=everything'],
)

decimal = subproject('CppDecimal')
decimal_dep = decimal.get_variable('decimal_dep')

executable(
  'amdcalc',
  [
    'src/amdcalc.cpp',
    'src/scanner.cpp',
    'src/solvetree.cpp',
    'src/parser.cpp',
  ],
  dependencies: decimal_dep,
)

# testrunner = executable(
#   'testrunner',
#   ['src/tests.cpp', 'src/scanner.cpp', 'src/solvetree.cpp', 'src/parser.cpp'],
# )

# test('amdcalc test', testrunner)