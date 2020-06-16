#include <stdio.h>

#define TEST_LOG(format, ...) \
	printf("[test] %s(): " format, __FUNCTION__, ##__VA_ARGS__)

#define PRINT_TEST_RESULT(N) \
	printf("[test] %d tests, %d pass.\n", tests_count, tests_count + N)

#define CHECK(X)                                                          \
	do {                                                              \
		tests_count += 1;                                         \
		printf("[test] %s(): %s. # %s\n", __FUNCTION__, "" #X "", \
		       (X) ? "PASS" : (ret -= 1, "NO PASS!"));            \
	} while (0);

#define CHECK_WITH_TEXT(TEXT, X)                                      \
	do {                                                          \
		tests_count += 1;                                     \
		printf("[test] %s(): %s. # %s\n", __FUNCTION__, TEXT, \
		       (X) ? "PASS" : (ret -= 1, "NO PASS!"));        \
	} while (0);

#define CHECK2(X)                                                             \
	do {                                                                  \
		if (!(X)) {                                                   \
			printf("[test] %s(): %s. # NO PASS!\n", __FUNCTION__, \
			       "" #X "");                                     \
			ret -= 1;                                             \
		}                                                             \
	} while (0);

extern int tests_count;

void test_charset(void);
void test_string(void);
void test_object(void);
void test_thread(void);
void test_font_load(void);
void test_xml_parser(void);
void test_strpool(void);
void test_linkedlist(void);
void test_widget_opacity(void);
void test_widget_event(void);
void test_textview_resize(void);
void test_textedit(void);
void test_image_reader(void);

void test_css_parser(void);
void test_mainloop(void);
void test_block_layout(void);
void test_flex_layout(void);
void test_widget_rect(void);
