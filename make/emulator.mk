# Testing & coverage. {{{

PHONY += coverage test test%

test%: all test-data
	$(RUNTESTS) $(INSTALL_DIR)/koreader $* $T

test: testall

COVERAGE_STATS = luacov.stats.out
COVERAGE_REPORT = luacov.report.out

$(INSTALL_DIR)/koreader/.luacov:
	$(SYMLINK) .luacov $@

coverage: all test-data $(INSTALL_DIR)/koreader/.luacov
	rm -f $(addprefix $(INSTALL_DIR)/koreader/,$(COVERAGE_STATS) $(COVERAGE_REPORT))
	# Run tests.
	$(RUNTESTS) $(INSTALL_DIR)/koreader front --coverage --exclude-tags=nocov $T
	# Aggregate statistics.
	cd $(INSTALL_DIR)/koreader && \
	    eval "$$($(LUAROCKS_BINARY) path)" && \
	    test -r $(COVERAGE_STATS) || \
	    ./luajit tools/merge_luacov_stats.lua $(COVERAGE_STATS) spec/.run/*/$(COVERAGE_STATS)
	# Generate report.
	cd $(INSTALL_DIR)/koreader && \
	    eval "$$($(LUAROCKS_BINARY) path)" && \
	    ./luajit -e 'r = require "luacov.runner"; r.run_report(r.configuration)' /dev/null
	# Show a summary.
	sed -n -e '/^Summary$$/{h;n;p;H;g;:_loop;p;n;b_loop}' $(INSTALL_DIR)/koreader/$(COVERAGE_REPORT)

# }}}

# vim: foldmethod=marker foldlevel=0
