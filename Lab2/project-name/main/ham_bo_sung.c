#include "ham_bo_sung.h"
#include "font8x8_basic.h"
#include "driver/i2c.h"
static const char *TAG = "0.96 inch oled";

void ssd1306_init() {
	esp_err_t espRc;

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();

	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
	i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);

	i2c_master_write_byte(cmd, OLED_CMD_SET_CHARGE_PUMP, true);
	i2c_master_write_byte(cmd, 0x14, true);

	i2c_master_write_byte(cmd, OLED_CMD_SET_SEGMENT_REMAP, true); // reverse left-right mapping
	i2c_master_write_byte(cmd, OLED_CMD_SET_COM_SCAN_MODE, true); // reverse up-bottom mapping

	i2c_master_write_byte(cmd, OLED_CMD_DISPLAY_NORMAL, true);
    i2c_master_write_byte(cmd, OLED_CMD_DISPLAY_OFF, true);
	i2c_master_write_byte(cmd, OLED_CMD_DISPLAY_ON, true);
	i2c_master_stop(cmd);

	espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
	if (espRc == ESP_OK) {
		ESP_LOGI(TAG, "OLED configured successfully");
	} else {
		ESP_LOGE(TAG, "OLED configuration failed. code: 0x%.2X", espRc);
	}
	i2c_cmd_link_delete(cmd);
}

void task_ssd1306_display_text(const void *arg_text) {
	char *text = (char*)arg_text;
	uint8_t text_len = strlen(text);

	i2c_cmd_handle_t cmd;

	uint8_t cur_page = 1;

	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

	i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
	i2c_master_write_byte(cmd, 0x00, true); // reset column - choose column --> 0
	i2c_master_write_byte(cmd, 0x10, true); // reset line - choose line --> 0
	i2c_master_write_byte(cmd, 0xB0 | cur_page, true); // reset page

	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);

	for (uint8_t i = 0; i < text_len; i++) {
		if (text[i] == '\n') {
			cmd = i2c_cmd_link_create();
			i2c_master_start(cmd);
			i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

			i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
			i2c_master_write_byte(cmd, 0x00, true); // reset column
			i2c_master_write_byte(cmd, 0x10, true);
			i2c_master_write_byte(cmd, 0xB0 | ++cur_page, true); // increment page

			i2c_master_stop(cmd);
			i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
			i2c_cmd_link_delete(cmd);
		} else {
			cmd = i2c_cmd_link_create();
			i2c_master_start(cmd);
			i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

			i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_DATA_STREAM, true);
			i2c_master_write(cmd, font8x8_basic_tr[(uint8_t)text[i]], 8, true);

			i2c_master_stop(cmd);
			i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
			i2c_cmd_link_delete(cmd);
		}
	}

	vTaskDelete(NULL);
}

// 'unnamed (1)', 128x64px
const unsigned char myUITlogo [1024] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 

	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xf8, 0x0f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xfe, 0x00, 0x30, 0x03, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x07, 0xff, 0xff, 0x80, 0xcf, 0xf8, 0x04, 0x7f, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xff, 0x9f, 0xff, 0xff, 

	0xff, 0xff, 0xff, 0xff, 0xff, 0xc1, 0xff, 0xff, 0x07, 0xff, 0xfd, 0xff, 0xff, 0xef, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0x7f, 0xff, 0xf7, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x3f, 0xff, 0x83, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xf7, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xf3, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe1, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff, 0xf7, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xf7, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x3f, 0xcf, 0xf8, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0x03, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xef, 0xff, 0xff, 

	0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0xfe, 0x1e, 0x3c, 0x1f, 0xff, 0xfb, 0xff, 0xdf, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xfe, 0x0f, 0xfc, 0x78, 0x07, 0x0f, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xf8, 0xe0, 0x03, 0x8f, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe0, 0x7f, 0xff, 0x80, 0x00, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xc0, 0xf8, 0x37, 0xfc, 0x1f, 0xff, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x03, 0xf8, 0x3e, 0x0f, 0xf8, 0x1b, 0x07, 0xf7, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfe, 0x07, 0xf8, 0x3c, 0x0f, 0xf8, 0x0e, 0x0f, 0xef, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff,

	0xff, 0xff, 0xff, 0xf8, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf0, 0x1f, 0xff, 0xe0, 0x3f, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xe0, 0x3f, 0x81, 0xfc, 0x1f, 0xfc, 0x1f, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xe0, 0x3f, 0xe0, 0x3f, 0x07, 0xf8, 0x7e, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xc0, 0x3f, 0xf8, 0x07, 0xe3, 0xe3, 0xf8, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0x00, 0x78, 0xcf, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xe0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 

	0xff, 0xff, 0xff, 0xf0, 0x01, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xf8, 0x00, 0x3f, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 

	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1c, 0x63, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1c, 0x63, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1c, 0x63, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x18, 0xe3, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 

	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

uint8_t data[1024];
void ConvertBitmap()
{
    int i = 0;
    for (int i = 0; i < 128; i += 8)
    {
        data[i + 0] =   ((myUITlogo[ 0  + i / 8] & 0x80) >> 7)|
                        ((myUITlogo[ 16 + i / 8] & 0x80) >> 6)|
                        ((myUITlogo[ 32 + i / 8] & 0x80) >> 5)| 
                        ((myUITlogo[ 48 + i / 8] & 0x80) >> 4)|
                        ((myUITlogo[ 64 + i / 8] & 0x80) >> 3)|
                        ((myUITlogo[ 80 + i / 8] & 0x80) >> 2)|
                        ((myUITlogo[ 96 + i / 8] & 0x80) >> 1)|
                        ((myUITlogo[112 + i / 8] & 0x80) >> 0);
		data[i + 1] =   ((myUITlogo[ 0  + i / 8] & 0x40) >> 6)|
                        ((myUITlogo[ 16 + i / 8] & 0x40) >> 5)|
                        ((myUITlogo[ 32 + i / 8] & 0x40) >> 4)| 
                        ((myUITlogo[ 48 + i / 8] & 0x40) >> 3)|
                        ((myUITlogo[ 64 + i / 8] & 0x40) >> 2)|
                        ((myUITlogo[ 80 + i / 8] & 0x40) >> 1)|
                        ((myUITlogo[ 96 + i / 8] & 0x40) >> 0)|
                        ((myUITlogo[112 + i / 8] & 0x40) << 1);
		data[i + 2] =   ((myUITlogo[ 0  + i / 8] & 0x20) >> 5)|
                        ((myUITlogo[ 16 + i / 8] & 0x20) >> 4)|
                        ((myUITlogo[ 32 + i / 8] & 0x20) >> 3)| 
                        ((myUITlogo[ 48 + i / 8] & 0x20) >> 2)|
                        ((myUITlogo[ 64 + i / 8] & 0x20) >> 1)|
                        ((myUITlogo[ 80 + i / 8] & 0x20) >> 0)|
                        ((myUITlogo[ 96 + i / 8] & 0x20) << 1)|
                        ((myUITlogo[112 + i / 8] & 0x20) << 2);
		data[i + 3] =   ((myUITlogo[ 0  + i / 8] & 0x10) >> 4)|
                        ((myUITlogo[ 16 + i / 8] & 0x10) >> 3)|
                        ((myUITlogo[ 32 + i / 8] & 0x10) >> 2)| 
                        ((myUITlogo[ 48 + i / 8] & 0x10) >> 1)|
                        ((myUITlogo[ 64 + i / 8] & 0x10) >> 0)|
                        ((myUITlogo[ 80 + i / 8] & 0x10) << 1)|
                        ((myUITlogo[ 96 + i / 8] & 0x10) << 2)|
                        ((myUITlogo[112 + i / 8] & 0x10) << 3);
		data[i + 4] =   ((myUITlogo[ 0  + i / 8] & 0x08) >> 3)|
                        ((myUITlogo[ 16 + i / 8] & 0x08) >> 2)|
                        ((myUITlogo[ 32 + i / 8] & 0x08) >> 1)| 
                        ((myUITlogo[ 48 + i / 8] & 0x08) >> 0)|
                        ((myUITlogo[ 64 + i / 8] & 0x08) << 1)|
                        ((myUITlogo[ 80 + i / 8] & 0x08) << 2)|
                        ((myUITlogo[ 96 + i / 8] & 0x08) << 3)|
                        ((myUITlogo[112 + i / 8] & 0x08) << 4);
		data[i + 5] =   ((myUITlogo[ 0  + i / 8] & 0x04) >> 2)|
                        ((myUITlogo[ 16 + i / 8] & 0x04) >> 1)|
                        ((myUITlogo[ 32 + i / 8] & 0x04) >> 0)| 
                        ((myUITlogo[ 48 + i / 8] & 0x04) << 1)|
                        ((myUITlogo[ 64 + i / 8] & 0x04) << 2)|
                        ((myUITlogo[ 80 + i / 8] & 0x04) << 3)|
                        ((myUITlogo[ 96 + i / 8] & 0x04) << 4)|
                        ((myUITlogo[112 + i / 8] & 0x04) << 5);
		data[i + 6] =   ((myUITlogo[ 0  + i / 8] & 0x02) >> 1)|
                        ((myUITlogo[ 16 + i / 8] & 0x02) >> 0)|
                        ((myUITlogo[ 32 + i / 8] & 0x02) << 1)| 
                        ((myUITlogo[ 48 + i / 8] & 0x02) << 2)|
                        ((myUITlogo[ 64 + i / 8] & 0x02) << 3)|
                        ((myUITlogo[ 80 + i / 8] & 0x02) << 4)|
                        ((myUITlogo[ 96 + i / 8] & 0x02) << 5)|
                        ((myUITlogo[112 + i / 8] & 0x02) << 6);
		data[i + 7] =   ((myUITlogo[ 0  + i / 8] & 0x01) >> 0)|
                        ((myUITlogo[ 16 + i / 8] & 0x01) << 1)|
                        ((myUITlogo[ 32 + i / 8] & 0x01) << 2)| 
                        ((myUITlogo[ 48 + i / 8] & 0x01) << 3)|
                        ((myUITlogo[ 64 + i / 8] & 0x01) << 4)|
                        ((myUITlogo[ 80 + i / 8] & 0x01) << 5)|
                        ((myUITlogo[ 96 + i / 8] & 0x01) << 6)|
                        ((myUITlogo[112 + i / 8] & 0x01) << 7);
    }
    
    for (int i = 128, j = 0; i < 256; i += 8, j += 8)
    {
        data[i + 0] =   ((myUITlogo[ 8 * 16 + j / 8] & 0x80) >> 7)|
                        ((myUITlogo[ 8 * 18 + j / 8] & 0x80) >> 6)|
                        ((myUITlogo[ 8 * 20 + j / 8] & 0x80) >> 5)| 
                        ((myUITlogo[ 8 * 22 + j / 8] & 0x80) >> 4)|
                        ((myUITlogo[ 8 * 24 + j / 8] & 0x80) >> 3)|
                        ((myUITlogo[ 8 * 26 + j / 8] & 0x80) >> 2)|
                        ((myUITlogo[ 8 * 28 + j / 8] & 0x80) >> 1)|
                        ((myUITlogo[ 8 * 30 + j / 8] & 0x80) >> 0);
		data[i + 1] =   ((myUITlogo[ 8 * 16 + j / 8] & 0x40) >> 6)|
                        ((myUITlogo[ 8 * 18 + j / 8] & 0x40) >> 5)|
                        ((myUITlogo[ 8 * 20 + j / 8] & 0x40) >> 4)| 
                        ((myUITlogo[ 8 * 22 + j / 8] & 0x40) >> 3)|
                        ((myUITlogo[ 8 * 24 + j / 8] & 0x40) >> 2)|
                        ((myUITlogo[ 8 * 26 + j / 8] & 0x40) >> 1)|
                        ((myUITlogo[ 8 * 28 + j / 8] & 0x40) >> 0)|
                        ((myUITlogo[ 8 * 30 + j / 8] & 0x40) << 1);
		data[i + 2] =   ((myUITlogo[ 8  * 16 + j / 8] & 0x20) >> 5)|
                        ((myUITlogo[ 8 * 18 + j / 8] & 0x20) >> 4)|
                        ((myUITlogo[ 8 * 20 + j / 8] & 0x20) >> 3)| 
                        ((myUITlogo[ 8 * 22 + j / 8] & 0x20) >> 2)|
                        ((myUITlogo[ 8 * 24 + j / 8] & 0x20) >> 1)|
                        ((myUITlogo[ 8 * 26 + j / 8] & 0x20) >> 0)|
                        ((myUITlogo[ 8 * 28 + j / 8] & 0x20) << 1)|
                        ((myUITlogo[ 8 * 30 + j / 8] & 0x20) << 2);
		data[i + 3] =   ((myUITlogo[ 8  * 16 + j / 8] & 0x10) >> 4)|
                        ((myUITlogo[ 8 * 18 + j / 8] & 0x10) >> 3)|
                        ((myUITlogo[ 8 * 20 + j / 8] & 0x10) >> 2)| 
                        ((myUITlogo[ 8 * 22 + j / 8] & 0x10) >> 1)|
                        ((myUITlogo[ 8 * 24 + j / 8] & 0x10) >> 0)|
                        ((myUITlogo[ 8 * 26 + j / 8] & 0x10) << 1)|
                        ((myUITlogo[ 8 * 28 + j / 8] & 0x10) << 2)|
                        ((myUITlogo[ 8 * 30 + j / 8] & 0x10) << 3);
		data[i + 4] =   ((myUITlogo[ 8  * 16 + j / 8] & 0x08) >> 3)|
                        ((myUITlogo[ 8 * 18 + j / 8] & 0x08) >> 2)|
                        ((myUITlogo[ 8 * 20 + j / 8] & 0x08) >> 1)| 
                        ((myUITlogo[ 8 * 22 + j / 8] & 0x08) >> 0)|
                        ((myUITlogo[ 8 * 24 + j / 8] & 0x08) << 1)|
                        ((myUITlogo[ 8 * 26 + j / 8] & 0x08) << 2)|
                        ((myUITlogo[ 8 * 28 + j / 8] & 0x08) << 3)|
                        ((myUITlogo[ 8 * 30 + j / 8] & 0x08) << 4);
		data[i + 5] =   ((myUITlogo[ 8  * 16 + j / 8] & 0x04) >> 2)|
                        ((myUITlogo[ 8 * 18 + j / 8] & 0x04) >> 1)|
                        ((myUITlogo[ 8 * 20 + j / 8] & 0x04) >> 0)| 
                        ((myUITlogo[ 8 * 22 + j / 8] & 0x04) << 1)|
                        ((myUITlogo[ 8 * 24 + j / 8] & 0x04) << 2)|
                        ((myUITlogo[ 8 * 26 + j / 8] & 0x04) << 3)|
                        ((myUITlogo[ 8 * 28 + j / 8] & 0x04) << 4)|
                        ((myUITlogo[ 8 * 30 + j / 8] & 0x04) << 5);
		data[i + 6] =   ((myUITlogo[ 8  * 16 + j / 8] & 0x02) >> 1)|
                        ((myUITlogo[ 8 * 18 + j / 8] & 0x02) >> 0)|
                        ((myUITlogo[ 8 * 20 + j / 8] & 0x02) << 1)| 
                        ((myUITlogo[ 8 * 22 + j / 8] & 0x02) << 2)|
                        ((myUITlogo[ 8 * 24 + j / 8] & 0x02) << 3)|
                        ((myUITlogo[ 8 * 26 + j / 8] & 0x02) << 4)|
                        ((myUITlogo[ 8 * 28 + j / 8] & 0x02) << 5)|
                        ((myUITlogo[ 8 * 30 + j / 8] & 0x02) << 6);
		data[i + 7] =   ((myUITlogo[ 8 * 16 + j / 8] & 0x01) >> 0)|
                        ((myUITlogo[ 8 * 18 + j / 8] & 0x01) << 1)|
                        ((myUITlogo[ 8 * 20 + j / 8] & 0x01) << 2)| 
                        ((myUITlogo[ 8 * 22 + j / 8] & 0x01) << 3)|
                        ((myUITlogo[ 8 * 24 + j / 8] & 0x01) << 4)|
                        ((myUITlogo[ 8 * 26 + j / 8] & 0x01) << 5)|
                        ((myUITlogo[ 8 * 28 + j / 8] & 0x01) << 6)|
                        ((myUITlogo[ 8 * 30 + j / 8] & 0x01) << 7);
    }
    
    for (int i = 256, j = 0; i < 384; i += 8, j += 8)
    {
        data[i + 0] =   ((myUITlogo[ 8 * 32  + j / 8] & 0x80) >> 7)|
                        ((myUITlogo[ 8 * 34 + j / 8] & 0x80) >> 6)|
                        ((myUITlogo[ 8 * 36 + j / 8] & 0x80) >> 5)| 
                        ((myUITlogo[ 8 * 38 + j / 8] & 0x80) >> 4)|
                        ((myUITlogo[ 8 * 40 + j / 8] & 0x80) >> 3)|
                        ((myUITlogo[ 8 * 42 + j / 8] & 0x80) >> 2)|
                        ((myUITlogo[ 8 * 44 + j / 8] & 0x80) >> 1)|
                        ((myUITlogo[ 8 * 46 + j / 8] & 0x80) >> 0);
		data[i + 1] =   ((myUITlogo[ 8  * 32 + j / 8] & 0x40) >> 6)|
                        ((myUITlogo[ 8 * 34 + j / 8] & 0x40) >> 5)|
                        ((myUITlogo[ 8 * 36 + j / 8] & 0x40) >> 4)| 
                        ((myUITlogo[ 8 * 38 + j / 8] & 0x40) >> 3)|
                        ((myUITlogo[ 8 * 40 + j / 8] & 0x40) >> 2)|
                        ((myUITlogo[ 8 * 42 + j / 8] & 0x40) >> 1)|
                        ((myUITlogo[ 8 * 44 + j / 8] & 0x40) >> 0)|
                        ((myUITlogo[ 8 * 46 + j / 8] & 0x40) << 1);
		data[i + 2] =   ((myUITlogo[ 8  * 32 + j / 8] & 0x20) >> 5)|
                        ((myUITlogo[ 8 * 34 + j / 8] & 0x20) >> 4)|
                        ((myUITlogo[ 8 * 36 + j / 8] & 0x20) >> 3)| 
                        ((myUITlogo[ 8 * 38 + j / 8] & 0x20) >> 2)|
                        ((myUITlogo[ 8 * 40 + j / 8] & 0x20) >> 1)|
                        ((myUITlogo[ 8 * 42 + j / 8] & 0x20) >> 0)|
                        ((myUITlogo[ 8 * 44 + j / 8] & 0x20) << 1)|
                        ((myUITlogo[ 8 * 46 + j / 8] & 0x20) << 2);
		data[i + 3] =   ((myUITlogo[ 8  * 32 + j / 8] & 0x10) >> 4)|
                        ((myUITlogo[ 8 * 34 + j / 8] & 0x10) >> 3)|
                        ((myUITlogo[ 8 * 36 + j / 8] & 0x10) >> 2)| 
                        ((myUITlogo[ 8 * 38 + j / 8] & 0x10) >> 1)|
                        ((myUITlogo[ 8 * 40 + j / 8] & 0x10) >> 0)|
                        ((myUITlogo[ 8 * 42 + j / 8] & 0x10) << 1)|
                        ((myUITlogo[ 8 * 44 + j / 8] & 0x10) << 2)|
                        ((myUITlogo[ 8 * 46 + j / 8] & 0x10) << 3);
		data[i + 4] =   ((myUITlogo[ 8  * 32 + j / 8] & 0x08) >> 3)|
                        ((myUITlogo[ 8 * 34 + j / 8] & 0x08) >> 2)|
                        ((myUITlogo[ 8 * 36 + j / 8] & 0x08) >> 1)| 
                        ((myUITlogo[ 8 * 28 + j / 8] & 0x08) >> 0)|
                        ((myUITlogo[ 8 * 40 + j / 8] & 0x08) << 1)|
                        ((myUITlogo[ 8 * 42 + j / 8] & 0x08) << 2)|
                        ((myUITlogo[ 8 * 44 + j / 8] & 0x08) << 3)|
                        ((myUITlogo[ 8 * 46 + j / 8] & 0x08) << 4);
		data[i + 5] =   ((myUITlogo[ 8  * 32 + j / 8] & 0x04) >> 2)|
                        ((myUITlogo[ 8 * 34 + j / 8] & 0x04) >> 1)|
                        ((myUITlogo[ 8 * 36 + j / 8] & 0x04) >> 0)| 
                        ((myUITlogo[ 8 * 38 + j / 8] & 0x04) << 1)|
                        ((myUITlogo[ 8 * 40 + j / 8] & 0x04) << 2)|
                        ((myUITlogo[ 8 * 42 + j / 8] & 0x04) << 3)|
                        ((myUITlogo[ 8 * 44 + j / 8] & 0x04) << 4)|
                        ((myUITlogo[ 8 * 46 + j / 8] & 0x04) << 5);
		data[i + 6] =   ((myUITlogo[ 8  * 32 + j / 8] & 0x02) >> 1)|
                        ((myUITlogo[ 8 * 34 + j / 8] & 0x02) >> 0)|
                        ((myUITlogo[ 8 * 36 + j / 8] & 0x02) << 1)| 
                        ((myUITlogo[ 8 * 38 + j / 8] & 0x02) << 2)|
                        ((myUITlogo[ 8 * 40 + j / 8] & 0x02) << 3)|
                        ((myUITlogo[ 8 * 42 + j / 8] & 0x02) << 4)|
                        ((myUITlogo[ 8 * 44 + j / 8] & 0x02) << 5)|
                        ((myUITlogo[ 8 * 46 + j / 8] & 0x02) << 6);
		data[i + 7] =   ((myUITlogo[ 8 * 32  + j / 8] & 0x01) >> 0)|
                        ((myUITlogo[ 8 * 34 + j / 8] & 0x01) << 1)|
                        ((myUITlogo[ 8 * 36 + j / 8] & 0x01) << 2)| 
                        ((myUITlogo[ 8 * 38 + j / 8] & 0x01) << 3)|
                        ((myUITlogo[ 8 * 40 + j / 8] & 0x01) << 4)|
                        ((myUITlogo[ 8 * 42 + j / 8] & 0x01) << 5)|
                        ((myUITlogo[ 8 * 44 + j / 8] & 0x01) << 6)|
                        ((myUITlogo[ 8 * 46 + j / 8] & 0x01) << 7);
    }

	for (int i = 384, j = 0; i < 512; i += 8, j += 8)
    {
        data[i + 0] =   ((myUITlogo[ 16 * 24 + j / 8] & 0x80) >> 7)|
                        ((myUITlogo[ 16 * 25 + j / 8] & 0x80) >> 6)|
                        ((myUITlogo[ 16 * 26 + j / 8] & 0x80) >> 5)| 
                        ((myUITlogo[ 16 * 27 + j / 8] & 0x80) >> 4)|
                        ((myUITlogo[ 16 * 28 + j / 8] & 0x80) >> 3)|
                        ((myUITlogo[ 16 * 29 + j / 8] & 0x80) >> 2)|
                        ((myUITlogo[ 16 * 30 + j / 8] & 0x80) >> 1)|
                        ((myUITlogo[ 16 * 31 + j / 8] & 0x80) >> 0);
		data[i + 1] =   ((myUITlogo[ 16 * 24 + j / 8] & 0x40) >> 6)|
                        ((myUITlogo[ 16 * 25 + j / 8] & 0x40) >> 5)|
                        ((myUITlogo[ 16 * 26 + j / 8] & 0x40) >> 4)| 
                        ((myUITlogo[ 16 * 27 + j / 8] & 0x40) >> 3)|
                        ((myUITlogo[ 16 * 28 + j / 8] & 0x40) >> 2)|
                        ((myUITlogo[ 16 * 29 + j / 8] & 0x40) >> 1)|
                        ((myUITlogo[ 16 * 30 + j / 8] & 0x40) >> 0)|
                        ((myUITlogo[ 16 * 31 + j / 8] & 0x40) << 1);
		data[i + 2] =   ((myUITlogo[ 16 * 24 + j / 8] & 0x20) >> 5)|
                        ((myUITlogo[ 16 * 25 + j / 8] & 0x20) >> 4)|
                        ((myUITlogo[ 16 * 26 + j / 8] & 0x20) >> 3)| 
                        ((myUITlogo[ 16 * 27 + j / 8] & 0x20) >> 2)|
                        ((myUITlogo[ 16 * 28 + j / 8] & 0x20) >> 1)|
                        ((myUITlogo[ 16 * 29 + j / 8] & 0x20) >> 0)|
                        ((myUITlogo[ 16 * 30 + j / 8] & 0x20) << 1)|
                        ((myUITlogo[ 16 * 31 + j / 8] & 0x20) << 2);
		data[i + 3] =   ((myUITlogo[ 16 * 24 + j / 8] & 0x10) >> 4)|
                        ((myUITlogo[ 16 * 25 + j / 8] & 0x10) >> 3)|
                        ((myUITlogo[ 16 * 26 + j / 8] & 0x10) >> 2)| 
                        ((myUITlogo[ 16 * 27 + j / 8] & 0x10) >> 1)|
                        ((myUITlogo[ 16 * 28 + j / 8] & 0x10) >> 0)|
                        ((myUITlogo[ 16 * 29 + j / 8] & 0x10) << 1)|
                        ((myUITlogo[ 16 * 30 + j / 8] & 0x10) << 2)|
                        ((myUITlogo[ 16 * 31 + j / 8] & 0x10) << 3);
		data[i + 4] =   ((myUITlogo[ 16 * 24 + j / 8] & 0x08) >> 3)|
                        ((myUITlogo[ 16 * 25 + j / 8] & 0x08) >> 2)|
                        ((myUITlogo[ 16 * 26 + j / 8] & 0x08) >> 1)| 
                        ((myUITlogo[ 16 * 27 + j / 8] & 0x08) >> 0)|
                        ((myUITlogo[ 16 * 28 + j / 8] & 0x08) << 1)|
                        ((myUITlogo[ 16 * 29 + j / 8] & 0x08) << 2)|
                        ((myUITlogo[ 16 * 30 + j / 8] & 0x08) << 3)|
                        ((myUITlogo[ 16 * 31 + j / 8] & 0x08) << 4);
		data[i + 5] =   ((myUITlogo[ 16 * 24 + j / 8] & 0x04) >> 2)|
                        ((myUITlogo[ 16 * 25 + j / 8] & 0x04) >> 1)|
                        ((myUITlogo[ 16 * 26 + j / 8] & 0x04) >> 0)| 
                        ((myUITlogo[ 16 * 27 + j / 8] & 0x04) << 1)|
                        ((myUITlogo[ 16 * 28 + j / 8] & 0x04) << 2)|
                        ((myUITlogo[ 16 * 29 + j / 8] & 0x04) << 3)|
                        ((myUITlogo[ 16 * 30 + j / 8] & 0x04) << 4)|
                        ((myUITlogo[ 16 * 31 + j / 8] & 0x04) << 5);
		data[i + 6] =   ((myUITlogo[ 16 * 24 + j / 8] & 0x02) >> 1)|
                        ((myUITlogo[ 16 * 25 + j / 8] & 0x02) >> 0)|
                        ((myUITlogo[ 16 * 26 + j / 8] & 0x02) << 1)| 
                        ((myUITlogo[ 16 * 27 + j / 8] & 0x02) << 2)|
                        ((myUITlogo[ 16 * 28 + j / 8] & 0x02) << 3)|
                        ((myUITlogo[ 16 * 29 + j / 8] & 0x02) << 4)|
                        ((myUITlogo[ 16 * 30 + j / 8] & 0x02) << 5)|
                        ((myUITlogo[ 16 * 31 + j / 8] & 0x02) << 6);
		data[i + 7] =   ((myUITlogo[ 16 * 24  + j / 8] & 0x01) >> 0)|
                        ((myUITlogo[ 16 * 25 + j / 8] & 0x01) << 1)|
                        ((myUITlogo[ 16 * 26 + j / 8] & 0x01) << 2)| 
                        ((myUITlogo[ 16 * 27 + j / 8] & 0x01) << 3)|
                        ((myUITlogo[ 16 * 28 + j / 8] & 0x01) << 4)|
                        ((myUITlogo[ 16 * 29 + j / 8] & 0x01) << 5)|
                        ((myUITlogo[ 16 * 30 + j / 8] & 0x01) << 6)|
                        ((myUITlogo[ 16 * 31 + j / 8] & 0x01) << 7);
    }

	for (int i = 512, j = 0; i < 640; i += 8, j += 8)
    {
        data[i + 0] =   ((myUITlogo[ 16 * 32 + j / 8] & 0x80) >> 7)|
                        ((myUITlogo[ 16 * 33 + j / 8] & 0x80) >> 6)|
                        ((myUITlogo[ 16 * 34 + j / 8] & 0x80) >> 5)| 
                        ((myUITlogo[ 16 * 35 + j / 8] & 0x80) >> 4)|
                        ((myUITlogo[ 16 * 36 + j / 8] & 0x80) >> 3)|
                        ((myUITlogo[ 16 * 37 + j / 8] & 0x80) >> 2)|
                        ((myUITlogo[ 16 * 38 + j / 8] & 0x80) >> 1)|
                        ((myUITlogo[ 16 * 39 + j / 8] & 0x80) >> 0);
		data[i + 1] =   ((myUITlogo[ 16 * 32 + j / 8] & 0x40) >> 6)|
                        ((myUITlogo[ 16 * 33 + j / 8] & 0x40) >> 5)|
                        ((myUITlogo[ 16 * 34 + j / 8] & 0x40) >> 4)| 
                        ((myUITlogo[ 16 * 35 + j / 8] & 0x40) >> 3)|
                        ((myUITlogo[ 16 * 36 + j / 8] & 0x40) >> 2)|
                        ((myUITlogo[ 16 * 37 + j / 8] & 0x40) >> 1)|
                        ((myUITlogo[ 16 * 38 + j / 8] & 0x40) >> 0)|
                        ((myUITlogo[ 16 * 39 + j / 8] & 0x40) << 1);
		data[i + 2] =   ((myUITlogo[ 16 * 32 + j / 8] & 0x20) >> 5)|
                        ((myUITlogo[ 16 * 33 + j / 8] & 0x20) >> 4)|
                        ((myUITlogo[ 16 * 34 + j / 8] & 0x20) >> 3)| 
                        ((myUITlogo[ 16 * 35 + j / 8] & 0x20) >> 2)|
                        ((myUITlogo[ 16 * 36 + j / 8] & 0x20) >> 1)|
                        ((myUITlogo[ 16 * 37 + j / 8] & 0x20) >> 0)|
                        ((myUITlogo[ 16 * 38 + j / 8] & 0x20) << 1)|
                        ((myUITlogo[ 16 * 39 + j / 8] & 0x20) << 2);
		data[i + 3] =   ((myUITlogo[ 16 * 32 + j / 8] & 0x10) >> 4)|
                        ((myUITlogo[ 16 * 33 + j / 8] & 0x10) >> 3)|
                        ((myUITlogo[ 16 * 34 + j / 8] & 0x10) >> 2)| 
                        ((myUITlogo[ 16 * 35 + j / 8] & 0x10) >> 1)|
                        ((myUITlogo[ 16 * 36 + j / 8] & 0x10) >> 0)|
                        ((myUITlogo[ 16 * 37 + j / 8] & 0x10) << 1)|
                        ((myUITlogo[ 16 * 38 + j / 8] & 0x10) << 2)|
                        ((myUITlogo[ 16 * 39 + j / 8] & 0x10) << 3);
		data[i + 4] =   ((myUITlogo[ 16 * 32 + j / 8] & 0x08) >> 3)|
                        ((myUITlogo[ 16 * 33 + j / 8] & 0x08) >> 2)|
                        ((myUITlogo[ 16 * 34 + j / 8] & 0x08) >> 1)| 
                        ((myUITlogo[ 16 * 35 + j / 8] & 0x08) >> 0)|
                        ((myUITlogo[ 16 * 36 + j / 8] & 0x08) << 1)|
                        ((myUITlogo[ 16 * 37 + j / 8] & 0x08) << 2)|
                        ((myUITlogo[ 16 * 38 + j / 8] & 0x08) << 3)|
                        ((myUITlogo[ 16 * 39 + j / 8] & 0x08) << 4);
		data[i + 5] =   ((myUITlogo[ 16 * 32 + j / 8] & 0x04) >> 2)|
                        ((myUITlogo[ 16 * 33 + j / 8] & 0x04) >> 1)|
                        ((myUITlogo[ 16 * 34 + j / 8] & 0x04) >> 0)| 
                        ((myUITlogo[ 16 * 35 + j / 8] & 0x04) << 1)|
                        ((myUITlogo[ 16 * 36 + j / 8] & 0x04) << 2)|
                        ((myUITlogo[ 16 * 37 + j / 8] & 0x04) << 3)|
                        ((myUITlogo[ 16 * 38 + j / 8] & 0x04) << 4)|
                        ((myUITlogo[ 16 * 39 + j / 8] & 0x04) << 5);
		data[i + 6] =   ((myUITlogo[ 16 * 32 + j / 8] & 0x02) >> 1)|
                        ((myUITlogo[ 16 * 33 + j / 8] & 0x02) >> 0)|
                        ((myUITlogo[ 16 * 34 + j / 8] & 0x02) << 1)| 
                        ((myUITlogo[ 16 * 35 + j / 8] & 0x02) << 2)|
                        ((myUITlogo[ 16 * 36 + j / 8] & 0x02) << 3)|
                        ((myUITlogo[ 16 * 37 + j / 8] & 0x02) << 4)|
                        ((myUITlogo[ 16 * 38 + j / 8] & 0x02) << 5)|
                        ((myUITlogo[ 16 * 39 + j / 8] & 0x02) << 6);
		data[i + 7] =   ((myUITlogo[ 16 * 32  + j / 8] & 0x01) >> 0)|
                        ((myUITlogo[ 16 * 33 + j / 8] & 0x01) << 1)|
                        ((myUITlogo[ 16 * 34 + j / 8] & 0x01) << 2)| 
                        ((myUITlogo[ 16 * 35 + j / 8] & 0x01) << 3)|
                        ((myUITlogo[ 16 * 36 + j / 8] & 0x01) << 4)|
                        ((myUITlogo[ 16 * 37 + j / 8] & 0x01) << 5)|
                        ((myUITlogo[ 16 * 38 + j / 8] & 0x01) << 6)|
                        ((myUITlogo[ 16 * 39 + j / 8] & 0x01) << 7);
    }

	for (int i = 640, j = 0; i < 768; i += 8, j += 8)
    {
        data[i + 0] =   ((myUITlogo[ 16 * 40 + j / 8] & 0x80) >> 7)|
                        ((myUITlogo[ 16 * 41 + j / 8] & 0x80) >> 6)|
                        ((myUITlogo[ 16 * 42 + j / 8] & 0x80) >> 5)| 
                        ((myUITlogo[ 16 * 43 + j / 8] & 0x80) >> 4)|
                        ((myUITlogo[ 16 * 44 + j / 8] & 0x80) >> 3)|
                        ((myUITlogo[ 16 * 45 + j / 8] & 0x80) >> 2)|
                        ((myUITlogo[ 16 * 46 + j / 8] & 0x80) >> 1)|
                        ((myUITlogo[ 16 * 47 + j / 8] & 0x80) >> 0);
		data[i + 1] =   ((myUITlogo[ 16 * 40 + j / 8] & 0x40) >> 6)|
                        ((myUITlogo[ 16 * 41 + j / 8] & 0x40) >> 5)|
                        ((myUITlogo[ 16 * 42 + j / 8] & 0x40) >> 4)| 
                        ((myUITlogo[ 16 * 43 + j / 8] & 0x40) >> 3)|
                        ((myUITlogo[ 16 * 44 + j / 8] & 0x40) >> 2)|
                        ((myUITlogo[ 16 * 45 + j / 8] & 0x40) >> 1)|
                        ((myUITlogo[ 16 * 46 + j / 8] & 0x40) >> 0)|
                        ((myUITlogo[ 16 * 47 + j / 8] & 0x40) << 1);
		data[i + 2] =   ((myUITlogo[ 16 * 40 + j / 8] & 0x20) >> 5)|
                        ((myUITlogo[ 16 * 41 + j / 8] & 0x20) >> 4)|
                        ((myUITlogo[ 16 * 42 + j / 8] & 0x20) >> 3)| 
                        ((myUITlogo[ 16 * 43 + j / 8] & 0x20) >> 2)|
                        ((myUITlogo[ 16 * 44 + j / 8] & 0x20) >> 1)|
                        ((myUITlogo[ 16 * 45 + j / 8] & 0x20) >> 0)|
                        ((myUITlogo[ 16 * 46 + j / 8] & 0x20) << 1)|
                        ((myUITlogo[ 16 * 47 + j / 8] & 0x20) << 2);
		data[i + 3] =   ((myUITlogo[ 16 * 40 + j / 8] & 0x10) >> 4)|
                        ((myUITlogo[ 16 * 41 + j / 8] & 0x10) >> 3)|
                        ((myUITlogo[ 16 * 42 + j / 8] & 0x10) >> 2)| 
                        ((myUITlogo[ 16 * 43 + j / 8] & 0x10) >> 1)|
                        ((myUITlogo[ 16 * 44 + j / 8] & 0x10) >> 0)|
                        ((myUITlogo[ 16 * 45 + j / 8] & 0x10) << 1)|
                        ((myUITlogo[ 16 * 46 + j / 8] & 0x10) << 2)|
                        ((myUITlogo[ 16 * 47 + j / 8] & 0x10) << 3);
		data[i + 4] =   ((myUITlogo[ 16 * 40 + j / 8] & 0x08) >> 3)|
                        ((myUITlogo[ 16 * 41 + j / 8] & 0x08) >> 2)|
                        ((myUITlogo[ 16 * 42 + j / 8] & 0x08) >> 1)| 
                        ((myUITlogo[ 16 * 43 + j / 8] & 0x08) >> 0)|
                        ((myUITlogo[ 16 * 44 + j / 8] & 0x08) << 1)|
                        ((myUITlogo[ 16 * 45 + j / 8] & 0x08) << 2)|
                        ((myUITlogo[ 16 * 46 + j / 8] & 0x08) << 3)|
                        ((myUITlogo[ 16 * 47 + j / 8] & 0x08) << 4);
		data[i + 5] =   ((myUITlogo[ 16 * 40 + j / 8] & 0x04) >> 2)|
                        ((myUITlogo[ 16 * 41 + j / 8] & 0x04) >> 1)|
                        ((myUITlogo[ 16 * 42 + j / 8] & 0x04) >> 0)| 
                        ((myUITlogo[ 16 * 43 + j / 8] & 0x04) << 1)|
                        ((myUITlogo[ 16 * 44 + j / 8] & 0x04) << 2)|
                        ((myUITlogo[ 16 * 45 + j / 8] & 0x04) << 3)|
                        ((myUITlogo[ 16 * 46 + j / 8] & 0x04) << 4)|
                        ((myUITlogo[ 16 * 47 + j / 8] & 0x04) << 5);
		data[i + 6] =   ((myUITlogo[ 16 * 40 + j / 8] & 0x02) >> 1)|
                        ((myUITlogo[ 16 * 41 + j / 8] & 0x02) >> 0)|
                        ((myUITlogo[ 16 * 42 + j / 8] & 0x02) << 1)| 
                        ((myUITlogo[ 16 * 43 + j / 8] & 0x02) << 2)|
                        ((myUITlogo[ 16 * 44 + j / 8] & 0x02) << 3)|
                        ((myUITlogo[ 16 * 45 + j / 8] & 0x02) << 4)|
                        ((myUITlogo[ 16 * 46 + j / 8] & 0x02) << 5)|
                        ((myUITlogo[ 16 * 47 + j / 8] & 0x02) << 6);
		data[i + 7] =   ((myUITlogo[ 16 * 40  + j / 8] & 0x01) >> 0)|
                        ((myUITlogo[ 16 * 41 + j / 8] & 0x01) << 1)|
                        ((myUITlogo[ 16 * 42 + j / 8] & 0x01) << 2)| 
                        ((myUITlogo[ 16 * 43 + j / 8] & 0x01) << 3)|
                        ((myUITlogo[ 16 * 44 + j / 8] & 0x01) << 4)|
                        ((myUITlogo[ 16 * 45 + j / 8] & 0x01) << 5)|
                        ((myUITlogo[ 16 * 46 + j / 8] & 0x01) << 6)|
                        ((myUITlogo[ 16 * 47 + j / 8] & 0x01) << 7);
	}
	for (int i = 768, j = 0; i < 896; i += 8, j += 8)
    {
        data[i + 0] =   ((myUITlogo[ 16 * 48 + j / 8] & 0x80) >> 7)|
                        ((myUITlogo[ 16 * 49 + j / 8] & 0x80) >> 6)|
                        ((myUITlogo[ 16 * 50 + j / 8] & 0x80) >> 5)| 
                        ((myUITlogo[ 16 * 51 + j / 8] & 0x80) >> 4)|
                        ((myUITlogo[ 16 * 52 + j / 8] & 0x80) >> 3)|
                        ((myUITlogo[ 16 * 53 + j / 8] & 0x80) >> 2)|
                        ((myUITlogo[ 16 * 54 + j / 8] & 0x80) >> 1)|
                        ((myUITlogo[ 16 * 55 + j / 8] & 0x80) >> 0);
		data[i + 1] =   ((myUITlogo[ 16 * 48 + j / 8] & 0x40) >> 6)|
                        ((myUITlogo[ 16 * 49 + j / 8] & 0x40) >> 5)|
                        ((myUITlogo[ 16 * 50 + j / 8] & 0x40) >> 4)| 
                        ((myUITlogo[ 16 * 51 + j / 8] & 0x40) >> 3)|
                        ((myUITlogo[ 16 * 52 + j / 8] & 0x40) >> 2)|
                        ((myUITlogo[ 16 * 53 + j / 8] & 0x40) >> 1)|
                        ((myUITlogo[ 16 * 54 + j / 8] & 0x40) >> 0)|
                        ((myUITlogo[ 16 * 55 + j / 8] & 0x40) << 1);
		data[i + 2] =   ((myUITlogo[ 16 * 48 + j / 8] & 0x20) >> 5)|
                        ((myUITlogo[ 16 * 49 + j / 8] & 0x20) >> 4)|
                        ((myUITlogo[ 16 * 50 + j / 8] & 0x20) >> 3)| 
                        ((myUITlogo[ 16 * 51 + j / 8] & 0x20) >> 2)|
                        ((myUITlogo[ 16 * 52 + j / 8] & 0x20) >> 1)|
                        ((myUITlogo[ 16 * 53 + j / 8] & 0x20) >> 0)|
                        ((myUITlogo[ 16 * 54 + j / 8] & 0x20) << 1)|
                        ((myUITlogo[ 16 * 55 + j / 8] & 0x20) << 2);
		data[i + 3] =   ((myUITlogo[ 16 * 48 + j / 8] & 0x10) >> 4)|
                        ((myUITlogo[ 16 * 49 + j / 8] & 0x10) >> 3)|
                        ((myUITlogo[ 16 * 50 + j / 8] & 0x10) >> 2)| 
                        ((myUITlogo[ 16 * 51 + j / 8] & 0x10) >> 1)|
                        ((myUITlogo[ 16 * 52 + j / 8] & 0x10) >> 0)|
                        ((myUITlogo[ 16 * 53 + j / 8] & 0x10) << 1)|
                        ((myUITlogo[ 16 * 54 + j / 8] & 0x10) << 2)|
                        ((myUITlogo[ 16 * 55 + j / 8] & 0x10) << 3);
		data[i + 4] =   ((myUITlogo[ 16 * 48 + j / 8] & 0x08) >> 3)|
                        ((myUITlogo[ 16 * 49 + j / 8] & 0x08) >> 2)|
                        ((myUITlogo[ 16 * 50 + j / 8] & 0x08) >> 1)| 
                        ((myUITlogo[ 16 * 51 + j / 8] & 0x08) >> 0)|
                        ((myUITlogo[ 16 * 52 + j / 8] & 0x08) << 1)|
                        ((myUITlogo[ 16 * 53 + j / 8] & 0x08) << 2)|
                        ((myUITlogo[ 16 * 54 + j / 8] & 0x08) << 3)|
                        ((myUITlogo[ 16 * 55 + j / 8] & 0x08) << 4);
		data[i + 5] =   ((myUITlogo[ 16 * 48 + j / 8] & 0x04) >> 2)|
                        ((myUITlogo[ 16 * 49 + j / 8] & 0x04) >> 1)|
                        ((myUITlogo[ 16 * 50 + j / 8] & 0x04) >> 0)| 
                        ((myUITlogo[ 16 * 51 + j / 8] & 0x04) << 1)|
                        ((myUITlogo[ 16 * 52 + j / 8] & 0x04) << 2)|
                        ((myUITlogo[ 16 * 53 + j / 8] & 0x04) << 3)|
                        ((myUITlogo[ 16 * 54 + j / 8] & 0x04) << 4)|
                        ((myUITlogo[ 16 * 55 + j / 8] & 0x04) << 5);
		data[i + 6] =   ((myUITlogo[ 16 * 48 + j / 8] & 0x02) >> 1)|
                        ((myUITlogo[ 16 * 49 + j / 8] & 0x02) >> 0)|
                        ((myUITlogo[ 16 * 50 + j / 8] & 0x02) << 1)| 
                        ((myUITlogo[ 16 * 51 + j / 8] & 0x02) << 2)|
                        ((myUITlogo[ 16 * 52 + j / 8] & 0x02) << 3)|
                        ((myUITlogo[ 16 * 53 + j / 8] & 0x02) << 4)|
                        ((myUITlogo[ 16 * 54 + j / 8] & 0x02) << 5)|
                        ((myUITlogo[ 16 * 55 + j / 8] & 0x02) << 6);
		data[i + 7] =   ((myUITlogo[ 16 * 48 + j / 8] & 0x01) >> 0)|
                        ((myUITlogo[ 16 * 49 + j / 8] & 0x01) << 1)|
                        ((myUITlogo[ 16 * 50 + j / 8] & 0x01) << 2)| 
                        ((myUITlogo[ 16 * 51 + j / 8] & 0x01) << 3)|
                        ((myUITlogo[ 16 * 52 + j / 8] & 0x01) << 4)|
                        ((myUITlogo[ 16 * 53 + j / 8] & 0x01) << 5)|
                        ((myUITlogo[ 16 * 54 + j / 8] & 0x01) << 6)|
                        ((myUITlogo[ 16 * 55 + j / 8] & 0x01) << 7);
    }
    
	for (int i = 896, j = 0; i < 1024; i += 8, j += 8)
    {
        data[i + 0] =   ((myUITlogo[ 16 * 56 + j / 8] & 0x80) >> 7)|
                        ((myUITlogo[ 16 * 57 + j / 8] & 0x80) >> 6)|
                        ((myUITlogo[ 16 * 58 + j / 8] & 0x80) >> 5)| 
                        ((myUITlogo[ 16 * 59 + j / 8] & 0x80) >> 4)|
                        ((myUITlogo[ 16 * 60 + j / 8] & 0x80) >> 3)|
                        ((myUITlogo[ 16 * 61 + j / 8] & 0x80) >> 2)|
                        ((myUITlogo[ 16 * 62 + j / 8] & 0x80) >> 1)|
                        ((myUITlogo[ 16 * 63 + j / 8] & 0x80) >> 0);
		data[i + 1] =   ((myUITlogo[ 16 * 56 + j / 8] & 0x40) >> 6)|
                        ((myUITlogo[ 16 * 57 + j / 8] & 0x40) >> 5)|
                        ((myUITlogo[ 16 * 58 + j / 8] & 0x40) >> 4)| 
                        ((myUITlogo[ 16 * 59 + j / 8] & 0x40) >> 3)|
                        ((myUITlogo[ 16 * 60 + j / 8] & 0x40) >> 2)|
                        ((myUITlogo[ 16 * 61 + j / 8] & 0x40) >> 1)|
                        ((myUITlogo[ 16 * 62 + j / 8] & 0x40) >> 0)|
                        ((myUITlogo[ 16 * 63 + j / 8] & 0x40) << 1);
		data[i + 2] =   ((myUITlogo[ 16 * 56 + j / 8] & 0x20) >> 5)|
                        ((myUITlogo[ 16 * 57 + j / 8] & 0x20) >> 4)|
                        ((myUITlogo[ 16 * 58 + j / 8] & 0x20) >> 3)| 
                        ((myUITlogo[ 16 * 59 + j / 8] & 0x20) >> 2)|
                        ((myUITlogo[ 16 * 60 + j / 8] & 0x20) >> 1)|
                        ((myUITlogo[ 16 * 61 + j / 8] & 0x20) >> 0)|
                        ((myUITlogo[ 16 * 62 + j / 8] & 0x20) << 1)|
                        ((myUITlogo[ 16 * 63 + j / 8] & 0x20) << 2);
		data[i + 3] =   ((myUITlogo[ 16 * 56 + j / 8] & 0x10) >> 4)|
                        ((myUITlogo[ 16 * 57 + j / 8] & 0x10) >> 3)|
                        ((myUITlogo[ 16 * 58 + j / 8] & 0x10) >> 2)| 
                        ((myUITlogo[ 16 * 59 + j / 8] & 0x10) >> 1)|
                        ((myUITlogo[ 16 * 60 + j / 8] & 0x10) >> 0)|
                        ((myUITlogo[ 16 * 61 + j / 8] & 0x10) << 1)|
                        ((myUITlogo[ 16 * 62 + j / 8] & 0x10) << 2)|
                        ((myUITlogo[ 16 * 63 + j / 8] & 0x10) << 3);
		data[i + 4] =   ((myUITlogo[ 16 * 56 + j / 8] & 0x08) >> 3)|
                        ((myUITlogo[ 16 * 57 + j / 8] & 0x08) >> 2)|
                        ((myUITlogo[ 16 * 58 + j / 8] & 0x08) >> 1)| 
                        ((myUITlogo[ 16 * 59 + j / 8] & 0x08) >> 0)|
                        ((myUITlogo[ 16 * 60 + j / 8] & 0x08) << 1)|
                        ((myUITlogo[ 16 * 61 + j / 8] & 0x08) << 2)|
                        ((myUITlogo[ 16 * 62 + j / 8] & 0x08) << 3)|
                        ((myUITlogo[ 16 * 63 + j / 8] & 0x08) << 4);
		data[i + 5] =   ((myUITlogo[ 16 * 56 + j / 8] & 0x04) >> 2)|
                        ((myUITlogo[ 16 * 57 + j / 8] & 0x04) >> 1)|
                        ((myUITlogo[ 16 * 58 + j / 8] & 0x04) >> 0)| 
                        ((myUITlogo[ 16 * 59 + j / 8] & 0x04) << 1)|
                        ((myUITlogo[ 16 * 60 + j / 8] & 0x04) << 2)|
                        ((myUITlogo[ 16 * 61 + j / 8] & 0x04) << 3)|
                        ((myUITlogo[ 16 * 62 + j / 8] & 0x04) << 4)|
                        ((myUITlogo[ 16 * 63 + j / 8] & 0x04) << 5);
		data[i + 6] =   ((myUITlogo[ 16 * 56 + j / 8] & 0x02) >> 1)|
                        ((myUITlogo[ 16 * 57 + j / 8] & 0x02) >> 0)|
                        ((myUITlogo[ 16 * 58 + j / 8] & 0x02) << 1)| 
                        ((myUITlogo[ 16 * 59 + j / 8] & 0x02) << 2)|
                        ((myUITlogo[ 16 * 60 + j / 8] & 0x02) << 3)|
                        ((myUITlogo[ 16 * 61 + j / 8] & 0x02) << 4)|
                        ((myUITlogo[ 16 * 62 + j / 8] & 0x02) << 5)|
                        ((myUITlogo[ 16 * 63 + j / 8] & 0x02) << 6);
		data[i + 7] =   ((myUITlogo[ 16 * 56 + j / 8] & 0x01) >> 0)|
                        ((myUITlogo[ 16 * 57 + j / 8] & 0x01) << 1)|
                        ((myUITlogo[ 16 * 58 + j / 8] & 0x01) << 2)| 
                        ((myUITlogo[ 16 * 59 + j / 8] & 0x01) << 3)|
                        ((myUITlogo[ 16 * 60 + j / 8] & 0x01) << 4)|
                        ((myUITlogo[ 16 * 61 + j / 8] & 0x01) << 5)|
                        ((myUITlogo[ 16 * 62 + j / 8] & 0x01) << 6)|
                        ((myUITlogo[ 16 * 63 + j / 8] & 0x01) << 7);
    }
}

void SelectPage(uint8_t page)
{
	uint8_t cur_page = page;
	i2c_cmd_handle_t cmd;
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

	i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
	i2c_master_write_byte(cmd, 0x00, true); // reset column - choose column --> 0
	i2c_master_write_byte(cmd, 0x10, true); // reset line - choose line --> 0
	i2c_master_write_byte(cmd, 0xB0 | cur_page, true); // reset page

	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
}

void ssd1306_display_bitmap() {

	uint8_t cur_page = 0;

	i2c_cmd_handle_t cmd;
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

	i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
	i2c_master_write_byte(cmd, 0x00, true); // reset column - choose column --> 0
	i2c_master_write_byte(cmd, 0x10, true); // reset line - choose line --> 0
	i2c_master_write_byte(cmd, 0xB0 | cur_page, true); // reset page

	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
	
	//display text
	ConvertBitmap();
	for (int i = 0; i < 1024; i++)
	{
		cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
		i2c_master_write_byte(cmd, 0xC0, true);


		i2c_master_write_byte(cmd, data[i], true);


		i2c_master_stop(cmd);
		i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
		i2c_cmd_link_delete(cmd);

		if (i % 128 == 0 && i != 0)
			SelectPage(i / 127);
	}
	

	vTaskDelete(NULL);
}

void ssd1306_draw_pikachu()
{
	// 'download (1)', 128x64px
	const unsigned char pikachu [1024] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xbf, 0xff, 0xdf, 0xdf, 0xdf, 0xcf, 0xef, 0xef, 0xe7, 
	0xe7, 0xe7, 0xe7, 0xe3, 0xe3, 0xe3, 0xe3, 0xe1, 0xe1, 0x61, 0xe1, 0xe1, 0xe0, 0xe0, 0xf0, 0xf8, 
	0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xdf, 0xff, 0xf7, 0xfb, 
	0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xdf, 0xff, 
	0xef, 0xff, 0x77, 0x7f, 0xfb, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xef, 0xff, 0xff, 0xfb, 0xff, 0x3d, 0x1f, 0x1f, 0x1e, 0x1f, 0x1f, 0x7f, 0x7f, 0x7f, 
	0x7f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 
	0xfd, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xdf, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xfd, 0xfd, 0xf9, 0xf0, 0xf0, 0xe0, 0xe0, 
	0xc0, 0x80, 0x41, 0xe1, 0xe1, 0xe1, 0xf3, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xff, 
	0xff, 0x07, 0x87, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x8f, 0x0f, 0x0d, 0x0d, 0x1f, 0x1f, 
	0x9f, 0xdf, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x73, 0x63, 0x61, 0x65, 0x27, 
	0x27, 0x27, 0x25, 0x21, 0x33, 0x73, 0x7f, 0x7f, 0x7f, 0xbf, 0xef, 0xf7, 0xfb, 0xf9, 0xfc, 0xf7, 
	0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7d, 
	0xff, 0xff, 0xfb, 0xff, 0xf7, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf9, 0xf7, 0xff, 0xff, 0xdf, 0xbf, 0xbf, 0x7f, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xfe, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xfc, 0xff, 0xfb, 0xfb, 0xfb, 0xff, 0xfb, 0xfb, 0xff, 0xff, 0xfd, 0xff, 0xfe, 0xff, 0xff, 
	0xff, 0xff, 0xdf, 0xdf, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xf8, 0xf8, 0xf0, 0xe0, 0xe0, 0xf0, 
	0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf8, 0xf8, 0xec, 0xec, 0xee, 0xff, 0xff, 0xdf, 0xdf, 0xff, 0xff, 
	0xbf, 0xbf, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 
	0xfe, 0xfe, 0xfc, 0xfc, 0xfc, 0xfc, 0xdc, 0xfc, 0xfc, 0xf8, 0xf8, 0xf8, 0xf8, 0xf8, 0xf9, 0xfb, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xff, 0xdf, 0xef, 0xfb, 
	0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x7f, 
	0xff, 0xff, 0xff, 0xff, 0xbf, 0xbf, 0xbf, 0xbf, 0x3f, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 0xbf, 
	0xbf, 0xbf, 0xbf, 0xff, 0xff, 0xff, 0xbf, 0xfe, 0xff, 0xfd, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xf7, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
	0xf3, 0xdf, 0xbf, 0xff, 0xfd, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xdf, 0xef, 0xdf, 0xef, 0xe7, 0xf7, 0xff, 0xfe, 0xff, 0xf9, 0xfb, 0xdf, 0xff, 0xfb, 0xfb, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xef, 0xe7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 
	0xf7, 0xff, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 
	0xdf, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xbf, 0xff, 0xff, 0xf7, 0xf7, 0xff, 0xff, 0xff, 0x7f, 
	0xff, 0xff, 0xfe, 0xff, 0x3f, 0xff, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

	i2c_cmd_handle_t cmd;
	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

	i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
	i2c_master_write_byte(cmd, 0x00, true); // reset column - choose column --> 0
	i2c_master_write_byte(cmd, 0x10, true); // reset line - choose line --> 0
	i2c_master_write_byte(cmd, 0xB0 | 0, true); // reset page

	i2c_master_stop(cmd);
	i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
	i2c_cmd_link_delete(cmd);
	
	//display text
	
	for (int i = 0; i < 1024; i++)
	{
		cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
		i2c_master_write_byte(cmd, 0xC0, true);


		i2c_master_write_byte(cmd, pikachu[i], true);


		i2c_master_stop(cmd);
		i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
		i2c_cmd_link_delete(cmd);

		SelectPage(i % 8);
	}
	

	vTaskDelete(NULL);

}

void task_ssd1306_display_clear(void *ignore) {
	i2c_cmd_handle_t cmd;

	uint8_t clear[128];
	for (uint8_t i = 0; i < 128; i++) {
		clear[i] = 0;
	}
	for (uint8_t i = 0; i < 8; i++) {
		cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
		i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_SINGLE, true);
		i2c_master_write_byte(cmd, 0xB0 | i, true);

		i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_DATA_STREAM, true);
		i2c_master_write(cmd, clear, 128, true);
		i2c_master_stop(cmd);
		i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
		i2c_cmd_link_delete(cmd);
	}

	vTaskDelete(NULL);
}