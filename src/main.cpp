#include <cstddef>
extern "C" {
#include <clang-c/CXString.h>
#include <clang-c/Index.h>
}
#include <iostream>
#include <string>

std::string to_string(CXString s){
    std::string retn{clang_getCString(s)};
    clang_disposeString(s);
    return retn;
}

int main(int argc, char **argv) {

    CXIndex index = clang_createIndex(0, 0);
    const char *filename = "main.cpp";
    // if(argc >= 1){
    //     filename = argv[1];
    // }
    // CXCursor_FirstDecl
    // CXTranslationUnit aUnit = clang_parseTranslationUnit(
    //     index, "a.c", NULL, 0, NULL, 0, CXTranslationUnit_None);
    CXTranslationUnit translationUnit = clang_parseTranslationUnit(
        index, filename, NULL, 0, NULL, 0, CXTranslationUnit_None);
    CXFile file = clang_getFile(translationUnit, filename);
    unsigned int line, colomn;
    while (std::cin >> line >> colomn) {
        CXSourceLocation source =
            clang_getLocation(translationUnit, file, line, colomn);
        CXCursor cursor = clang_getCursor(translationUnit, source);
        if (clang_getCursorKind(cursor) == CXCursor_DeclRefExpr
            || clang_isDeclaration(clang_getCursorKind(cursor))) {
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

    // CXCursor cursor = clang_getTranslationUnitCursor(translationUnit);
    // auto visitor = [](CXCursor cursor, CXCursor /* parent */,
    //                   CXClientData /* clientData */) -> CXChildVisitResult {
    //     CXSourceLocation location = clang_getCursorLocation(cursor);
    //     if (clang_Location_isFromMainFile(location) == 0)
    //         return CXChildVisit_Continue;
    //     auto cursor_kind = clang_getCursorKind(cursor);
    //     if(cursor_kind == CXCursor_DeclRefExpr){
    //         cursor = clang_getCursorDefinition(cursor);
    //         cursor_kind = clang_getCursorKind(cursor);
    //     }
    //     if (clang_isDeclaration(cursor_kind)) {
    //         // auto name = to_string(clang_getCursorSpelling(cursor));
    //         // if (name == "a") {
    //         //     unsigned int line, colomn;
    //         //     clang_getExpansionLocation(location, NULL, &line, &colomn,
    //         //                                NULL);
    //         //     std::cout << "a : " << line << ":" << colomn << "\n";
    //         // }
    //         auto USR = to_string(clang_getCursorUSR(cursor));
    //         unsigned int line, colomn;
    //         CXFile inner_file;
    //         clang_getExpansionLocation(location, &inner_file, &line, &colomn,
    //                                         NULL);
    //         std::cout << USR << ":\n"
    //                   << to_string(clang_getFileName(inner_file)) << ":"
    //                   << line << ":" << colomn << "\n";
    //         if(cursor_kind == CXCursor_FunctionDecl){
    //             return CXChildVisit_Recurse;
    //         }
    //         return CXChildVisit_Continue;
    //     }
    //     return CXChildVisit_Recurse;
    // };
    // unsigned int treeLevel = 0;
    // clang_visitChildren(cursor, visitor, &treeLevel);
    clang_disposeTranslationUnit(translationUnit);
    clang_disposeIndex(index);
    return 0;
}