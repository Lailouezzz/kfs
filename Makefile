#!/usr/bin/make -f
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
#
# Author : Alan "ale-boud" Le Bouder <ale-boud@student.42lehavre.fr>
#
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# Default target
all: all-minishell
bonus: all

# Include vars and msg module
include Makefile.vars Makefile.msg

# ---
# General targets
# ---

.PHONY: all bonus

# ---
# Check configuration
# ---

Makefile.cfg:
	$(call emsg,Makefile.cfg missing did you "./configure")
	@exit 1
