﻿#include <stdio.h>
#include <wchar.h>
#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include "test.h"
#include "libtest.h"

int tests_count = 0;

int main(void)
{
	int ret = 0;
	Logger_SetLevel(LOGGER_LEVEL_OFF);
	describe("test charset", test_charset);
	describe("test linkedlist", test_linkedlist);
	describe("test string", test_string);
	describe("test strpool", test_strpool);
	describe("test object", test_object);
	desrcibe("test thread", test_thread);
	describe("teest font_load", test_font_load);
	describe("test image_reader", test_image_reader);
	describe("test xml_parser", test_xml_parser);
	describe("test widget_event", test_widget_event);
	describe("test widget_opacity", test_widget_opacity);
	describe("test textview_resize", test_textview_resize);
	describe("test textedit", test_textedit);
	describe("test mainloop", test_mainloop);
	describe("test css parser", test_css_parser);
	describe("test block layout", test_block_layout);
	describe("test flex layout", test_flex_layout);
	describe("test widget rect", test_widget_rect);
	return ret - print_test_result();
}
