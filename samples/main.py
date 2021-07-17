import clang.cindex

def find_for(node: clang.cindex.Cursor):
    if node.kind == clang.cindex.CursorKind.FOR_STMT:
        print(node.location.line)
    # Recurse for children of this node
    for c in node.get_children():
        find_for(c)

index = clang.cindex.Index.create()
tu = index.parse("for_loops.c", ["-x", "c", "-std=c11"])

find_for(tu.cursor)
