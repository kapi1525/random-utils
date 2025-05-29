request_tests = [
    'download',
]

foreach i : request_tests
    test('Request ' + i,executable(i, files(i + '.cpp'), dependencies: rutils_request_dep))
endforeach
