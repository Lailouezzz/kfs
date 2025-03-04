#!/usr/bin/make -f
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
# Author : Alan "ale-boud" Le Bouder <ale-boud@student.42lehavre.fr>
#
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

include Makefile.msg
PHONY :=

# Default target
PHONY += all
all: all-kfsos
PHONY += bonus
bonus: all


PHONY += clean
clean:
	$(call rmsg,Cleaning kernel dir and removing iso root directory)
	$(call qcmd,$(MAKE) -C $(K_DIR) fclean) || true
	$(call qcmd,$(RM) -r $(ISO_ROOT_DIR))

PHONY += fclean
fclean: clean
	$(call rmsg,Removing $(ISO))
	$(call qcmd,$(RM) $(ISO))

PHONY += re
re: fclean all

run-qemu: $(ISO)
	qemu-system-i386 -cdrom kfsos.iso -boot d

run-qemu-debug: $(ISO)
	qemu-system-i386 -cdrom kfsos.iso -boot d -S -s

# Include vars and msg module
include Makefile.vars Makefile.msg

# ---
# General targets
# ---

PHONY += all-kfsos
all-kfsos: $(ISO)

PHONY += $(K_BIN)
$(K_BIN): $(K_DIR)/Makefile.cfg
	$(call bcmd,make,-C $(K_DIR),$(MAKE) -C $(K_DIR))

$(K_DIR)/Makefile.cfg:
	$(call qcmd,sh $(K_DIR)/configure.sh)

$(ISO): $(K_BIN) $(GRUB_CFG)
	$(call qcmd,$(MKDIR) -pv $(ISO_ROOT_DIR)/boot/grub)
	$(call qcmd,cp $(K_BIN) $(ISO_ROOT_DIR)/boot)
	$(call qcmd,cp $(GRUB_CFG) $(ISO_ROOT_DIR)/boot/grub)
	grub-file --is-x86-multiboot2 $(ISO_ROOT_DIR)/boot/vmkfs
	grub-mkrescue -o $(ISO) $(ISO_ROOT_DIR)

# ---
# Check configuration
# ---

Makefile.cfg:
	$(call emsg,Makefile.cfg missing did you "./configure")
	@exit 1

.PHONY: $(PHONY)
