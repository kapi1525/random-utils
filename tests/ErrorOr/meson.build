error_or_tests = [
    'basic_type',
    'class',
    'class2',
    'void',
    'void2',
    'move_copy_count',
]

foreach i : error_or_tests
    test_name = 'ErrorOr ' + i
    
    test(test_name, executable(test_name.underscorify(), files(i + '.cpp'), dependencies: rutils_base_dep))
endforeach
