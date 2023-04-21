#ifndef _FUJI_MENU_
#define _FUJI_MENU_

#include <stdio.h>
#include "fnFS.h"

#define MAX_MENU_SIZE 65535
#define MAX_MENU_LINE 256
#define MAX_MENU_LINES 4096
#define MAX_MENU_NAME_LEN 40
#define MAX_MENU_RESOURCE_LEN 256

//
// Menu line format is:
//
//     <entry>    := <type> <display> <resource> | <STRING>
//     <type>     := 00 | 01 | 02 | 03 "|"
//     <display>  := <STRING> "|"
//     <resource> := <STRING>
//
// If the line contains no type then type "00" is assumed.


class fujiMenu
{
private:

    FILE * _menu_file = nullptr;
    uint16_t _current_offset = 0;
    uint16_t _current_pos = 0;
    fsdir_entry _direntry;
    int8_t decode_menutype(const char * buf);

public:

    fujiMenu() {};
    ~fujiMenu() {};

    bool init(const char *path, FILE * mf);
    void release();
    bool get_initialized() { return (_menu_file != nullptr); };
    uint16_t get_pos() { return _current_offset; };
    bool set_pos(uint16_t newPos);
    fsdir_entry_t * get_next_menu_entry();
};

#endif // _FUJI_MENU_
