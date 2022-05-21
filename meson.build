project('pinochle', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

glibdep = dependency('glib-2.0')
exe = executable('pinochle', 'pinochle.c',
  dependencies : glibdep,
  install : true)

test('basic', exe)
