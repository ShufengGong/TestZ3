#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys

import clang.cindex
from clang.cindex import CursorKind, TranslationUnit

code = '''
float max(float x, float y) {
    x += 1;
    return x > y ? x : y;
}
'''


def parse(source):
    unsaved_files = [('<main>.cpp', source)]

    index = clang.cindex.Index.create()
    unit = index.parse('<main>.cpp', unsaved_files=unsaved_files)
    for diag in unit.diagnostics:
        print(diag, file=sys.stderr)
    return unit.cursor


def get_operator(node):
    if node.kind == CursorKind.BINARY_OPERATOR or node.kind == CursorKind.COMPOUND_ASSIGNMENT_OPERATOR:
        left, _ = node.get_children()
        offset = len([_ for _ in left.get_tokens()])
        return [tok for tok in node.get_tokens()][offset].spelling

    if node.kind == CursorKind.UNARY_OPERATOR:
        operand = next(node.get_children())
        tokens = [tok.spelling for tok in node.get_tokens()]
        optokens = [tok.spelling for tok in operand.get_tokens()]
        if tokens[0] == optokens[0]:
            return tokens[1]
        else:
            return tokens[0]

    raise ValueError('Not a unary or binary op: %r', node.kind)


def codegen(node, base_indent=4, indent=0):
    def is_std_ns(node):
        return node.kind == CursorKind.NAMESPACE and node.spelling == 'std'

    k = node.kind  # type: clang.cindex.CursorKind
    if k == CursorKind.RETURN_STMT:
        print("return sentence")

    if k in [CursorKind.BINARY_OPERATOR,
             CursorKind.UNARY_OPERATOR,
             CursorKind.COMPOUND_ASSIGNMENT_OPERATOR]:
        print('operator is: ', get_operator(node))

    tpl = '{indent}{kind}{name}{type_name}'
    if node.spelling:
        name = ' s: %s' % node.spelling
    else:
        name = ''
    if node.type and node.type.spelling:
        type_name = ', t: %s' % node.type.spelling
    else:
        type_name = ''

    print(tpl.format(indent=' ' * indent, kind=k.name, name=name, type_name=type_name))

    # FIXME: skip auto generated decls
    for c in node.get_children():
        codegen(c, base_indent, indent + base_indent)


if __name__ == '__main__':
    ast = parse('./single_dep.cpp')
    codegen(ast)
