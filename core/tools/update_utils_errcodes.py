import configparser

def update_errcodes_enum_declaration( section: str , errcodes: list[tuple[str,str]] ) -> str:
    out = ""
    out += f"            typedef enum { section } : int {{\n"
    for code in errcodes:
        out += f"                { code[0].upper() } = { code[1] },\n"
    out += f"            }} { section };\n\n"
    return out

def update_errcodes_get_name_function_declaration( section: str ) -> str:
    return f"            static std::string get_name( { section } __ec_code );\n"

def update_errcodes_name_map_declaration( section: str ) -> str:
    return f"            static const std::unordered_map<{ section },std::string> _um_{ section }_namemap;\n"

def update_errcodes_get_name_function_definition( section: str ) -> str:
    out = ""
    out += f"std::string utils::errcodes::get_name( utils::errcodes::{ section } __ec_code ){{\n"
    out += f"    try {{\n"
    out += f"        return _um_{ section }_namemap.at( __ec_code );\n"
    out += f"    }}\n"
    out += f"    catch ( std::out_of_range )\n"
    out += f"    {{\n"
    out += f"        return \"\";\n"
    out += f"    }}\n"
    out += f"}}\n\n"
    return out

def update_errcodes_name_map_definition( section: str , errcodes: list[tuple[str,str]] ) -> str:
    out = ""
    out += f"const std::unordered_map<utils::errcodes::{ section },std::string> utils::errcodes::_um_{ section }_namemap = {{\n"
    for code in errcodes:
        out += f"    {{ { code[0].upper() } , \"{ code[0].upper() }\" }},\n"
    out += f"}};\n\n"
    return out

def main():
    cp = configparser.ConfigParser()
    cp.read( "./data/update_utils_errcodes.cfg" , encoding = 'utf-8' )
    gen_target_header = cp.get( "gen_target" , "header" )
    gen_target_source = cp.get( "gen_target" , "source" )
    gen_source_header = cp.get( "gen_source" , "header" )
    gen_source_source = cp.get( "gen_source" , "source" )

    i_errcodes_enum_declaration = ""
    i_errcodes_get_name_function_declaration = ""
    i_errcodes_name_map_declaration = ""
    i_errcodes_get_name_function_definition = ""
    i_errcodes_name_map_definition = ""

    print( "Updating:" )
    print( f" - { gen_source_header } -> { gen_target_header }" )
    print( f" - { gen_source_source } -> { gen_target_source }" )

    section_list = []
    print( "Found following sections:" )
    for section in cp:
        if section != "gen_target" and section != "gen_source" and section != "DEFAULT":
            section_list.append( section )
            print( f" - { section }" )

    for section in section_list:
        print( f"Entering section: { section }..." )
        errcodes = cp.items( section )
        i_errcodes_enum_declaration += update_errcodes_enum_declaration( section , errcodes )
        i_errcodes_get_name_function_declaration += update_errcodes_get_name_function_declaration( section )
        i_errcodes_name_map_declaration += update_errcodes_name_map_declaration( section )
        i_errcodes_get_name_function_definition += update_errcodes_get_name_function_definition( section )
        i_errcodes_name_map_definition += update_errcodes_name_map_definition( section , errcodes )
        print( f"Leaving section: { section }..." )

    with open( gen_source_header , 'r' ) as rFile:
        header_code = rFile.read()

    with open( gen_source_source , 'r' ) as rFile:
        source_code = rFile.read()

    header_code = header_code.replace( "@INSERT_ERRCODES_ENUM_DECLARATION@" , i_errcodes_enum_declaration[:-1] )
    header_code = header_code.replace( "@INSERT_ERRCODES_GET_NAME_FUNCTION_DECLARATION@" , i_errcodes_get_name_function_declaration[:-1] )
    header_code = header_code.replace( "@INSERT_ERRCODES_NAME_MAP_DECLARATION@" , i_errcodes_name_map_declaration[:-1] )
    source_code = source_code.replace( "@INSERT_ERRCODES_GET_NAME_FUNCTION_DEFINITION@" , i_errcodes_get_name_function_definition[:-1] )
    source_code = source_code.replace( "@INSERT_ERRCODES_NAME_MAP_DEFINITION@" , i_errcodes_name_map_definition[:-1] )    

    with open( gen_target_header , 'w' ) as wFile:
        wFile.write( header_code )

    with open( gen_target_source , 'w' ) as wFile:
        wFile.write( source_code )

if __name__ == "__main__":
    main()
