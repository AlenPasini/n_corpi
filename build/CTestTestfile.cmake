# CMake generated Testfile for 
# Source directory: /home/gregorio-montroni/n_corpi
# Build directory: /home/gregorio-montroni/n_corpi/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[n_bodies_simulation.t]=] "/home/gregorio-montroni/n_corpi/build/Debug/n_bodies_simulation.t")
  set_tests_properties([=[n_bodies_simulation.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/gregorio-montroni/n_corpi/CMakeLists.txt;55;add_test;/home/gregorio-montroni/n_corpi/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[n_bodies_simulation.t]=] "/home/gregorio-montroni/n_corpi/build/Release/n_bodies_simulation.t")
  set_tests_properties([=[n_bodies_simulation.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/gregorio-montroni/n_corpi/CMakeLists.txt;55;add_test;/home/gregorio-montroni/n_corpi/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[n_bodies_simulation.t]=] "/home/gregorio-montroni/n_corpi/build/RelWithDebInfo/n_bodies_simulation.t")
  set_tests_properties([=[n_bodies_simulation.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/gregorio-montroni/n_corpi/CMakeLists.txt;55;add_test;/home/gregorio-montroni/n_corpi/CMakeLists.txt;0;")
else()
  add_test([=[n_bodies_simulation.t]=] NOT_AVAILABLE)
endif()
