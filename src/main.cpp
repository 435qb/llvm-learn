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
            auto visit = [](void *, CXCursor cursor,
                            CXSourceRange range) -> enum CXVisitorResult {
                auto start = clang_getRangeStart(range);
                auto end = clang_getRangeEnd(range);
                unsigned int start_line, end_line, start_colomn, end_colomn;
                clang_getExpansionLocation(start, NULL, &start_line,
                                           &start_colomn, NULL);
                clang_getExpansionLocation(end, NULL, &end_line,
                                           &end_colomn, NULL);
                auto name = clang_getCursorSpelling(cursor);
                std::cout << clang_getCString(name) << ": "
                          << start_line << ":" << start_colomn << "-"
                          << end_line   << ":" << end_colomn << "\n";
                return CXVisit_Continue;
            };
            clang_findReferencesInFile(cursor, file, {NULL, visit});
            // auto type = clang_getCursorType(cursor);
            // auto stype = clang_getTypeSpelling(type);
            // auto name = clang_getCursorSpelling(cursor);
            // std::cout << clang_getCString(name) << " : "
            //           << clang_getCString(stype) << "\n";
            // clang_disposeString(stype);
            // clang_disposeString(name);
        }
    }
    clang_disposeTranslationUnit(translationUnit);
    return 0;
}