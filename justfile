YEAR := "2023"

_default:
    just --list

watch:
    fd ".*c$" -t f | entr ninja -C build test

generate DAY:
    #!/usr/bin/env bash
    set -eu
    just download-puzzle {{ DAY }}
    [[ -d "src/day{{ DAY }}" ]] || {
        cp -av src/template "src/day{{ DAY }}"
        sed -i -e 's/dayXX/day{{ DAY }}/' "src/day{{ DAY }}/solve_test.c"
    }
    grep -q "day{{ DAY }}" meson.build || {
        sed -i "/# XXX: marker/i\  'day{{ DAY }}': [ 'src/day{{ DAY }}/solve.c' ]," meson.build
    }
    [[ -e ./input/day{{ DAY }}.txt ]] || {
        aoc download --year {{ YEAR }} --day {{ DAY }} --input-only --input-file ./input/day{{ DAY }}.txt
    }

submit DAY PART ANSWER:
    aoc submit --year {{ YEAR }} --day {{ DAY }} {{ PART }} {{ ANSWER }}

test:
    ninja -C build test

test-latest:
    #!/usr/bin/env bash
    ninja --quiet -C build
    EXE=$(find build -name "day*_test" -type f -executable | sort -n | tail -n1)
    echo "Running $EXE"
    exec "$EXE"

download-puzzle DAY:
    #!/usr/bin/env bash
    set -eu
    mkdir -p puzzle
    aoc --puzzle-only --year 2023 --day {{ DAY }} > puzzle/day{{ DAY }}.md
