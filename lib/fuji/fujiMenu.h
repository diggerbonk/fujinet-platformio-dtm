#ifndef _FUJI_MENU_
#define _FUJI_MENU_

#include <stdio.h>
#include "fnFS.h"

#define MAX_MENU_SIZE 65535
#define MAX_MENU_LINE_LEN 256
#define MAX_MENU_LINES 4096

//
// Menu line format is:
//
//     <entry>    := [<type> "|"]<resource>
//     <type>     := Hex Byte in ASCII (e.g. "0A")
//     <resource> := <STRING>

class fujiMenu
{
private:

    FILE * _menu_file = nullptr;
    uint16_t _current_offset = 0;
    uint16_t _current_pos = 0;
    fsdir_entry _direntry;
    int8_t _type = 0;
    int8_t decode_menutype(const char * buf);
    

public:

    fujiMenu() {};
    ~fujiMenu() {};

    uint8_t get_menu_entry_type() { return _type; };
    bool init(const char *path, FILE * mf);
    void release();
    bool get_initialized() { return (_menu_file != nullptr); };
    uint16_t get_pos() { return _current_offset; };
    bool set_pos(uint16_t newPos);
    fsdir_entry_t * get_next_menu_entry();
};

#endif // _FUJI_MENU_
