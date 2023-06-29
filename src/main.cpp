#include <cstddef>
extern "C" {
#include <clang-c/CXString.h>
#include <clang-c/Index.h>
}
#include <iostream>

int main(int argc, char **argv) {

    CXIndex index = clang_createIndex(0, 0);

    const char *filename = "main.c";
    CXTranslationUnit translationUnit = clang_parseTranslationUnit(
        index, filename, NULL, 0, NULL, 0, CXTranslationUnit_None);
    CXFile file = clang_getFile(translationUnit, filename);
    unsigned int line, colomn;
    while (std::cin >> line >> colomn) {
        CXSourceLocation source =
            clang_getLocation(translationUnit, file, line, colomn);
        CXCursor cursor = clang_getCursor(translationUnit, source);
        if (clang_getCursorKind(cursor) == CXCursor_DeclRefExpr) {
            auto type = clang_getCursorType(cursor);
            auto stype = clang_getTypeSpelling(type);
            auto name = clang_getCursorSpelling(cursor);
            std::cout << clang_getCString(name) << " : "
                      << clang_getCString(stype) << "\n";
            clang_disposeString(stype);
            clang_disposeString(name);
        }
    }
    clang_disposeTranslationUnit(translationUnit);
    return 0;
}