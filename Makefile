.PHONY: hex clean

DIRS := $(shell find . -mindepth 2 -maxdepth 3 -name Makefile -not -path './Makefile' -print0 | xargs -0 dirname | sort -u)

hex:
	@echo "Running make hex in all subdirectories..."
	@for dir in $(DIRS); do \
		printf "=> %s\n" "$$dir"; \
		$(MAKE) -C "$$dir" hex || exit $$?; \
	done
	@echo "All hex targets completed."

clean:
	@echo "Running make clean in all subdirectories..."
	@for dir in $(DIRS); do \
		printf "=> %s\n" "$$dir"; \
		$(MAKE) -C "$$dir" clean || exit $$?; \
	done
	@echo "All clean targets completed."
