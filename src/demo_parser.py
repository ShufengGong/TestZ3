#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys

import clang.cindex
from clang.cindex import CursorKind, TranslationUnit


def filter_the_module(root, filepath):
    children = []
    for child in root.get_children():
        if child.location and child.location.file and \
                child.location.file.name == filepath:
            children.append(child)
    return children


def parse(source):
    index = clang.cindex.Index.create()

    return index.parse(source, args=['-X','c++'])


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


def codegen(node, out_put, is_skip, base_indent=4, indent=0):
    k = node.kind  # type: clang.cindex.CursorKind
    print(k)
    return

    prt_format = '{indent}{kind}{name}{type_name}'
    if node.spelling:
        name = ' s: %s' % node.spelling
    else:
        name = ''
    if node.type and node.type.spelling:
        type_name = ', t: %s' % node.type.spelling
    else:
        type_name = ''

    if k == CursorKind.CXX_METHOD and node.spelling in ['aggregate', 'update', 'generate']:
        is_skip = False
        print('expr ' + node.spelling + '(expr x, expr y){\n')

    # if k == CursorKind.CXX_METHOD and node.spelling in ['aggregate']:
    #     is_skip = False
    #
    if not is_skip:
        # out_put.write('expr ' + node.spelling + '(expr x, expr y){\n')

        # if k in [CursorKind.BINARY_OPERATOR,
        #          CursorKind.UNARY_OPERATOR,
        #          CursorKind.COMPOUND_ASSIGNMENT_OPERATOR]:
        #     print((' ' * indent) + 'operator: ', get_operator(node))
        # print(prt_format.format(indent=' ' * indent, kind=k.name, name=name, type_name=type_name))

        if k == CursorKind.OVERLOADED_DECL_REF and node.spelling == 'atomic_min':
            # out_put.write('\treturn ite(x < y, x, y);\n')
            print('\treturn ite(x < y, x, y);\n')

        if k == CursorKind.OVERLOADED_DECL_REF and node.spelling == 'atomic_max':
            # out_put.write('\treturn ite(x > y, x, y);\n')
            print('\treturn ite(x > y, x, y);\n')

        if k == CursorKind.OVERLOADED_DECL_REF and node.spelling == 'atomic_sum':
            # out_put.write('\treturn x + y;\n')
            print('\treturn x + y;\n')

    # FIXME: skip auto generated decls
    for c in node.get_children():
        codegen(c, out_put, is_skip, base_indent, indent + base_indent)


if __name__ == '__main__':
    filepath = './demo_to_parse.h'
    unit = parse(filepath)

    modules = filter_the_module(unit.cursor, filepath)
    print(modules.__sizeof__())
    to_reflect, to_include = [], []
    # head information
    out_file = open('./demo_parsed.h', 'w')
    out_file.write('#include \"z3++.h\"\n\n')
    out_file.write('using namespace z3;\n\n')

    for module in modules:
        codegen(module, out_file, True)

    out_file.close()
