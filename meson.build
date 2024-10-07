project(
  'test-project',
  'c',
  version: '0.1',
  default_options: ['warning_level=3', 'c_std=c17'],
)
unity_subproject = subproject('unity')
unity_dependency = unity_subproject.get_variable('unity_dep')
unity_gen_runner = unity_subproject.get_variable('gen_test_runner')

src = files('test' / 'emulator_test.c', 'src' / 'emulator.c')
inc = include_directories('src', 'include')
test = executable(
  'app_test',
  sources: [src, unity_gen_runner.process('test' / 'emulator_test.c')],
  include_directories: inc,
  dependencies: unity_dependency,
)
test('app_test', test)
