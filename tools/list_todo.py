import configparser
import glob
import os
import re
import sys

class todo_data:
    def __init__( self , msg: str , start_line: int , start_col: int ):
        self.msg = msg
        self.start_line = start_line
        self.start_col = start_col

todo_regex = re.compile( r'\s*(//|#) \*+\n\s*(//|#) TODO: (.+)\n\s*(//|#) \*+' , re.MULTILINE )

def find_files_to_check( dir: str , exts: list[str] ) -> list[str]:
    files = []
    for ext in exts:
        files.extend( glob.glob( os.path.join( dir , f"**/*{ ext }" ) , recursive = True ) )
    files.sort()
    return files

def check_file( filename: str ) -> list[todo_data]:
    with open( filename , 'r' , encoding = 'utf-8' ) as rFile:
        content = rFile.read()
    todos = []
    matches = todo_regex.finditer( content )
    for match in matches:
        start_pos = match.start()
        start_line = content.count( '\n' , 0 , start_pos ) + 1
        start_col = start_pos - content.rfind( '\n' , 0 , start_pos )
        todo_msg = match.group( 3 )
        todos.append( todo_data( todo_msg , start_line , start_col ) )
    return todos    

def main():
    cp = configparser.ConfigParser()
    cp.read( "./data/list_todo.cfg" , encoding = 'utf-8' )
    check_target_dirs_str = cp.get( "check_target" , "dirs" )
    if ',' in check_target_dirs_str:
        check_target_dirs = check_target_dirs_str.split( ',' )
    else:
        check_target_dirs = [ check_target_dirs_str ]
    check_target_exts = cp.get( "check_target" , "exts" ).split( '/' )

    project_root_dir = os.path.dirname( os.path.dirname( os.path.abspath( __file__ ) ) )
    global_todos_count = 0

    for dir in check_target_dirs:
        print( f"Entering directory: { dir }" )
        files = find_files_to_check( os.path.join( project_root_dir , dir ) , check_target_exts )
        for file in files:
            todos = check_file( file )
            todos_count = len( todos )
            if todos_count != 0:
                global_todos_count += todos_count
                print( f" - Found { todos_count } TODO in file \"{ os.path.relpath( file , project_root_dir ) }\":" )
                for todo in todos:
                    print( f"   * \033[0;31m{ todo.msg }\033[0m (Ln { todo.start_line }, Col { todo.start_col })" )
        print( f"Leaving directory: { dir }" )

    print( f"Work done, { global_todos_count } TODO found." )

    if len( sys.argv ) > 1 and sys.argv[1] == "restrict" and global_todos_count != 0:
        # restrict mode, exit with 1 where there's still sth to do remaining
        exit( 1 )

    exit( 0 )

if __name__ == "__main__":
    main()
