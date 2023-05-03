
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

    char tempBuf[MAX_MENU_LINE_LEN];

    // find the offset of the new menu position.
    _current_pos = 0;
    _current_offset = 0;
    int linelen = 0;

    while (_current_pos < newPos && fgets(tempBuf, MAX_MENU_LINE_LEN, _menu_file)) 
    {

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

uint8_t fujiMenu::decode_menutype(const char * buf)
{
    return (uint8_t)strtol(buf, nullptr, 16);
}

fsdir_entry_t * fujiMenu::get_next_menu_entry() 
{
    char tempBuf[MAX_MENU_LINE_LEN];
    _type = 0;
    _name_len = 0;
    _resource_len = 0;
    memset(_name, 0, MAX_MENU_NAME_LEN);
    memset(_resource, 0, MAX_MENU_RESOURCE_LEN);
    uint8_t nameStart = 0;
    uint8_t resourceStart = 0;

    // if we have an offset, skip to it. 
    if (_current_offset > 0) 
    {
        if (fseek(_menu_file, _current_offset, 0) != 0)
        {
            Debug_printf("fujiMenu::get_next_menu_entry, cannot seek to current offset.");
            return nullptr;
        }
    }

    if (fgets(tempBuf, MAX_MENU_LINE_LEN, _menu_file)) 
    {
        _current_pos += 1;
        _current_offset += strlen(tempBuf);

        _direntry.isDir = false;
        _direntry.size = 0;
        _direntry.modified_time = 0;

        // menu format: [<type>|]<name>[|<resource>]
        //
        //       <type> : Eight bit int in ascii hex ("0F") format.
        //       <name> : Name of the resource being pointed to by this 
        //                menu. A short, friendly string that will be 
        //                displayed in the TNFS browser. Must not contain 
        //                the pipe ('|') symbol. If no resource is 
        //                specified, this is also the resource.
        //   <resource> : A string resource, format dependent on <type>

        int len = strlen(tempBuf);
        if (len>0 && tempBuf[len-1] == '\n') {
            tempBuf[len-1] = 0;
            len--;
        }

        if (len > 2 && tempBuf[2] == '|')
        {
            nameStart = 3;
            _name_len = len+1;
            _type = decode_menutype(tempBuf);
            printf("menu type %i\n", _type);

            char * pt = strchr(&tempBuf[nameStart], '|');
            if (pt)
            {
                _name_len = (pt - (tempBuf + nameStart));
                resourceStart = 4 + _name_len;
                _resource_len = len - resourceStart;
            }
            else
            {
                resourceStart = nameStart;
                _name_len = len - 3;
                _resource_len = _name_len;
            }
        }
        else
        {
            _name_len = len;
            _resource_len = _name_len;
        }

        if (_type == 1) _direntry.isDir = true;

        strncpy(_name, &tempBuf[nameStart], _name_len);
        strncpy(_resource, &tempBuf[resourceStart], _resource_len);

        strlcpy(_direntry.filename, _name, _name_len+1);
        
        return &_direntry;
    }
    else return nullptr;
}
