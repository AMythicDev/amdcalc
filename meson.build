project(
  'amdcalc',
  'cpp',
  default_options: ['cpp_std=c++17'],
)

decimal = subproject('CppDecimal').get_variable('decimal_dep')
doctest = subproject('doctest').get_variable('doctest_dep')

amdcalc = static_library(
  'amdcalc',
  [
    'src/scanner.cpp',
    'src/solvetree.cpp',
    'src/parser.cpp',
  ],
  dependencies: decimal,
)

executable('amdcalc', 'src/amdcalc.cpp', link_with: amdcalc, dependencies: decimal)

testrunner = executable(
  'amdcalc_test',
  'src/tests.cpp',
  dependencies: [doctest, decimal],
  link_with: amdcalc,
)

test('Amdcalc test', testrunner)