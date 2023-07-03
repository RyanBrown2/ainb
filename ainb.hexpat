#pragma endian little

struct FileHeader {
    char type[4] @ 0x0;
    u32 entry_count @ 0x0c;
    u32 command_count @0x10;
    
    u32 *after_last_command : u32 @0x20; // will probably rename
    char *string_list_start : u32 @0x24; // beginning of string list
    
    //will probably rename
    u8 *parameter_table : u32 @0x2c; // beginning of parameter table section
    
    u8 *command_parameters : u32 @0x34; // beginning of the command parameter section
    
    u8 *parameter_section : u32 @0x40; // beginning of overall parameter section
    
    u8 *function_start : u32 @0x44; // beginning of the function section
    u8 *function_end : u32 @0x5c; // end of function section
    
    u32 string_list_second_offset @0x60; // offset for the second string in string list
    
    u32 type_indicator @0x64;
    
};

struct EntryCommand {
    u32 string_list_offset; // string list offset for command name
    u128 guid;
    u32 execution_command_index; // index of command to execute
};

struct TableParamEntry {

};

struct CommandBody {
    
};

struct ExecutionCommand {
    u16 unknown1;
    u32 index;
    u16 unknown2;
    u32 string_list_offset; // string list offset for command name
    u32 uid;
    padding[4];
    u32 command_body_pointer; // pointer to start of body
    padding[0x14];
    u128 guid;
};



FileHeader fileHeader @ 0x4;

EntryCommand entryCommands[fileHeader.entry_count] @0x74;

u32 execution_start_address = 0x74 + (0x18*fileHeader.entry_count);
ExecutionCommand executionCommand[fileHeader.command_count] @execution_start_address;