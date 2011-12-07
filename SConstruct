import os

include_dir = Dir('#/src')
#env = Environment()
#env.Append(CPPPATH = include_dir)

#print("include path is")
#print(env['CPPPATH'])

phase_release = "Release"
phase_debug = "Debug"
phase_profile = "Profiling"

cflags_release = '-march=native -mtune=native -O2 -std=gnu++0x' 
cflags_debug = '-ggdb -Wall -std=gnu++0x'
cflags_profile = '-ggdb -pg -Wall -std=gnu++0x'

ldflags_release = '-s'
ldflags_debug = ''
ldflags_profile = '-pg'

bin_dir = Dir('#bin/release')
lib_dir = Dir('#build/release/libs')

phase = phase_release
cflags = cflags_release
ldflags = ldflags_release
Export('include_dir lib_dir bin_dir cflags phase ldflags')
SConscript('src/SConscript', variant_dir='build/release')

bin_dir = Dir('#bin/debug')
lib_dir = Dir('#build/debug/libs')

cflags = cflags_debug
phase = phase_debug
ldflags = ldflags_debug
Export('include_dir lib_dir bin_dir cflags phase ldflags')
SConscript('src/SConscript', variant_dir='build/debug')

bin_dir = Dir('#bin/profile')
lib_dir = Dir('#build/profile/libs')

cflags = cflags_profile
phase = phase_profile
ldflags = ldflags_profile
Export('include_dir lib_dir bin_dir cflags phase ldflags')
SConscript('src/SConscript', variant_dir='build/profile')

