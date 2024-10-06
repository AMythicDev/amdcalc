project(
  'amdcalc',
  'cpp',
  default_options: ['cpp_std=c++17', 'warning_level=everything'],
)

executable(
  'amdcalc',
  [
    'src/amdcalc.cpp',
    'src/scanner.cpp',
    'src/solvetree.cpp',
    'src/parser.cpp',
  ],
)

testrunner = executable(
  'testrunner',
  ['src/tests.cpp', 'src/scanner.cpp', 'src/solvetree.cpp', 'src/parser.cpp'],
)

test('amdcalc test', testrunner)
