basic_test_name = 'Defer basic'
test(basic_test_name, executable(basic_test_name.underscorify(), files('basic.cpp'), dependencies: rutils_base_dep))

fail_test_name = 'Defer fail'
test(fail_test_name, executable(fail_test_name.underscorify(), files('fail.cpp'), dependencies: rutils_base_dep), should_fail: true)