
#include <util/fs_util.h>
#include <testing/testing.h>

using namespace util::fs;

// --------------------------------------------------------------------------
void test_command_ls () {
  auto result = command("ls Make*");

  EXPECT_EQUAL(result.exit_code, 0);
  EXPECT_EQUAL(result.output, std::string("Makefile\n"));
}

// --------------------------------------------------------------------------
void test_command_pwd () {
  auto pwd = sys_fs::current_path();
  std::string expected = pwd.string() + "\n";
  auto result = command("pwd");

  EXPECT_EQUAL(result.exit_code, 0);
  EXPECT_EQUAL(result.output, expected);
}

// --------------------------------------------------------------------------
void test_main (const testing::start_params&) {
  testing::log_info("Running " __FILE__);
  run_test(test_command_ls);
  run_test(test_command_pwd);
}

// --------------------------------------------------------------------------

