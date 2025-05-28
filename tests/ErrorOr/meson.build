error_or_tests = [
    'basic_type',
    'class',
    'class2',
    'void',
    'void2',
    'move_copy_count',
]

foreach i : error_or_tests
    test('ErrorOr ' + i,executable(i, files(i + '.cpp'), dependencies: rutils_base_dep))
endforeach
