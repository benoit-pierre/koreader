TESTSUITES := all base bench front

COVERAGE_ARGS = --test-args='--coverage --exclude-tags=nocov' --timeout-multiplier=3
COVERAGE_STATS = $(INSTALL_DIR)/koreader/luacov.stats.out
COVERAGE_REPORT = $(INSTALL_DIR)/koreader/luacov.report.out

RUNTESTS = $(INSTALL_DIR)/koreader/runtests

test: test-all

$(TESTSUITES:%=test-%): all
	$(RUNTESTS) $(@:test-%=%) $(TARGS)

testcov-front: all
	rm -f $(COVERAGE_STATS) $(COVERAGE_REPORT)
	$(RUNTESTS) front $(COVERAGE_ARGS) $(TARGS)
	test -r $(COVERAGE_STATS) || \
		find $(INSTALL_DIR)/koreader/.testrun -name $(notdir $(COVERAGE_STATS)) -print0 | \
		xargs -0 cat >$(COVERAGE_STATS)
	cd $(INSTALL_DIR)/koreader && ./luajit luacov
	test -r $(COVERAGE_REPORT)

cov: testcov-front
	sed -n -e '/^Summary$$/{h;n;p;H;g;:_loop;p;n;b_loop}' $(COVERAGE_REPORT)

cov-full: testcov-front
	cat $(COVERAGE_REPORT)

.PHONY: cov cov-full test $(TESTSUITES:%=test-%) testcov-front
