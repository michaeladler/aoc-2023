project('aoc-2023', 'c', version: '0.1', default_options: ['c_std=gnu11', 'warning_level=3', 'b_ndebug=if-release'])

add_project_arguments([
  '-march=native',
  '-mtune=native',
  '-Wno-language-extension-token', # can be removed once we switch to C23
  '-Wno-gnu-statement-expression',
  '-Wno-c11-extensions',
  '-D_GNU_SOURCE'
], language : 'c')

if get_option('have-inputs')
  add_project_arguments('-DHAVE_INPUTS', language : 'c')
endif

c = meson.get_compiler('c')

inc_dirs = ['include', 'vendor/ctl', 'vendor/sort', 'vendor/stb', 'vendor/log', 'vendor/str', 'vendor/hashmap']

aoc_lib = static_library(
    'aoc_lib',
    'vendor/str/str.c',
    'vendor/log/log.c',
    'vendor/hashmap/hashmap.c',
    'lib/aoc/io.c',
    'lib/aoc/math.c',
    'lib/aoc/parser.c',
    'lib/aoc/point.c',
    'lib/aoc/string.c',
    'vendor/stb/stb_sprintf.c',
    include_directories: include_directories(inc_dirs)
)

days = {
  'day01': [ 'src/day01/solve.c' ],
  'day02': [ 'src/day02/solve.c' ],
  'day03': [ 'src/day03/solve.c' ],
  'day04': [ 'src/day04/solve.c', 'src/day04/sort.c' ],
  'day05': [ 'src/day05/solve.c' ],
  # XXX: marker
}

foreach day, sources : days
  executable(day,
    [ 'src/main.c' ] + sources,
    c_args: [f'-DDAY="@day@"'],
    link_with: aoc_lib,
    include_directories: include_directories(inc_dirs))

  test(day,
    executable(f'@day@_test',
      [ f'src/@day@/solve_test.c'] + sources,
      c_args: [f'-DDAY="@day@"'],
      link_with: aoc_lib,
      include_directories: include_directories(inc_dirs + ['vendor/ctest']))
  )
endforeach
