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

PHONY += clean
clean:
	CLEAN_OBJS=1 $(MAKE) subdirs

PHONY += fclean
fclean: clean
	$(RM) kfs

PHONY += mrproper
mrproper: clean
	$(RM) Makefile.cfg

PHONY += re
re: fclean all

# Exported global variables
include Makefile.cfg
Makefile.cfg:
	$(call emsg,Makefile.cfg missing did you "./configure")
	@exit 1
include Makefile.vars

# K_LDSCRIPT is defined in the architecture directory
kfs: $(K_OBJS) $(K_LDSCRIPT)
	$(LD) $(KLDFLAGS) -T $(K_LDSCRIPT) -nostdlib -o $@ $(K_OBJS)

include Makefile.rules

.PHONY: $(PHONY)
