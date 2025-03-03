#!/usr/bin/make -f
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
# Author : Alan "ale-boud" Le Bouder <ale-boud@student.42lehavre.fr>
#
# Description of source tree :
# └── flemme
#
# After compilation : TODO
# - $(OBJDIR) contain all object files and gcc generated dependencies
# - $(OUTDIR) contain the executable
#
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

# Avoid exported variable
PHONY :=

.EXPORT_ALL_VARIABLES:

PHONY += all
all: subdirs
	$(MAKE) kfs

# Exported global variables
include Makefile.cfg
Makefile.cfg:
	$(call emsg,Makefile.cfg missing did you "./configure")
	@exit 1
include Makefile.vars

# K_LDSCRIPT is defined in the architecture directory
PHONY += kfs
kfs: $(K_OBJS) $(K_LDSCRIPT)
	$(LD) $(KLDFLAGS) -T $(K_LDSCRIPT) $(K_OBJS) -nostdlib -o kfs

include Makefile.rules

.PHONY: $(PHONY)
