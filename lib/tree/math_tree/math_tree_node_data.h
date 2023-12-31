#ifndef MATH_TREE_NODE_DATA_H
#define MATH_TREE_NODE_DATA_H


enum MathNodeOperator {

    OPERATOR_ADD,
    OPERATOR_SUB,
    OPERATOR_DIV,
    OPERATOR_MUL,
    OPERATOR_POW,
    OPERATOR_SIN,
    OPERATOR_COS,
    OPERATOR_LN,
    OPERATOR_EQUAL,
    OPERATOR_NOT_EQUAL,
    OPERATOR_LESS,
    OPERATOR_GREATER,
    OPERATOR_OPEN_PARENTHESIS,
    OPERATOR_CLOSE_PARENTHESIS
};

enum LangNodeOperator {

    IF,
    WHILE,
    ELSE,
    ASSIGN,
    END_LINE,
    PRINT,
    CALL_FUNC,
    FUNC_RET,
    NEW_FUNC,
    FUNC,
    READ,
    FUNC_ARG,
    INIT,
    TYPE_INT,
    CLOSE_BRACE,
    OPEN_BRACE,
    OR,
    AND
};

union MathNodeData {

    double mathNodeValue;
    MathNodeOperator mathOperator;
    LangNodeOperator langOperator;
};

enum MathNodeType {

    NUMBER,
    VARIABLE,
    UNARY_OPERATOR,
    BINARY_OPERATOR,
    LANGUAGE_OPERATOR,
    END,
    NODE_TYPE_ERROR,
};

struct MathNode {

    MathNodeType nodeType;
    MathNodeData nodeValue;
};

#endif
