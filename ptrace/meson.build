project('ptrace', 'c', 'cpp',
    version: '0.1'
)


libs = []
deps = []
incs = []

link_args = []

executable(
    'ptrace',
    './ptrace_peekuser.c',
    include_directories: incs,
    link_args: link_args,
    link_whole: libs,
    dependencies : deps,
    install: true,
    install_dir: '/usr/bin'
)
