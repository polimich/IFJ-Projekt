//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace decoder.c: xpoliv07, xmedri01        //
//    Datum: 06. 12. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __BUILT_IN_H__
#define __BUILT_IN_H__

#define GENERATOR_ADD "LABEL $ADD\
\nCREATEFRAME\
\nDEFVAR TF@op1\
\nDEFVAR TF@op2\
\nDEFVAR TF@op1$type\
\nDEFVAR TF@op2$type\
\nDEFVAR TF@op1$tmp\
\nDEFVAR TF@op2$tmp\
\nPOPS TF@op1\
\nPOPS TF@op2\
\nTYPE TF@op1$type TF@op1\
\nTYPE TF@op2$type TF@op2\
\nJUMPIFNEQ $ADD_NOT_EQL_TYPES TF@op1$type TF@op2$type\
\nLABEL $ADD_OK\
\nPUSHS TF@op1\
\nPUSHS TF@op2\
\nADDS\
\nJUMP $ADD_END\
\nLABEL $ADD_NOT_EQL_TYPES\
\nJUMPIFEQ $ADD_OP1_FLOAT TF@op1$type string@float\
\nJUMPIFEQ $ADD_OP1_INT TF@op1$type string@int\
\nJUMP $ADD_ERROR\
\nLABEL $ADD_OP1_FLOAT\
\nJUMPIFEQ $ADD_CONVERT_OP2_2_FLOAT TF@op2$type string@int\
\nJUMP $ADD_ERROR\
\nLABEL $ADD_OP1_INT\
\nJUMPIFEQ $ADD_CONVERT_OP2_2_INT TF@op2$type string@float\
\nJUMP $ADD_ERROR\
\nLABEL $ADD_CONVERT_OP2_2_FLOAT\
\nINT2FLOAT TF@op2$tmp TF@op2\
\nMOVE TF@op2 TF@op2$tmp\
\nJUMP $ADD_OK\
\nLABEL $ADD_CONVERT_OP2_2_INT\
\nINT2FLOAT TF@op1$tmp TF@op1\
\nMOVE TF@op1 TF@op1$tmp\
\nJUMP $ADD_OK\
\nLABEL $ADD_ERROR\
\nEXIT int@4\
\nLABEL $ADD_END\
\nRETURN\n"

#define GENERATOR_SUB "LABEL $SUB\
\nCREATEFRAME\
\nDEFVAR TF@op1\
\nDEFVAR TF@op2\
\nDEFVAR TF@op1$type\
\nDEFVAR TF@op2$type\
\nDEFVAR TF@op1$tmp\
\nDEFVAR TF@op2$tmp\
\nPOPS TF@op1\
\nPOPS TF@op2\
\nTYPE TF@op1$type TF@op1\
\nTYPE TF@op2$type TF@op2\
\nJUMPIFNEQ $SUB_NOT_EQL_TYPES TF@op1$type TF@op2$type\
\nLABEL $SUB_OK\
\nPUSHS TF@op1\
\nPUSHS TF@op2\
\nSUBS\
\nJUMP $SUB_END\
\nLABEL $SUB_NOT_EQL_TYPES\
\nJUMPIFEQ $SUB_OP1_FLOAT TF@op1$type string@float\
\nJUMPIFEQ $SUB_OP1_INT TF@op1$type string@int\
\nJUMP $SUB_ERROR\
\nLABEL $SUB_OP1_FLOAT\
\nJUMPIFEQ $SUB_CONVERT_OP2_2_FLOAT TF@op2$type string@int\
\nJUMP $SUB_ERROR\
\nLABEL $SUB_OP1_INT\
\nJUMPIFEQ $SUB_CONVERT_OP2_2_INT TF@op2$type string@float\
\nJUMP $SUB_ERROR\
\nLABEL $SUB_CONVERT_OP2_2_FLOAT\
\nINT2FLOAT TF@op2$tmp TF@op2\
\nMOVE TF@op2 TF@op2$tmp\
\nJUMP $SUB_OK\
\nLABEL $SUB_CONVERT_OP2_2_INT\
\nINT2FLOAT TF@op1$tmp TF@op1\
\nMOVE TF@op1 TF@op1$tmp\
\nJUMP $SUB_OK\
\nLABEL $SUB_ERROR\
\nEXIT int@4\
\nLABEL $SUB_END\
\nRETURN\n"

#define GENERATOR_MUL "LABEL $MUL\
\nCREATEFRAME\
\nDEFVAR TF@op1\
\nDEFVAR TF@op2\
\nDEFVAR TF@op1$type\
\nDEFVAR TF@op2$type\
\nDEFVAR TF@op1$tmp\
\nDEFVAR TF@op2$tmp\
\nPOPS TF@op1\
\nPOPS TF@op2\
\nTYPE TF@op1$type TF@op1\
\nTYPE TF@op2$type TF@op2\
\nJUMPIFNEQ $MUL_NOT_EQL_TYPES TF@op1$type TF@op2$type\
\nLABEL $MUL_OK\
\nPUSHS TF@op1\
\nPUSHS TF@op2\
\nMULS\
\nJUMP $MUL_END\
\nLABEL $MUL_NOT_EQL_TYPES\
\nJUMPIFEQ $MUL_OP1_FLOAT TF@op1$type string@float\
\nJUMPIFEQ $MUL_OP1_INT TF@op1$type string@int\
\nJUMP $MUL_ERROR\
\nLABEL $MUL_OP1_FLOAT\
\nJUMPIFEQ $MUL_CONVERT_OP2_2_FLOAT TF@op2$type string@int\
\nJUMP $MUL_ERROR\
\nLABEL $MUL_OP1_INT\
\nJUMPIFEQ $MUL_CONVERT_OP2_2_INT TF@op2$type string@float\
\nJUMP $MUL_ERROR\
\nLABEL $MUL_CONVERT_OP2_2_FLOAT\
\nINT2FLOAT TF@op2$tmp TF@op2\
\nMOVE TF@op2 TF@op2$tmp\
\nJUMP $MUL_OK\
\nLABEL $MUL_CONVERT_OP2_2_INT\
\nINT2FLOAT TF@op1$tmp TF@op1\
\nMOVE TF@op1 TF@op1$tmp\
\nJUMP $MUL_OK\
\nLABEL $MUL_ERROR\
\nEXIT int@4\
\nLABEL $MUL_END\
\nRETURN\n"

#define GENERATOR_DIV "LABEL $DIV\
\nCREATEFRAME\
\nDEFVAR TF@op1\
\nDEFVAR TF@op2\
\nDEFVAR TF@op1$type\
\nDEFVAR TF@op2$type\
\nDEFVAR TF@op1$tmp\
\nDEFVAR TF@op2$tmp\
\nPOPS TF@op1\
\nPOPS TF@op2\
\nTYPE TF@op1$type TF@op1\
\nTYPE TF@op2$type TF@op2\
\nJUMPIFNEQ $DIV_NOT_EQL_TYPES TF@op1$type TF@op2$type\
\nLABEL $DIV_OK\
\nPUSHS TF@op1\
\nPUSHS TF@op2\
\nDIVS\
\nJUMP $DIV_END\
\nLABEL $DIV_NOT_EQL_TYPES\
\nJUMPIFEQ $DIV_OP1_FLOAT TF@op1$type string@float\
\nJUMPIFEQ $DIV_OP1_INT TF@op1$type string@int\
\nJUMP $DIV_ERROR\
\nLABEL $DIV_OP1_FLOAT\
\nJUMPIFEQ $DIV_CONVERT_OP2_2_FLOAT TF@op2$type string@int\
\nJUMP $DIV_ERROR\
\nLABEL $DIV_OP1_INT\
\nJUMPIFEQ $DIV_CONVERT_OP2_2_INT TF@op2$type string@float\
\nJUMP $DIV_ERROR\
\nLABEL $DIV_CONVERT_OP2_2_FLOAT\
\nINT2FLOAT TF@op2$tmp TF@op2\
\nMOVE TF@op2 TF@op2$tmp\
\nJUMP $DIV_OK\
\nLABEL $DIV_CONVERT_OP2_2_INT\
\nINT2FLOAT TF@op1$tmp TF@op1\
\nMOVE TF@op1 TF@op1$tmp\
\nJUMP $DIV_OK\
\nLABEL $DIV_ERROR\
\nEXIT int@4\
\nLABEL $DIV_END\
\nRETURN\n"

#define GENERATOR_TO_GOOD_TYPE "LABEL $TO_GOOD_TYPE\
\nPUSHFRAME\
\nDEFVAR LF@$VAL_OP1\
\nDEFVAR LF@$VAL_OP2\
\nDEFVAR LF@$CONVERT_OP2\
\nPOPS LF@$VAL_OP2\
\nPOPS LF@$VAL_OP1\
\nDEFVAR LF@$OP1_TYPE\
\nDEFVAR LF@$OP2_TYPE\
\nTYPE LF@$OP1_TYPE LF@$VAL_OP1\
\nTYPE LF@$OP2_TYPE LF@$VAL_OP2\
\nPUSHS LF@$VAL_OP2                                        #OP2 na zasobnik aby to pak nemusel delat pod kazdym labelem\
\nJUMPIFEQ $SAME_TYPES LF@$OP1_TYPE LF@$OP2_TYPE           #Když jsou typy stejné\
\nJUMPIFEQ $OP1_IS_INT LF@$OP1_TYPE string@int                                                \
\nJUMPIFEQ $OP1_IS_FLOAT LF@$OP1_TYPE string@float\
\nJUMPIFEQ $OP1_IS_STR LF@$OP1_TYPE string@string\
\nJUMPIFEQ $OP1_IS_BOOL LF@$OP1_TYPE string@bool                                                \
\nLABEL $OP1_IS_INT                               \
\n    CALL intval               \
\nLABEL $OP1_IS_FLOAT\
\n    CALL floatval \
\nLABEL $OP1_IS_STR\
\n    CALL strval    \
\nLABEL $OP1_IS_BOOL\
\n    CALL boolval\
\nLABEL $SAME_TYPES\
\n    POPFRAME\
\n    RETURN\
\n###################################################################\
\n#Vraci hodnotu prevedenou do INT\
\nLABEL $intval\
\nPUSHFRAME\
\nDEFVAR LF@$OP\
\nDEFVAR LF@$TYPE_OP\
\nPOPS LF@$OP                                 #Naplni OP\
\nTYPE LF@$TYPE_OP LF@$OP\
\nJUMPIFEQ $INTVAL_END LF@$TYPE_OP string@int              #INT se na INT nemeni\
\nJUMPIFEQ $INTVAL_OP_IS_FLOAT LF@$TYPE_OP string@float\
\nJUMPIFEQ $INTVAL_OP_IS_STR LF@$TYPE_OP string@string\
\nJUMPIFEQ $INTVAL_OP_IS_BOOL LF@$TYPE_OP string@bool\
\n\
\nLABEL $INTVAL_OP_IS_FLOAT\
\n    INT2FLOAT LF@$OP LF@$OP\
\n    JUMP $INTVAL_END\
\n\
\nLABEL $INTVAL_OP_IS_STR\
\n    PUSHS LF@$OP\
\n    CALL STR_TO_INT                         #Toto musiš udělat\
\nLABEL $INTVAL_OP_IS_BOOL\
\n    JUMPIFEQ $INTVAL_BOOL_IS_ZERO LF@$OP bool@false\
\n    PUSHS int@0                             # false == 0\
\n    POPFRAME\
\n    RETURN\
\n    LABEL $INTVAL_BOOL_IS_ZERO\
\n    PUSHS int@1\
\n    POPFRAME                             # true == 1\
\n    RETURN\
\nLABEL $INTVAL_END\
\n    PUSHS LF@$OP\
\n    POPFRAME\
\n    RETURN\
\n\
\n####################################################################\
\n#Vraci hodnotu prevedenou do FLOAT\
\nLABEL $floatval\
\nPUSHFRAME\
\nDEFVAR LF@$OP\
\nDEFVAR LF@$TYPE_OP\
\nPOPS LF@$OP                                 #Naplni OP\
\nTYPE LF@$TYPE_OP LF@$OP\
\nJUMPIFEQ $FLOATVAL_END LF@$TYPE_OP string@int\
\nJUMPIFEQ $FLOATVAL_OP_IS_INT  LF@$TYPE_OP string@int\
\nJUMPIFEQ $FLOATVAL_OP_IS_STR  LF@$TYPE_OP string@string\
\nJUMPIFEQ $FLOATVAL_OP_IS_BOOL LF@$TYPE_OP string@bool\
\n\
\nLABEL $FLOATVAL_OP_IS_INT\
\n    FLOAT2INT LF@$OP LF@$OP\
\n    JUMP $FLOATVAL_END\
\n\
\nLABEL $FLOATVAL_OP_IS_STR\
\n    PUSHS LF@$OP\
\n    CALL STR_TO_FLOAT                         #Toto musiš udělat\
\n\
\nLABEL $FLOATVAL_OP_IS_BOOL\
\n    JUMPIFEQ $FLOATVAL_BOOL_ISN_ZERO LF@$OP bool@false      #OP == FALSE\
\n    PUSHS float@0x1.0p+0\
\n    POPFRAME                     \
\n    RETURN\
\n    LABEL $FLOATVAL_BOOL_IS_ZERO                            #OP != FALSE\
\n    PUSHS float@0x0.0p+0\
\n    POPFRAME\
\n    RETURN\
\nLABEL $FLOATVAL_END\
\n    PUSHS LF@$OP\
\n    POPFRAME\
\n    RETURN\
\n\
\n################################################################\
\n#Vraci hodnotu prevedenou do STR\
\nLABEL $strval\
\nPUSHFRAME\
\nDEFVAR LF@$OP\
\nDEFVAR LF@$TYPE_OP\
\nPOPS LF@$OP                                 #Naplni OP\
\nTYPE LF@$TYPE_OP LF@$OP\
\nJUMPIFEQ $STRVAL_END LF@$TYPE_OP string@string                  # str to str\
\nJUMPIFEQ $STRVAL_OP_IS_INT  LF@$TYPE_OP string@int\
\nJUMPIFEQ $STRVAL_OP_IS_FLOAT  LF@$TYPE_OP string@float\
\nJUMPIFEQ $STRVAL_OP_IS_BOOL LF@$TYPE_OP string@bool\
\n\
\nLABEL $STRVAL_OP_IS_INT\
\n    CALL INT_TO_STR\
\nLABEL $STRVAL_OP_IS_STR\
\n\
\nLABEL $STRVAL_OP_IS_BOOL\
\n    JUMPIFEQ $STRVAL_BOOL_ISN_ZERO LF@$OP bool@false \
\n    PUSHS string@1\
\n    POPFRAME\
\n    RETURN\
\n    LABEL $STRVAL_BOOL_IS_ZERO                            #OP == FALSE\
\n    PUSHS string@0  \
\n    POPFRAME\
\nRETURN\
\n#\
\nLABEL $STRVAL_END\
\n    PUSHS LF@$OP\
\n    POPFRAME\
\n    RETURN\
\n\
\n##########################################################\
\n#Vraci hodnotu prevedenou do BOOL\
\nLABEL $boolval\
\nPUSHFRAME\
\nDEFVAR LF@$OP\
\nDEFVAR LF@$TYPE_OP\
\nPOPS LF@$OP                                 #Naplni OP\
\nTYPE LF@$TYPE_OP LF@$OP\
\n#\
\nJUMPIFEQ $BOOLVAL_END LF@$TYPE_OP string@bool                 # str to str\
\nJUMPIFEQ $BOOLVAL_OP_IS_INT  LF@$TYPE_OP string@int\
\nJUMPIFEQ $BOOLVAL_OP_IS_FLOAT  LF@$TYPE_OP string@float\
\nJUMPIFEQ $BOOLVAL_OP_IS_STR LF@$TYPE_OP string@string\
\n#\
\nLABEL $BOOLVAL_OP_IS_INT\
\n    JUMPIFEQ $INT_OP_IS_ZERO LF@$OP int@0\
\n    PUSHS bool@true\
\n    RETURN\
\n    LABEL $INT_OP_IS_ZERO\
\n    PUSHS bool@false\
\n    POPFRAME\
\n    RETURN\
\n\
\nLABEL $BOOLVAL_OP_IS_FLOAT\
\n    JUMPIFEQ $FLOAT_OP_IS_ZERO LF@$OP float@0x0.0p+0\
\n    PUSHS bool@true\
\n    RETURN\
\n    LABEL $FLOAT_OP_IS_ZERO\
\n    PUSHS bool@false\
\n    POPFRAME\
\n    RETURN\
\n#\
\nLABEL $BOOLVAL_OP_IS_STR\\
\n    JUMPIFEQ $STR_OP_IS_EMPTY LF@$OP string@\
\n    PUSHS bool@true\
\n    RETURN\
\n    LABEL $STR_OP_IS_EMPTY\
\n    PUSHS bool@false\
\n    RETURN\
\n#\
\nLABEL $BOOLVAL_END\
\n    PUSHS LF@$OP\
\n    POPFRAME\
\n    RETURN\
\n#\
\n###############################################################"

#define CHR "#CHR(op) => prevod int so ascii\
    \nLABEL $ORD\
    \nPUSHFRAME\
    \nINT2CHARS\
    \nPOPFRAME\
    \nRETURN"

#define ORD "#ORD(op) => int value of first char in string\
    \nLABEL $ORD\
    \nPUSHFRAME\
    \nPUSHS int@0\
    \nSTRI2INTS\
    \nPOPFRAME\
    \nRETURN"




#endif