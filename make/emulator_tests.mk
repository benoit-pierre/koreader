TESTSUITES := all base front

COVERAGE_STATS = luacov.stats.out
COVERAGE_REPORT = luacov.report.out

RUNTESTS = $(INSTALL_DIR)/koreader/spec/runtests

runtests: all
	$(strip $(RUNTESTS) $(VERBOSE:%=-v)) $(TARGS)

test: test-all

$(TESTSUITES:%=test-%): all
	$(strip $(RUNTESTS) $(VERBOSE:%=-v) $(@:test-%=%)) $(TARGS)

coverage: all
	rm -f $(COVERAGE_STATS) $(COVERAGE_REPORT)
	# Run tests.
	$(strip $(RUNTESTS) $(VERBOSE:%=-v) front --coverage --tags='!nocov') $(TARGS)
	# Aggregate statistics.
	cd $(INSTALL_DIR)/koreader && test -r $(COVERAGE_STATS) || \
	    ./luajit tools/merge_luacov_stats.lua $(COVERAGE_STATS) spec/run/*/$(COVERAGE_STATS)
	# Generate report.
	cd $(INSTALL_DIR)/koreader && \
	    ./luajit -e 'r = require "luacov.runner"; r.run_report(r.configuration)' /dev/null
	test -r $(INSTALL_DIR)/koreader/$(COVERAGE_REPORT)
	# Show a summary.
	sed -n -e '/^Summary$$/{h;n;p;H;g;:_loop;p;n;b_loop}' $(INSTALL_DIR)/koreader/$(COVERAGE_REPORT)

.PHONY: coverage runtests test $(TESTSUITES:%=test-%)
