# CMake generated Testfile for 
# Source directory: /Users/ben/work/games/Listener
# Build directory: /Users/ben/work/games/Listener
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(ExternalLibAS "/Users/ben/work/games/Listener/bin/Main" "Scripts/12_PhysicsStressTest.as" "-w" "-timeout" "5")
add_test(ExternalLibLua "/Users/ben/work/games/Listener/bin/Main" "LuaScripts/12_PhysicsStressTest.lua" "-w" "-timeout" "5")
