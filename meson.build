project('TemplateVM', 'cpp', default_options : ['cpp_std=c++17', 'warning_level=3'])

if meson.get_compiler('cpp').get_id() == 'gcc' or meson.get_compiler('cpp').get_id() == 'clang'
  add_project_arguments('-Wno-unused-parameter', '-pedantic', language : 'cpp')
endif

executable('templatevm', 'source/main.cpp',
  include_directories : include_directories('source'))
