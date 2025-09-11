#!/usr/bin/env python
import os
import platform

Import('env')

# Propagate the 'TERM' environment variable from the OS, so tools can decide if they should colorize output
env['ENV']['TERM'] = os.getenv('TERM', 'unknown')

# Google test runner uses pthread on linux
if 'linux' in platform.system().lower():
    env.Append(LIBS=['pthread'])

# I did not find a way yet to make SCons' AddOption / GetOption work with PlatformIO.
# However, PlatformIO allows passing custom targets.
# Therefore I use this little hack to add the ability to run valgrind optionally.
def _build_nothing(target, source, env):
    pass
dummy = env.Command('foo', 'lib/readme.txt', _build_nothing)
Alias('memcheck', dummy)

prefix = ''
suffix = ''

if 'memcheck' in COMMAND_LINE_TARGETS:
    prefix += 'valgrind --leak-check=yes '

# Build separate program for every test case.
# PIOBUILDFILES contains the SCons node for every built object file as determined by platformio.ini
test_results = []
for node in env['PIOBUILDFILES']:
    test_name = os.path.basename(str(node[0])).rsplit('.')[0]
    test_path = os.path.join(env['BUILD_DIR'], test_name)
    test_program = env.Program(test_path, node)

    test_result = env.Command(test_path + 'Result.xml', test_program, f"{prefix}{os.path.join('.', '$SOURCE')} --gtest_output=xml:$TARGET{suffix}")
    env.AlwaysBuild(test_result)
    Alias(test_name, test_result)
    test_results += test_result

# Make sure that when only 'memcheck' is specified, all tests are still run
if 'memcheck' in COMMAND_LINE_TARGETS and (len(COMMAND_LINE_TARGETS) == 1):
    Alias('memcheck', test_results)

Alias('all', test_results)
Default('all')
