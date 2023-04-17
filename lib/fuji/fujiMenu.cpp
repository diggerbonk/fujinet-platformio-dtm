
#include "fujiMenu.h"

#include <cstring>
#include <stdlib.h>
#include <stdio.h>

#include "../../include/debug.h"

#include "fnFsSD.h"
#include "fnFsTNFS.h"

#include "utils.h"

bool fujiMenu::init(const char *path, fujiHost * fh)
{
    ssize_t read;
    const int bufferlen = 256;
    char buf[256];
    size_t len = 0;

    strlcpy(_path, path, sizeof(_path)); 

    // TODO: (TRENT) Open & scan menu file and mark 

    if (fh == nullptr)
        return false;

    Debug_printf("fujiMenu opening file path \"%s\"\n", path);

    _menu_file = fh->file_open(path, nullptr, 0, "r+");

    // walk through file, get each newline position
 
    _num_lines = 1;
    _line_offsets[0] = 0;
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

int8_t fujiMenu::asciiToHexval(char a)
{
    if (a > 47 && a < 58) return a - 48;
    else if (a > 47 && a < 71) return a - 55;
    else if (a > 96 && a < 103) return a - 87;
    else return -1;
} 

menu_entry_t * fujiMenu::get_current_menu_entry() 
{
    char tempBuf[MAX_MENU_LINE];
    uint8_t tempType = 0;

    if (fgets(tempBuf, MAX_MENU_LINE, _menu_file)) 
    {
        if (tempBuf[2] == '|' && asciiToHexval(tempBuf[0]) > 0 && asciiToHexval(tempBuf[1] > 0)) {
            _current_menu_entry.type = asciiToHexval(tempBuf[0]);
        }
        // TODO: parse reset of line
       
        return & _current_menu_entry;
    }
    else return nullptr;
}
