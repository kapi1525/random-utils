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


meson.override_dependency('rutils-base', rutils_base_dep)

if not meson.is_subproject()
    subdir('tests')
endif