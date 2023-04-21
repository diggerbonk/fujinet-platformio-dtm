
#include "fujiMenu.h"

#include <cstring>
#include <stdlib.h>
#include <stdio.h>

#include "../../include/debug.h"

#include "fnFsSD.h"
#include "fnFsTNFS.h"

#include "utils.h"

bool fujiMenu::init(const char *path, FILE * mf)
{
    Debug_printf("fujiMenu::init, IN");
    release();
    _menu_file = mf;
    return true;
}

void fujiMenu::release()
{
    _current_offset = 0;
    _current_pos = 0;

    if (_menu_file != nullptr) 
    {
        fclose(_menu_file);
        _menu_file = nullptr;
    }
}

bool fujiMenu::set_pos(uint16_t newPos) 
{

    char tempBuf[MAX_MENU_LINE];

    // find the offset of the new menu position.
    _current_pos = 0;
    _current_offset = 0;
    int linelen = 0;

    while (_current_pos < newPos && fgets(tempBuf, MAX_MENU_LINE, _menu_file)) {

        linelen = strlen(tempBuf);

        if (linelen <= 0)
        {
            break;
        }
        else if ((_current_offset + linelen) > MAX_MENU_SIZE) {
            Debug_printf("fujiMenu::init, truncating menu because it is over MAX_MENU_SIZE");
            break;
        }
        else if ((_current_pos + 1) > MAX_MENU_LINES) {
            Debug_printf("fujiMenu::init, truncating menu because it is over MAX_MENU_LINES");
            break;
        }

        _current_pos += 1;
        _current_offset += linelen;
    }

    return true;
}

int8_t fujiMenu::decode_menutype(const char * buf)
{
    return (int8_t)strtol(buf, nullptr, 16);
}

fsdir_entry_t * fujiMenu::get_next_menu_entry() 
{
    char tempBuf[MAX_MENU_LINE];
    uint8_t tempType = 0;

    // if we have an offset, skip to it. 
    if (_current_offset > 0) 
    {
        if (fseek(_menu_file, _current_offset, 0) != 0)
        {
            Debug_printf("fujiMenu::get_next_menu_entry, cannot seek to current offset.");
            return nullptr;
        }
    }

    if (fgets(tempBuf, MAX_MENU_LINE, _menu_file)) 
    {
        _current_pos += 1;
        _current_offset += strlen(tempBuf);

        _direntry.isDir = false;
        _direntry.size = 0;
        _direntry.modified_time = 0;

        // menu format: <type>|<display>|<resource>
        //              <display>

        // replace trailing newline. 
        int len = strlen(tempBuf);
        if (len>0) tempBuf[len-1] = 0;

        int16_t menuType = 0;
        uint16_t displayStart = 0;
        uint16_t resourceStart = len;
        
        if (len > 3 && tempBuf[2] == '|') {
            menuType = decode_menutype(tempBuf);
            displayStart = 3;
        }

        if (menuType > 0) 
        {
            resourceStart = strcspn(&tempBuf[displayStart], "|");
            if (resourceStart <= displayStart) resourceStart = len;
        }

        memcpy(_direntry.filename, &tempBuf[displayStart], resourceStart-displayStart);
        
        return &_direntry;
    }
    else return nullptr;
}
