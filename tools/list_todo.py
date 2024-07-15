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

class plan_data:
    def __init__( self , msg: str , version: str , start_line: int , start_col: int ):
        self.msg = msg
        self.version = version
        self.start_line = start_line
        self.start_col = start_col

todo_regex = re.compile( r'\s*(//|#) \*+\n\s*(//|#) TODO: (.+)\n\s*(//|#) \*+' , re.MULTILINE )
plan_regex = re.compile( r'\s*(//|#) \*+\n\s*(//|#) PLAN: (.+)\n\s*(//|#) VERSION: (.+)\n\s*(//|#) \*+' , re.MULTILINE )

def find_files_to_check( dir: str , exts: list[str] ) -> list[str]:
    files = []
    for ext in exts:
        files.extend( glob.glob( os.path.join( dir , f"**/*{ ext }" ) , recursive = True ) )
    files.sort()
    return files

def check_file_todo( filename: str ) -> list[todo_data]:
    with open( filename , 'r' , encoding = 'utf-8' ) as rFile:
        content = rFile.read()
    todos = []
    matches = todo_regex.finditer( content )
    for match in matches:
        todo_msg = match.group( 3 )
        start_pos = content.find( todo_msg , match.start() )
        start_line = content.count( '\n' , 0 , start_pos ) + 1
        start_col = start_pos - content.rfind( '\n' , 0 , start_pos )
        todo_msg = match.group( 3 )
        todos.append( todo_data( todo_msg , start_line , start_col ) )
    return todos

def check_file_plan( filename: str ) -> list[plan_data]:
    with open( filename , 'r' , encoding = 'utf-8' ) as rFile:
        content = rFile.read()
    plans = []
    matches = plan_regex.finditer( content )
    for match in matches:
        plan_msg = match.group( 3 )
        start_pos = content.find( plan_msg , match.start() )
        start_line = content.count( '\n' , 0 , start_pos ) + 1
        start_col = start_pos - content.rfind( '\n' , 0 , start_pos )
        plan_version = match.group( 5 )
        plans.append( plan_data( plan_msg , plan_version , start_line , start_col ) )
    return plans

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
    global_plans_count = 0

    for dir in check_target_dirs:
        print( f"Entering directory: { dir }" )
        files = find_files_to_check( os.path.join( project_root_dir , dir ) , check_target_exts )
        for file in files:
            todos = check_file_todo( file )
            todos_count = len( todos )
            if todos_count != 0:
                global_todos_count += todos_count
                print( f" - Found { todos_count } TODO in file \"{ os.path.relpath( file , project_root_dir ) }\":" )
                for todo in todos:
                    print( f"   * \033[0;31m{ todo.msg }\033[0m (Ln { todo.start_line }, Col { todo.start_col })" )
            plans = check_file_plan( file )
            plans_count = len( plans )
            if plans_count != 0:
                global_plans_count += plans_count
                print( f" - Found { plans_count } PLAN in file \"{ os.path.relpath( file , project_root_dir ) }\":" )
                for plan in plans:
                    print( f"   * \033[0;32m{ plan.msg }\033[0m (Ln { plan.start_line }, Col { plan.start_col })" )
        print( f"Leaving directory: { dir }" )

    print( f"Work done, { global_todos_count } TODO and { global_plans_count } PLAN found." )

    if len( sys.argv ) > 1 and sys.argv[1] == "restrict" and global_todos_count != 0:
        # restrict mode, exit with 1 where there's still sth to do remaining
        exit( 1 )

    exit( 0 )

if __name__ == "__main__":
    main()
