#include <consoleapp.hxx>
#include <iostream>

#include <tut/tut.hpp>
#include <tut/tut_reporter.hpp>

tut::test_runner_singleton runner;

int ntl::consoleapp::main()
{
  tut::reporter writer;
  runner.get().set_callback(&writer);
  //runner.get().run_tests("std::exception_ptr");
  runner.get().run_tests();

  bool ok = writer.all_ok();

  std::cout << (ok ? "well done" : "something wrong :-(") << '.' << std::endl;
  std::cout << "press enter to exit.";
  std::cin.get();

  return !ok;
}