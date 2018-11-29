#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "stdafx.h"

namespace payment {

enum CMDS
{
    MD_ERROR=-1,
    MD_NULL,
    MD_EXIT,

    MD_COUNT,
};

class Commands
{

protected:

    Commands() {};
    virtual ~Commands() {};

public:

    static CMDS cmd_service()
    {

        cmd_code = MD_NULL;

        int tmp = 0;
        while ((tmp = getchar()) != -1){

            switch (static_cast<char>(tmp)) {

                case '\n':
                {
                    if (cmds == "exit")
                    {
                        cmd_code =  MD_EXIT;
                    }

                    //TODO: add new commanders
                    //else if () {}

                    cmds.clear();
                    return cmd_code;
                }

                default:
                {
                    cmds.push_back(static_cast<char>(tmp));
                    break;
                }

            }

        }

        return MD_ERROR;

    }

private:

    static std::string cmds;
    static CMDS cmd_code;

};
std::string Commands::cmds = "";
CMDS Commands::cmd_code = MD_NULL;

}

#endif
