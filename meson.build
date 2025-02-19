project(
    'qt-mpv-bg-wlr',
    'cpp',
    'c',
    version: '0.1-dev',
    license: 'MIT',
    meson_version: '>=0.60.0',
    default_options: [
        'warning_level=2',
        'default_library=static',
        'optimization=3',
        'buildtype=release',
        'debug=false', # 'cpp_std=c++23' # not yet supported by meson, as of version 0.63.0
    ],
)

# clang v14.0.6 uses C++2b instead of C++23, so we've gotta account for that
# replace the following with a project default option once meson gets support for C++23
cpp_compiler = meson.get_compiler('cpp')
if cpp_compiler.has_argument('-std=c++23')
    add_global_arguments('-std=c++23', language: 'cpp')
elif cpp_compiler.has_argument('-std=c++2b')
    add_global_arguments('-std=c++2b', language: 'cpp')
else
    error(
        'Could not configure current C++ compiler (' + cpp_compiler.get_id() + ' ' +
        cpp_compiler.version() + ') with required C++ standard (C++23)',
    )
endif
add_project_arguments(
    [
        '-Wno-unused-parameter',
        '-Wno-unused-value',
        '-Wno-missing-field-initializers',
        '-Wno-narrowing',
    ],
    language: 'cpp',
)

if get_option('use_qt_version') == 'qt5'
    qt = import('qt5')
    qt_deps = dependency(
        'qt5',
        modules: ['Widgets', 'Core'],
    )

    layer_shell_qt_dep = dependency(
        'LayerShellQt',
        modules: ['LayerShellQt::Interface'],
    )

elif get_option('use_qt_version') == 'qt6'
    qt = import('qt6')
    qt_deps = dependency(
        'qt6',
        modules: ['Widgets', 'Core', 'OpenGLWidgets'],
    )

    layer_shell_qt_dep = dependency(
        'LayerShellQt',
        modules: ['LayerShellQt::Interface'],
    )

endif

mpv_dep = dependency('mpv')

include = ['include']

# headers = [
#     'include/mpvwidget.h',
# ]
# src = [
#     'src/simple.cpp',
#     'src/mpvwidget.cpp',
# ]

headers = [
    'include/mpvwidget.h',
    'include/manager.h',
    'include/paperwidget.h',
]
src = [
    'src/main.cpp',
    'src/mpvwidget.cpp',
    'src/manager.cpp',
    'src/paperwidget.cpp',
]

moc_files = qt.compile_moc(
    headers: headers,
    extra_args: ['-DMAKES_MY_MOC_HEADER_COMPILE'],
    include_directories: include,
    dependencies: qt_deps,
)
project_args = [
    '-DPROJECT_NAME="@0@"'.format(meson.project_name()),
    '-DPROJECT_VERSION="@0@"'.format(meson.project_version()),
]

summary = [
    '',
    '------------',
    '@0@ @1@'.format(meson.project_name(), meson.project_version()),
    'Qt: @0@'.format(get_option('use_qt_version')),
    '',
    '------------',
    'MACRO args:',
    project_args,
    '------------',
    '',
]
message('\n'.join(summary))

executable(
    meson.project_name(),
    src,
    moc_files,
    cpp_args: project_args,
    include_directories: ['include'],
    dependencies: [qt_deps, mpv_dep, layer_shell_qt_dep],
    install: true,
)

