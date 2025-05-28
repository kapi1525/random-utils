project(
    'random-utils',
    'cpp', 'c',
    version: '0.1.0',
    license: 'MIT',
    default_options: {
        'cpp_std':          'c++20',
        'c_std':            'c11',
        'default_library':  'static',
        'prefer_static':    'true',
        'warning_level':    '1',
    },
    meson_version: '>=1.2',
)



subdir('src')
inc = include_directories('inc')


base_deps = [
    dependency('threads')
]


request_deps = [
    # TODO: Make curl dependencies static
    subproject(
        'curl',
        default_options: {
            'tool': 'disabled',
            'tests': 'disabled',
            'unittests': 'disabled',
        },
    ).get_variable('curl_dep'),
]


rutils_base_lib = library(
    'rutils-base',
    sources: base_src,
    include_directories: inc,
    dependencies: base_deps,
    version: meson.project_version(),
)

rutils_base_dep = declare_dependency(
    link_with: rutils_base_lib,
    include_directories: inc,
)


rutils_request_lib = library(
    'rutils-request',
    sources: request_src,
    include_directories: inc,
    dependencies: [base_deps, request_deps, rutils_base_dep],
    version: meson.project_version(),
)

rutils_request_dep = declare_dependency(
    link_with: rutils_request_lib,
    include_directories: inc,
)

meson.override_dependency('rutils-base', rutils_base_dep)
meson.override_dependency('rutils-request', rutils_request_dep)

subdir('tests')