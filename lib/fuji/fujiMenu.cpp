
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
    ssize_t read;
    const int bufferlen = 256;
    char buf[256];
    size_t len = 0;

    if (_line_offsets != nullptr) 
    {
        free(_line_offsets);
        _line_offsets = nullptr;
    }

    _line_offsets = (uint16_t*)malloc(MAX_MENU_LINES*2);

    strlcpy(_path, path, sizeof(_path)); 

    _menu_file = mf;

    // walk through file, get each newline position
 
    _num_lines = 1;
 
    _menu_size = 0;
    while (fgets(buf, bufferlen, _menu_file)) {
        _menu_size += strlen(buf);
        if (_menu_size >= MAX_MENU_SIZE) {
            Debug_printf("fujiMenu::init, truncating menu because it is over MAX_MENU_SIZE");
            _menu_size -= strlen(buf);
            break;
        }
        else if (_num_lines >= MAX_MENU_LINES) {
            Debug_printf("fujiMenu::init, truncating menu because it is over MAX_MENU_LINES");
            break;
        }
        _line_offsets[_num_lines] = _menu_size;
        _num_lines++;
    }

    _initialized = true; 
    return true;
}

bool fujiMenu::set_pos(uint16_t newPos) 
{
    if (newPos >= _num_lines) {
        return false;
    }
    else {
        _current_offset = newPos;
        fseek(_menu_file, _current_offset, SEEK_SET);
        return true;
    }
}

fsdir_entry_t * fujiMenu::get_current_menu_entry() 
{
    char tempBuf[MAX_MENU_LINE];
    uint8_t tempType = 0;

    if (fgets(tempBuf, MAX_MENU_LINE, _menu_file)) 
    {
        //if (tempBuf[2] == '|' && asciiToHexval(tempBuf[0]) > 0 && asciiToHexval(tempBuf[1] > 0)) {
        //    _current_menu_entry.type = asciiToHexval(tempBuf[0]);
        //}
        // TODO: parse reset of line
       
        return nullptr;
    }
    else return nullptr;
}
