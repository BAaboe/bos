#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__) 
#error "This is not the linux kernel :)"
#endif /* ifdef MACRO */

#if !defined(__i386__)
#error "Wrong cross-compiler, needs to be ix86 compiler"
#endif

//color constants
typedef enum  {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
}vga_color;

static inline uint8_t vga_entry_color(vga_color fg, vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color){
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str){
	size_t len = 0;
	while(str[len]){
		len++;
	}
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGTH = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_init(void){
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for(size_t y = 0; y < VGA_HEIGTH; y++){
		for(size_t x = 0; x < VGA_WIDTH; x++){
			size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color){
	terminal_color = color;
}

void terminal_putcharat(char c, size_t x, size_t y){
	size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, terminal_color);
}

void terminal_putchar(char c){
	terminal_putcharat(c, terminal_column, terminal_row);
	if(++terminal_column == VGA_WIDTH){
		terminal_column = 0;
		if(++terminal_row == VGA_HEIGTH){
			terminal_row = 0;
			//TODO: add scroling window
		}
	}
}

void terminal_write(const char* str, size_t size){
	for(size_t i = 0; i < size; i++){
		terminal_putchar(str[i]);
	}
}

void terminal_print(const char* str){
	terminal_write(str, strlen(str));
}

void kernel_main(void){
	terminal_init();

	terminal_print("Hello, world :)");
}
