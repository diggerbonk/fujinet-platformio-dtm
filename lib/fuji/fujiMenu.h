#ifndef _FUJI_MENU_
#define _FUJI_MENU_

#include <stdio.h>
#include "fnFS.h"

#define MAX_MENU_SIZE 65535
#define MAX_MENU_LINE 256
#define MAX_MENU_LINES 16384
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
    bool _initialized;
    char _path[MAX_PATHLEN] = { '\0' };
    uint16_t * _line_offsets;
    uint16_t _num_lines = 0;
    uint16_t _menu_size = 0;
    uint16_t _current_offset = 0;

public:

    fujiMenu() { _initialized = false; };
    ~fujiMenu() { _initialized = false; };

    bool init(const char *path, FILE * mf);
    void release() { _initialized = false; };
    bool get_initialized() { return _initialized; };
    const char * get_path() { return _path; };
    uint16_t get_pos() { return _current_offset; };
    bool set_pos(uint16_t newPos);
    fsdir_entry_t * get_current_menu_entry();
};

#endif // _FUJI_MENU_
