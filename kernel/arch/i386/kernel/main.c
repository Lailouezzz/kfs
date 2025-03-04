#include <multiboot.h>

void test_caca(void);

/**
 * @brief The arch main entry in the higher half
 *
 * @param mb_info multiboot2 info
 * @param magic multiboot magic
 */
void	arch_main(struct multiboot_info *mb_info, unsigned int magic) {
	if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {

	}
	test_caca();
}
