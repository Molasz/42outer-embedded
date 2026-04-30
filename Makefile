.PHONY: hex

HEX_DIRS := $(shell find . -mindepth 2 -maxdepth 3 -name Makefile -not -path './Makefile' -print0 | xargs -0 grep -l '^hex:' 2>/dev/null | xargs -n1 dirname | sort -u)

hex:
	@echo "Running make hex in all subdirectories..."
	@for dir in $(HEX_DIRS); do \
		printf "=> %s\n" "$${dir}"; \
		$(MAKE) -C "$${dir}" hex || exit $$?; \
	done
	@echo "All hex targets completed."
