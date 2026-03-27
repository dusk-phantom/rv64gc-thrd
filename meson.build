project('rv64gc-thrd', 'c',
  version : '0.1.0',
  default_options : [
    'optimization=2',
    'debug=true',
  ],
)

inc_dir = include_directories('inc')

# fork.c 编译时禁止使用 callee-saved 寄存器 (s1-s11)
ffixed_flags = []
foreach reg : ['x9', 'x18', 'x19', 'x20', 'x21', 'x22', 'x23', 'x24', 'x25', 'x26', 'x27']
  ffixed_flags += '-ffixed-' + reg
endforeach

fork_lib = static_library('fork_objs', 'lib/fork.c',
  include_directories : inc_dir,
  c_args : ffixed_flags,
)

thrd_lib = static_library('thrd',
  'lib/son.c',
  'lib/clone.c',
  'lib/join.c',
  'lib/thrd.c',
  include_directories : inc_dir,
  objects : fork_lib.extract_all_objects(),
)

# 测试
test_names = ['test1', 'test2', 'test3', 'test4', 'test5', 'test6', 'test7', 'test8', 'b', 'd']

foreach t : test_names
  exe = executable(t, 'test' / t + '.c',
    include_directories : inc_dir,
    link_with : thrd_lib,
  )
  test(t, exe)
endforeach
