#define SCREEN_ADDRESS ((unsigned short*)0xB8000)
#define ADDRESS_REGISTER ((unsigned char *)0x3CE)
#define GRAPHICS_MODE_REGISTER (ADDRESS_REGISTER + 5)
#define MISCELLANEOUS_GRAPHICS_REGISTER (ADDRESS_REGISTER + 6)

#define GRAPHICS_MODE_REGISTER__MASK__WRITE_MODE (0b00000011)
#define GRAPHICS_MODE_REGISTER__MASK__MEMORY_MAP_SELECT (0b00001100)

#define RANGE_00 "A0000h-BFFFFh"
#define RANGE_01 "A0000h-AFFFFh"
#define RANGE_10 "B0000h-B7FFFh"
#define RANGE_11 "B8000h-BFFFFh"

static unsigned char	cx = 0;
static unsigned char	cy = 0;
static unsigned short	*video;


void test_caca(void)
{
	const char memory_map_select = *MISCELLANEOUS_GRAPHICS_REGISTER & GRAPHICS_MODE_REGISTER__MASK__MEMORY_MAP_SELECT;
	const char *str;

	switch (memory_map_select >> 2)
	{
		case 0b10011:
			str = RANGE_00;
			video = (unsigned short *)0xA0000;
			break;
		case 0b01:
			str = RANGE_01;
			video = (unsigned short *)0xA0000;
			break;
		case 0b10:
			str = RANGE_10;
			video = (unsigned short *)0xB0000;
			break;
		case 0b11:
			str = RANGE_11;
			video = (unsigned short *)0xB8000;
			break;
		default:
			str = "RIP";
			video = (unsigned short *)0xB8000;
			break;
	}

	while (*str)
		*video++ = 0x0b00 | *str++;
}
