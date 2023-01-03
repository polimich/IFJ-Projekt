//////////////////////////////////////////////////////////
//                                                      //
//    Implementace překladače jazyka IFJ22              //
//                                                      //
//    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    //
//                                                      //
//    Implementace builtins.c: xpoliv07, xmedri01       //
//    Datum: 06. 12. 2022                               //
//                                                      //
//    Licence: GNU GPL v3, nebo pozdější                //
//                                                      //
//////////////////////////////////////////////////////////

#ifndef __BUILTINS_H__
#define __BUILTINS_H__

#define GEN_TO_GOOD_TYPE "LABEL $TO_GOOD_TYPE\
\nCREATEFRAME\
\nDEFVAR TF@$VAL_OP1\
\nDEFVAR TF@$VAL_OP2\
\nDEFVAR TF@$CONVERT_OP2\
\nPOPS TF@$VAL_OP2\
\nPOPS TF@$VAL_OP1\
\nDEFVAR TF@$OP1_TYPE\
\nDEFVAR TF@$OP2_TYPE\
\nTYPE TF@$OP1_TYPE TF@$VAL_OP1\
\nTYPE TF@$OP2_TYPE TF@$VAL_OP2\
\nPUSHS TF@$VAL_OP1\
\nPUSHS TF@$VAL_OP2\
\nJUMPIFEQ $SAME_TYPES TF@$OP1_TYPE TF@$OP2_TYPE\
\nJUMPIFEQ $OP1_IS_INT TF@$OP1_TYPE string@int\
\nJUMPIFEQ $OP1_IS_FLOAT TF@$OP1_TYPE string@float\
\nJUMPIFEQ $OP1_IS_STR TF@$OP1_TYPE string@string\
\nJUMPIFEQ $OP1_IS_BOOL TF@$OP1_TYPE string@bool\
\nLABEL $OP1_IS_INT\
\n    CALL $intval\
\n    JUMP $SAME_TYPES\
\nLABEL $OP1_IS_FLOAT\
\n    CALL $floatval\
\n    JUMP $SAME_TYPES\
\nLABEL $OP1_IS_STR\
\n    CALL $strval\
\n    JUMP $SAME_TYPES\
\nLABEL $OP1_IS_BOOL\
\n    CALL $boolval\
\nLABEL $SAME_TYPES\
\n    CREATEFRAME\
\n    RETURN\n"
#define GEN_INTVAL "################################################################## #\
\n #Vraci hodnotu prevedenou do INT\
\nLABEL $intval\
\n\
\nCREATEFRAME\
\nDEFVAR TF@$OP\
\nDEFVAR TF@$TYPE_OP\
\nPOPS TF@$OP #Naplni OP\
\nTYPE TF@$TYPE_OP TF@$OP\
\nJUMPIFEQ $INTVAL_END TF@$TYPE_OP string@int #INT se na INT nemeni\
\nJUMPIFEQ $INTVAL_OP_IS_FLOAT TF@$TYPE_OP string@float\
\nJUMPIFEQ $INTVAL_OP_IS_STR TF@$TYPE_OP string@string\
\nJUMPIFEQ $INTVAL_OP_IS_BOOL TF@$TYPE_OP string@bool\
\n\
\nLABEL $INTVAL_OP_IS_FLOAT\
\n FLOAT2INT TF@$OP TF@$OP\
\n JUMP $INTVAL_END\
\n\
\nLABEL $INTVAL_OP_IS_STR\
\n #UNSUPPORT TYPE\
\n EXIT int@4\
\n\
\nLABEL $INTVAL_OP_IS_BOOL\
\n JUMPIFNEQ $INTVAL_BOOL_IS_ZERO TF@$OP bool@false\
\n PUSHS int@0\
\n CREATEFRAME\
\n RETURN\
\n LABEL $INTVAL_BOOL_IS_ZERO\
\n PUSHS int@1\
\n CREATEFRAME\
\n RETURN\
\n\
\nLABEL $INTVAL_END\
\n PUSHS TF@$OP\
\n CREATEFRAME\
\n RETURN \n"

#define GEN_FLOATVAL "####################################################################\
\n#Return value converted to FLOAT\
\n#Return value converted to FLOAT\
\nLABEL $floatval\
\n\
\n\
\nCREATEFRAME\
\nDEFVAR TF@$OP\
\nDEFVAR TF@$TYPE_OP\
\nPOPS TF@$OP\
\nTYPE TF@$TYPE_OP TF@$OP\
\nJUMPIFEQ $FLOATVAL_END TF@$TYPE_OP string@float\
\nJUMPIFEQ $FLOATVAL_OP_IS_INT  TF@$TYPE_OP string@int\
\nJUMPIFEQ $FLOATVAL_OP_IS_STR  TF@$TYPE_OP string@string\
\nJUMPIFEQ $FLOATVAL_OP_IS_BOOL TF@$TYPE_OP string@bool\
\nJUMPIFEQ $FLOATVAL_OP_IS_NULL TF@$TYPE_OP string@nil\
\n\
\nLABEL $FLOATVAL_OP_IS_INT\
\n    INT2FLOAT TF@$OP TF@$OP\
\n    JUMP $FLOATVAL_END\
\n\
\nLABEL $FLOATVAL_OP_IS_BOOL\
\n    JUMPIFEQ $FLOATVAL_BOOL_IS_ZERO TF@$OP bool@false\
\n    PUSHS float@0x1.0p+0\
\n    CREATEFRAME\
\n    RETURN\
\n    LABEL $FLOATVAL_BOOL_IS_ZERO\
\n    PUSHS float@0x0.0p+0\
\n    CREATEFRAME\
\n    RETURN\
\n\
\nLABEL $FLOATVAL_OP_IS_STR      \
\n#Unsupport type\
\n    EXIT int@4\
\n\
\nLABEL $FLOATVAL_OP_IS_NULL\
\n    PUSHS float@0x0.0p+0\
\n    CREATEFRAME\
\n    RETURN\
\n\
\nLABEL $FLOATVAL_END\
\n    PUSHS TF@$OP\
\n    CREATEFRAME\
\n    RETURN\n"

#define GEN_STRVAL "################################################################\
\n#Vraci hodnotu prevedenou do STR\
\nLABEL $strval\
\n\
\n\
\nCREATEFRAME\
\n\
\nDEFVAR TF@$OP\
\nDEFVAR TF@$TYPE_OP\
\nPOPS TF@$OP\
\nTYPE TF@$TYPE_OP TF@$OP\
\nJUMPIFEQ $STRVAL_END TF@$TYPE_OP string@string\
\nJUMPIFEQ $STRVAL_OP_IS_INT  TF@$TYPE_OP string@int\
\nJUMPIFEQ $STRVAL_OP_IS_FLOAT  TF@$TYPE_OP string@float\
\nJUMPIFEQ $STRVAL_OP_IS_BOOL TF@$TYPE_OP string@bool\
\nJUMPIFEQ $STRVAL_OP_IS_NULL TF@$TYPE_OP string@nil\
\n\
\nLABEL $STRVAL_OP_IS_FLOAT\
\nLABEL $STRVAL_OP_IS_INT\
\n#UNSUPPORT TYPE\
\n    EXIT int@4\
\n\
\n\
\nLABEL $STRVAL_OP_IS_BOOL\
\n    JUMPIFEQ $STRVAL_BOOL_IS_ZERO TF@$OP bool@false \
\n    PUSHS string@true\
\n    CREATEFRAME\
\n    RETURN\
\n    LABEL $STRVAL_BOOL_IS_ZERO\
\n    PUSHS string@false  \
\n    CREATEFRAME                           \
\n    RETURN\
\n\
\nLABEL $STRVAL_OP_IS_NULL\
\n    PUSHS string@\
\n    RETURN\
\n\
\nLABEL $STRVAL_END\
\n    PUSHS TF@$OP\
\n    CREATEFRAME\
\n    RETURN\n"

#define GEN_BOOLVAL "##########################################################\
\n#Vraci hodnotu prevedenou do BOOL\
\nLABEL $boolval\
\n\
\nCREATEFRAME\
\nDEFVAR TF@$OP\
\nDEFVAR TF@$TYPE_OP\
\nPOPS TF@$OP\
\nTYPE TF@$TYPE_OP TF@$OP\
\n\
\nJUMPIFEQ $BOOLVAL_END TF@$TYPE_OP string@bool\
\nJUMPIFEQ $BOOLVAL_OP_IS_INT  TF@$TYPE_OP string@int\
\nJUMPIFEQ $BOOLVAL_OP_IS_FLOAT  TF@$TYPE_OP string@float\
\nJUMPIFEQ $BOOLVAL_OP_IS_STR TF@$TYPE_OP string@string\
\nJUMPIFEQ $BOOLVAL_OP_IN_NULL TF@$TYPE_OP string@nil\
\n\
\nLABEL $BOOLVAL_OP_IS_INT\
\n    JUMPIFEQ $INT_OP_IS_ZERO TF@$OP int@0\
\n    PUSHS bool@true\
\n    CREATEFRAME\
\n    RETURN\
\n    LABEL $INT_OP_IS_ZERO\
\n    PUSHS bool@false\
\n    CREATEFRAME\
\n    RETURN\
\n\
\nLABEL $BOOLVAL_OP_IS_FLOAT\
\n    JUMPIFEQ $FLOAT_OP_IS_ZERO TF@$OP float@0x0.0p+0\
\n    PUSHS bool@true\
\n    CREATEFRAME\
\n    RETURN\
\n    LABEL $FLOAT_OP_IS_ZERO\
\n    PUSHS bool@false\
\n    CREATEFRAME\
\n    RETURN\
\n\
\nLABEL $BOOLVAL_OP_IS_STR    \
\n    JUMPIFEQ $STR_OP_IS_EMPTY TF@$OP string@\
\n    PUSHS bool@true\
\n    CREATEFRAME\
\n    RETURN\
\n    LABEL $STR_OP_IS_EMPTY\
\n    PUSHS bool@false\
\n    CREATEFRAME\
\n    RETURN\
\n\
\nLABEL $BOOLVAL_OP_IN_NULL\
\n    PUSHS bool@false\
\n    CREATEFRAME\
\n    RETURN\
\n\
\nLABEL $BOOLVAL_END\
\n    PUSHS TF@$OP\
\n    CREATEFRAME\
\n    RETURN\n"

#define GEN_ASCII_FUN "\n###############################################################\
\n#ORD & CHR\
\n###############################################################\
\n#ORD(op) => char to ascii int \
\nLABEL $ORD\
\nCALL $strval\
\nPUSHS int@0\
\nSTRI2INTS\
\nRETURN\
\n###############################################################\
\n#CHR(op) => ascii int to chr\
\nLABEL $CHR\
\nCALL $intval\
\nINT2CHARS\
\nRETURN\n"

#define GEN_LOGIC_FUN "###############################################################\
\n#COMPARE\
\n###############################################################\
\n#LT(op1, op2)  <\
\n#Vrací true/false na stack\
\nLABEL $LT\
\nCALL $TO_GOOD_TYPE\
\nLTS\
\nRETURN\
\n###############################################################\
\n#GT(op1, op2)  >\
\n#Vrací true/false na stack\
\nLABEL $GT\
\nCALL $TO_GOOD_TYPE\
\nGTS\
\nRETURN\
\n################################################################\
\n#EQ(op1, op2)   ==\
\n#Vrací true/false na stack\
\nLABEL $EQ\
\nCALL $TO_GOOD_TYPE\
\nEQS\
\nRETURN\
\n###############################################################\
\n#LTEQ(op1, op2)  LT || EQ\
\n#Vrací true/false na stack\
\nLABEL $LTEQ\
\nCALL $TO_GOOD_TYPE\
\n\
\nCREATEFRAME\
\nDEFVAR TF@$OP1\
\nDEFVAR TF@$OP2\
\nDEFVAR TF@$LT_RES\
\nDEFVAR TF@$EQ_RES\
\nDEFVAR TF@$OR_RES\
\nPOPS TF@$OP1\
\nPOPS TF@$OP2\
\nLT TF@$LT_RES TF@$OP1 TF@$OP2\
\nEQ TF@$EQ_RES TF@$OP1 TF@$OP2\
\nOR TF@$OR_RES TF@$LT_RES TF@$EQ_RES\
\nPUSHS TF@$OR_RES\
\nCREATEFRAME\
\nRETURN\
\n###############################################################\
\n#GTEQ(op1, op2)  GT || EQ\
\n#Vrací true/false na stack\
\nLABEL $GTEQ\
\nCALL $TO_GOOD_TYPE\
\n\
\nCREATEFRAME\
\nDEFVAR TF@$OP1\
\nDEFVAR TF@$OP2\
\nDEFVAR TF@$GT_RES\
\nDEFVAR TF@$EQ_RES\
\nDEFVAR TF@$OR_RES\
\nPOPS TF@$OP1\
\nPOPS TF@$OP2\
\nGT TF@$GT_RES TF@$OP1 TF@$OP2\
\nEQ TF@$EQ_RES TF@$OP1 TF@$OP2\
\nOR TF@$OR_RES TF@$GT_RES TF@$EQ_RES\
\nPUSHS TF@$OR_RES\
\nCREATEFRAME\
\nRETURN\n"

#define GEN_ARIT_CHECK "\n###############################################################\
\n#ARITMETIC\
\n###############################################################\
\n\
\n#funkce kontroluje a pripadne nastavuje typy tak \
\n#aby se s nimi dali provézt aritemtické operace\
\n#ARITMETICT_TYPE(op1, op2)\
\n#op1, op2 mohou být typ int, float, bool\
\n\
\n#Povolené INT, FLOAT a BOOL\
\nLABEL $ARITMETICT_TYPE  \
\n\
\n\
\nCREATEFRAME\
\n\
\nDEFVAR TF@$op2\
\nDEFVAR TF@op1$type\
\nDEFVAR TF@$op1\
\nDEFVAR TF@op2$type\
\n\
\nPOPS TF@$op1\
\nPOPS TF@$op2\
\nTYPE TF@op1$type TF@$op1\
\nTYPE TF@op2$type TF@$op2\
\nJUMPIFEQ $ATR_STR TF@op1$type string@string\
\nJUMPIFNEQ $ART_NOT_EQL_TYPES TF@op2$type string@string\
\nLABEL $ATR_STR\
\n    EXIT int@4\
\n\
\nLABEL $ART_NOT_EQL_TYPES\
\n\
\nJUMPIFNEQ $IS_op2_NULL TF@op1$type string@nil\
\n    PUSHS TF@$op1\
\n    PUSHFRAME\
\n    CALL $intval\
\n    POPFRAME\
\n    POPS TF@$op1\
\n    TYPE TF@op1$type TF@$op1\
\n\
\nLABEL $IS_op2_NULL\
\nJUMPIFNEQ $OP2_NON_NULL TF@op2$type string@nil\
\nLABEL $op2_to_int\
\n    PUSHS TF@$op2\
\n    PUSHFRAME\
\n    CALL $intval\
\n    POPFRAME\
\n    POPS TF@$op2\
\n    TYPE TF@op2$type TF@$op2\
\nLABEL $OP2_NON_NULL\
\n\
\nJUMPIFEQ $ART_CONVERT_OP2_TO_FLOAT TF@op1$type string@float\
\nJUMPIFEQ $ART_CONVERT_OP1_TO_FLOAT TF@op2$type string@float\
\nJUMPIFEQ $ART_CONVERT_OP1_FROM_BOOL TF@op1$type string@bool\
\nJUMPIFEQ $ART_CONVERT_OP2_FROM_BOOL TF@op2$type string@bool\
\nPUSHS TF@$op2\
\nPUSHS TF@$op1\
\nJUMP $ART_END\
\n\
\nLABEL $ART_CONVERT_OP1_TO_FLOAT\
\n    PUSHS TF@$op2\
\n    PUSHS TF@$op1\
\n    CALL $TO_GOOD_TYPE\
\n    JUMP $ART_END\
\n\
\nLABEL $ART_CONVERT_OP2_TO_FLOAT\
\n    PUSHS TF@$op1\
\n    PUSHS TF@$op2\
\n    CALL $TO_GOOD_TYPE\
\n    CALL $SWITCH_IN_STACK\
\n    JUMP $ART_END\
\n\
\nLABEL $ART_CONVERT_OP1_FROM_BOOL\
\n    JUMPIFEQ $ART_BOTH_BOOL TF@op1$type TF@op2$type\
\n    PUSHS TF@$op2\
\n    PUSHS TF@$op1\
\n    CALL $TO_GOOD_TYPE\
\n    JUMP $ART_END\
\n\
\nLABEL $ART_CONVERT_OP2_FROM_BOOL\
\n    PUSHS TF@$op1\
\n    PUSHS TF@$op2\
\n    CALL $TO_GOOD_TYPE\
\n    CALL $SWITCH_IN_STACK\
\n    JUMP $ART_END\
\n\
\nLABEL $ART_BOTH_BOOL\
\n    PUSHS TF@$op2\
\n    CALL $intval\
\n    PUSHS TF@$op1\
\n    CALL $intval\
\n    JUMP $ART_END\
\n\
\nLABEL $ART_END\
\nCREATEFRAME\
\nRETURN\n"

#define GEN_STACK_SWITCH "\n############################################\
\nLABEL $SWITCH_IN_STACK\
\n\
\nCREATEFRAME\
\nDEFVAR TF@$op1\
\nDEFVAR TF@$op2\
\nPOPS TF@$op1\
\nPOPS TF@$op2\
\nPUSHS TF@$op1\
\nPUSHS TF@$op2\
\nCREATEFRAME\
\nRETURN"

#define GEN_ARIT_FUNC "################################################\
\nLABEL $MUL\
\nCALL $ARITMETICT_TYPE\
\nMULS\
\nRETURN\
\n#####################################################################\
\nLABEL $DIV\
\n\
\nCALL $floatval\
\nCALL $SWITCH_IN_STACK\
\nCALL $floatval\
\nCALL $SWITCH_IN_STACK\
\nDIVS\
\nRETURN\
\n#####################################################################\
\nLABEL $ADD\
\nCALL $ARITMETICT_TYPE\
\nADDS\
\nRETURN\
\n#####################################################################\
\nLABEL $SUB\
\nCALL $ARITMETICT_TYPE\
\nSUBS\
\nRETURN\n"

#define GEN_WRITE "LABEL $WRITE\
\nCREATEFRAME\
\nDEFVAR TF@wr\
\nPOPS TF@wr\
\nWRITE TF@wr\
\nCREATEFRAME\
\nRETURN\n"

#define GEN_READS "LABEL $READS\
\nCREATEFRAME\
\nDEFVAR TF@retval\
\nREAD TF@retval string\
\nPUSHS TF@retval\
\nCREATEFRAME\
\nRETURN\n"

#define GEN_READI "LABEL $READI\
\nCREATEFRAME\
\nDEFVAR TF@retval\
\nREAD TF@retval int\
\nPUSHS TF@retval\
\nCREATEFRAME\
\nRETURN\n"

#define GEN_READF "LABEL $READF\
\nCREATEFRAME\
\nDEFVAR TF@retval\
\nREAD TF@retval float\
\nPUSHS TF@retval\
\nCREATEFRAME\
\nRETURN\n"

#define GEN_SUBSTR "LABEL $SUBSTRING\
\nCREATEFRAME\
\nDEFVAR TF@$full_string\
\nCALL $strval\
\nPOPS TF@$full_string\
\nDEFVAR TF@$first_idx\
\nCALL $intval\
\nPOPS TF@$first_idx\
\nDEFVAR TF@$last_idx\
\nCALL $intval\
\nPOPS TF@$last_idx\
\nDEFVAR TF@$test\
\nDEFVAR TF@$SUB_STR\
\nGT TF@$test TF@$first_idx int@0\
\nJUMPIFEQ $BAD_END_SUBSTR TF@$test bool@false\
\nGT TF@$test TF@$last_idx TF@$first_idx\
\nJUMPIFEQ $BAD_END_SUBSTR TF@$test bool@false\
\nDEFVAR TF@$len\
\nSUB TF@$len TF@$last_idx TF@$first_idx\
\nGT TF@$test TF@$len int@0\
\nJUMPIFEQ $BAD_END_SUBSTR TF@$test bool@false\
\n    PUSHS TF@$full_string\
\n    CALL $STRLEN\
\n    DEFVAR TF@$STR_LEN\
\n    POPS TF@$STR_LEN\
\n    GT TF@$test TF@$STR_LEN TF@$len\
\n    JUMPIFEQ $BAD_END_SUBSTR TF@$test bool@false\
\n        GT TF@$test TF@$STR_LEN TF@$last_idx\
\n     JUMPIFEQ $BAD_END_SUBSTR TF@$test bool@false\
\nMOVE TF@$SUB_STR string@\
\nDEFVAR TF@$char\
\nLABEL $STR_FOR\
\n    GETCHAR  TF@$char TF@$full_string TF@$first_idx\
\n    CONCAT TF@$SUB_STR TF@$SUB_STR TF@$char\
\n    ADD TF@$first_idx TF@$first_idx int@1\
\n    JUMPIFNEQ $STR_FOR TF@$first_idx TF@$last_idx\
\nPUSHS TF@$SUB_STR\
\nCREATEFRAME\
\nRETURN\
\nLABEL $BAD_END_SUBSTR\
\nMOVE TF@$SUB_STR nil@nil\
\nPUSHS TF@$SUB_STR\
\nCREATEFRAME\
\nRETURN\n"

#define GEN_STRLEN "LABEL $STRLEN\
\nCREATEFRAME\
\nDEFVAR TF@$LEN_OF_STR\
\nCALL $strval\
\nPOPS TF@$LEN_OF_STR\
\nSTRLEN TF@$LEN_OF_STR TF@$LEN_OF_STR\
\nPUSHS TF@$LEN_OF_STR\
\nCREATEFRAME\
\nRETURN\n"
#endif

#define GEN_CONCAT "#Concatenation of two strings\
\nLABEL $CONCAT\
\nCREATEFRAME\
\nDEFVAR TF@$op1\
\nDEFVAR TF@$op2\
\nPOPS TF@$op2\
\nPUSHFRAME\
\nCALL $strval\
\nPOPFRAME\
\nPOPS TF@$op1\
\nPUSHS TF@$op2\
\nPUSHFRAME\
\nCALL $strval\
\nPOPFRAME\
\nPOPS TF@$op2\
\nCONCAT TF@$op1 TF@$op1 TF@$op2\
\nPUSHS TF@$op1\
\nCREATEFRAME\
\nRETURN\n"
