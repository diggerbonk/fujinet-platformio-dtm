#ifndef _FUJI_MENU_
#define _FUJI_MENU_

#include <stdio.h>
#include "fnFS.h"

#define MAX_MENU_SIZE 65535
#define MAX_MENU_NAME_LEN 80
#define MAX_MENU_RESOURCE_LEN 168
#define MAX_MENU_LINE_LEN MAX_PATHLEN
#define MAX_MENU_LINES 4096

enum fujiMenuType
{
    MENU_TYPE_TEXT = 0,
    MENU_TYPE_FOLDER,
    MENU_TYPE_BINARY,
    MENU_TYPE_LINK
};

class fujiMenu
{
private:

    FILE * _menu_file = nullptr;
    uint16_t _current_offset = 0;
    uint16_t _current_pos = 0;
    fsdir_entry _direntry;

    uint8_t _type = 0;
    uint8_t _name_len = 0;
    char _name[MAX_MENU_NAME_LEN];
    uint8_t _resource_len = 0;
    char _resource[MAX_MENU_RESOURCE_LEN];
    uint8_t decode_menutype(const char * buf);

public:

    fujiMenu() {};
    ~fujiMenu() {};

    uint8_t get_menu_entry_type() { return _type; };
    uint8_t get_name_len() { return _name_len; };
    uint8_t get_resource_len() { return _resource_len; };
    const char * get_name() { return _name; };
    const char * get_resource() { return _resource; };
    bool init(const char *path, FILE * mf);
    void release();
    bool get_initialized() { return (_menu_file != nullptr); };
    uint16_t get_pos() { return _current_offset; };
    bool set_pos(uint16_t newPos);
    fsdir_entry_t * get_next_menu_entry();
};

#endif // _FUJI_MENU_
