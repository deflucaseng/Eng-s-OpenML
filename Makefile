# Default target
.DEFAULT_GOAL := help

# Help target that shows available commands
help:
	@echo "Available commands:"
	@echo "  make representation  - Generate tree representation of directory structure"
	@echo "  make help           - Show this help message"

# Target to generate tree representation
representation:
	python3 scripts/treerep.py

# Declare the phony targets (targets that don't represent files)
.PHONY: help representation