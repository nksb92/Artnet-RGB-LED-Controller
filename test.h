enum {
	main_menu,
	selection_frame
};

#define BITMAP_MAIN_MENU_LEN 2

int W_H_MATRIX_MAIN_MENU[BITMAP_MAIN_MENU_LEN][2] = {
	{128, 115},
	{24, 24}
};

int X_Y_MATRIX_MAIN_MENU[BITMAP_MAIN_MENU_LEN][2] = {
	{0, 0},
	{0, 0}
};

const unsigned char* BITMAP_MAIN_MENU_ARRAY[BITMAP_MAIN_MENU_LEN] = {
	BITMAP_main_menu,
	BITMAP_selection_frame
};