TEST_EXTENSIONS = .t
T_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
	$(top_srcdir)/config/tap-driver.sh

TEST_ENVIRONMENT = \
	export PATH=/home/dahn/workspace/imagedb/t/../imagelogue/cli:$(PATH)

TESTS = \
    t0000-sharness.t \
    t0001-server.t

check_SCRIPTS = $(TESTS)
